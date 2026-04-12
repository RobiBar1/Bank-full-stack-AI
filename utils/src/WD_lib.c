#define _POSIX_C_SOURCE 200809L

#include <assert.h>    /* assert */
#include <errno.h>     /* errno, EINTR */
#include <fcntl.h>     /* O_CREAT */
#include <limits.h>    /* CHAR_BIT */
#include <pthread.h>   /* pthread_create, pthread_join */
#include <semaphore.h> /* sem_t, sem_open, sem_wait, sem_post */
#include <signal.h>    /* sigaction, kill */
#include <stdio.h>     /* printf, perror */
#include <stdlib.h>    /* unsetenv, getenv, malloc, free */
#include <string.h>    /* strdup */
#include <sys/stat.h>  /* mode constants */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* waitpid, WNOHANG */
#include <unistd.h>    /* fork, execv */

#include "scheduler.h"           /* scheduler_t, SchedulerRun */
#include "watchdog_controller.h" /* watchdog_status_t */

#ifndef NDEBUG
#define DEBUG_PRINT(args) (printf args)
#else
#define DEBUG_PRINT(args)
#endif

#define UNUSED(x) (void)(x)

#define TIME_INTERVAL_FOR_PINGS 2

#define MAX_MISSED_HEARTBEATS 3
#define FORK_OR_EXECV_FAILED -1
#define MAX_PID_LEN 32
#define NOT_SUCCESS 1
#define SUCCESS 0

#define ARGV_I_ENTERED 2

#define SEM_PROCCESS "/process_ready"
#define SEM_THREAD "/watchdog_thread_ready"
#define EVN_VARIABLE_NAME "WD_PID"

typedef struct thread_info
{
    const char* prog_to_watch;
    char** argv;
    pid_t pid_to_watch;
} thread_info_t;

typedef struct wd_runtime
{
    const char* prog_to_revive;
    scheduler_t* scheduler;
    char* const* argv;
    sem_t* sem_to_signal;
    sem_t* sem_to_wait;
    pid_t pid_to_watch;
} wd_runtime_t;

static volatile sig_atomic_t g_is_stopped = 0;
static volatile sig_atomic_t g_heartbeats = 0;
static pthread_t g_panthom_thread = 0;

static void LogAndExit(char* msg, int status)
{
    perror(msg);
    _exit(status);
}

static void DummyCleanup(void* param) { UNUSED(param); }

static void BlockSignals(void)
{
    sigset_t set = {0};

    if (SUCCESS != sigemptyset(&set) || SUCCESS != sigaddset(&set, SIGUSR1) ||
        SUCCESS != sigaddset(&set, SIGUSR2))
    {
        LogAndExit("sigaddset() failed", NOT_SUCCESS);
    }

    if (SUCCESS != pthread_sigmask(SIG_BLOCK, &set, NULL))
    {
        LogAndExit("pthread_sigmask(SIG_BLOCK) failed", NOT_SUCCESS);
    }
}

static void UnblockSignals(void)
{
    sigset_t set;

    if (SUCCESS != sigemptyset(&set) || SUCCESS != sigaddset(&set, SIGUSR1) ||
        SUCCESS != sigaddset(&set, SIGUSR2))
    {
        LogAndExit("sigaddset() failed", NOT_SUCCESS);
    }

    if (SUCCESS != pthread_sigmask(SIG_UNBLOCK, &set, NULL))
    {
        LogAndExit("pthread_sigmask(SIG_UNBLOCK) failed", NOT_SUCCESS);
    }
}

static pid_t SpawnProcess(const char* prog_name, char* const* argv)
{
    pid_t pid = 0;

    assert(NULL != prog_name);
    assert(NULL != argv);

    pid = fork();
    if (FORK_OR_EXECV_FAILED == pid)
    {
        perror("fork");
    }
    else if (0 == pid)
    {
        execv(prog_name, argv);
        perror("execv");
        pid = FORK_OR_EXECV_FAILED;
    }

    return pid;
}

static void Revive(wd_runtime_t* runtime_args)
{
    assert(NULL != runtime_args);

    DEBUG_PRINT(("PID %d is starting revive process...\n", getpid()));

    kill(runtime_args->pid_to_watch, SIGTERM);
    runtime_args->pid_to_watch =
        SpawnProcess(runtime_args->prog_to_revive, runtime_args->argv);
    if (FORK_OR_EXECV_FAILED == runtime_args->pid_to_watch)
    {
        SchedulerDestroy(runtime_args->scheduler);
        LogAndExit("SpawnProcess() failed", NOT_SUCCESS);
    }

    g_heartbeats = 0;
    sem_post(runtime_args->sem_to_signal);
    while (SUCCESS != sem_wait(runtime_args->sem_to_wait))
        ;
}

static task_status SendSigUsr1(void* param)
{
    wd_runtime_t* runtime_args = (wd_runtime_t*)param;

    assert(NULL != runtime_args);

    if (!g_is_stopped)
    {
        DEBUG_PRINT(
            ("%d: send SIGUSR1 to %d\n", getpid(), runtime_args->pid_to_watch));
        kill(runtime_args->pid_to_watch,
             SIGUSR1); /*this before sync or after?*/
        __sync_fetch_and_add(&g_heartbeats, 1);
    }

    return REPEAT;
}

static task_status CheckAndRevive(void* param)
{
    wd_runtime_t* runtime_args = (wd_runtime_t*)param;

    assert(NULL != runtime_args);

    if (!g_is_stopped && g_heartbeats > MAX_MISSED_HEARTBEATS)
    {
        Revive(runtime_args);
    }

    return REPEAT;
}

static task_status CheckIsStop(void* param)
{
    wd_runtime_t* runtime_args = (wd_runtime_t*)param;

    assert(NULL != runtime_args);

    if (g_is_stopped)
    {
        SchedulerStop(runtime_args->scheduler);
        kill(runtime_args->pid_to_watch, SIGUSR2);

        return DO_NOT_REPEAT;
    }

    return REPEAT;
}

static int InitScheduler(scheduler_t* scheduler, wd_runtime_t* runtime_args)
{
    ilrd_uid_t uid1 = {0};
    ilrd_uid_t uid2 = {0};
    ilrd_uid_t uid3 = {0};

    assert(NULL != scheduler);
    assert(NULL != runtime_args);

    uid1 = SchedulerAddTask(scheduler, TIME_INTERVAL_FOR_PINGS, SendSigUsr1,
                            DummyCleanup, runtime_args);
    uid2 = SchedulerAddTask(scheduler, TIME_INTERVAL_FOR_PINGS, CheckAndRevive,
                            DummyCleanup, runtime_args);
    uid3 =
        SchedulerAddTask(scheduler, 1, CheckIsStop, DummyCleanup, runtime_args);

    if (IsILRDUIDEqual(&bad_uid, &uid1) || IsILRDUIDEqual(&bad_uid, &uid2) ||
        IsILRDUIDEqual(&bad_uid, &uid3))
    {
        return NOT_SUCCESS;
    }

    return SUCCESS;
}

static void InitOneSignalHandler(int signum, void (*handler)(int))
{
    struct sigaction sa = {0};

    assert(NULL != handler);

    sa.sa_handler = handler;
    if (SUCCESS != sigemptyset(&sa.sa_mask) ||
        SUCCESS != sigaction(signum, &sa, NULL))
    {
        LogAndExit("InitOneSignalHandler() failed", NOT_SUCCESS);
    }
}

static void SigUsr1Handler(int signum)
{
    UNUSED(signum);
    g_heartbeats = 0;
}

static void SigUsr2Handler(int signum)
{
    UNUSED(signum);
    g_is_stopped = 1;
}

static void InitAllSignalHandlers()
{
    InitOneSignalHandler(SIGUSR1, SigUsr1Handler);
    InitOneSignalHandler(SIGUSR2, SigUsr2Handler);
    InitOneSignalHandler(SIGCHLD, SIG_IGN);
}

static int InitSemaphores(wd_runtime_t* runtime_args,
                          const char* path_to_sem_signal,
                          const char* path_to_sem_wait)
{
    assert(NULL != runtime_args);
    assert(NULL != path_to_sem_signal);
    assert(NULL != path_to_sem_wait);

    runtime_args->sem_to_signal =
        sem_open(path_to_sem_signal, O_CREAT, 0666, 0);
    if (SEM_FAILED == runtime_args->sem_to_signal)
    {
        return NOT_SUCCESS;
    }

    runtime_args->sem_to_wait = sem_open(path_to_sem_wait, O_CREAT, 0666, 0);
    if (SEM_FAILED == runtime_args->sem_to_wait)
    {
        sem_close(runtime_args->sem_to_signal);
        sem_unlink(path_to_sem_signal);

        return NOT_SUCCESS;
    }

    return SUCCESS;
}

static void SemaCleanUp(wd_runtime_t* runtime_args,
                        const char* path_to_sem_signal,
                        const char* path_to_sem_wait)
{
    sem_close(runtime_args->sem_to_signal);
    sem_unlink(path_to_sem_signal);
    sem_close(runtime_args->sem_to_wait);
    sem_unlink(path_to_sem_wait);
}

static void InitTimeAndSyhnronicSystem(wd_runtime_t* runtime_args,
                                       const char* path_to_sem_signal,
                                       const char* path_to_sem_wait)
{
    assert(NULL != runtime_args);

    runtime_args->scheduler = SchedulerCreate();
    if (NULL == runtime_args->scheduler)
    {
        LogAndExit("SchedulerCreate() failed", NOT_SUCCESS);
    }

    if (SUCCESS != InitScheduler(runtime_args->scheduler, runtime_args))
    {
        SchedulerDestroy(runtime_args->scheduler);
        LogAndExit("InitScheduler() failed", NOT_SUCCESS);
    }

    if (SUCCESS !=
        InitSemaphores(runtime_args, path_to_sem_signal, path_to_sem_wait))
    {
        SchedulerDestroy(runtime_args->scheduler);
        LogAndExit("InitSemaphores() failed", NOT_SUCCESS);
    }
}

void StartWatching(const char* prog_to_watch, pid_t pid_to_watch,
                   char* const* argv, const char* path_to_sem_signal,
                   const char* path_to_sem_wait)
{
    wd_runtime_t runtime_args = {0};

    assert(NULL != path_to_sem_signal);
    assert(NULL != path_to_sem_wait);
    assert(NULL != prog_to_watch);
    assert(NULL != argv);

    InitAllSignalHandlers();
    UnblockSignals();
    InitTimeAndSyhnronicSystem(&runtime_args, path_to_sem_signal,
                               path_to_sem_wait);

    runtime_args.pid_to_watch = pid_to_watch;
    runtime_args.prog_to_revive = prog_to_watch;
    runtime_args.argv = argv;

    sem_post(runtime_args.sem_to_signal);
    while (SUCCESS != sem_wait(runtime_args.sem_to_wait) && errno == EINTR)
        ;

    DEBUG_PRINT(("PID %d just start watching PID %d\n", getpid(),
                 runtime_args.pid_to_watch));

    SchedulerRun(runtime_args.scheduler);
    SchedulerDestroy(runtime_args.scheduler);
    SemaCleanUp(&runtime_args, path_to_sem_signal, path_to_sem_wait);
}

static size_t CountArgv(char* argv[])
{
    size_t count = 0;

    assert(NULL != argv);

    while (NULL != argv[count])
    {
        ++count;
    }

    return count;
}

static char** CreateArgv(const char* watchdog_path, char* argv[])
{
    char my_pid[MAX_PID_LEN] = {0};
    char** complete_argv = NULL;
    size_t complete_argc = 0;
    size_t i = 0;

    assert(NULL != watchdog_path);
    assert(NULL != argv);

    complete_argc = CountArgv(argv) + ARGV_I_ENTERED;
    complete_argv = (char**)malloc(complete_argc * sizeof(char*));
    if (NULL == complete_argv)
    {
        return NULL;
    }

    snprintf(my_pid, MAX_PID_LEN, "%d", getpid());
    complete_argv[0] = (char*)watchdog_path;
    complete_argv[1] = strdup(my_pid);
    if (NULL == complete_argv[1])
    {
        free(complete_argv);
        complete_argv = NULL;

        return NULL;
    }

    for (i = ARGV_I_ENTERED; i < complete_argc; ++i)
    {
        complete_argv[i] = (char*)argv[i - ARGV_I_ENTERED];
    }

    return complete_argv;
}

static pid_t CheckAndSpawnWatchdog(const char* watchdog_path, char* const* argv)
{
    char* str_pid = NULL;
    pid_t pid = 0;

    str_pid = getenv(EVN_VARIABLE_NAME);
    if (NULL == str_pid)
    {
        DEBUG_PRINT(("First Init. Creating new watchdog\n"));
        pid = SpawnProcess(watchdog_path, argv);
    }
    else
    {
        DEBUG_PRINT(("Watchdog exists. pid: %s\n", str_pid));
        pid = strtol(str_pid, NULL, 10);
    }

    return pid;
}

static void* ThreadFunc(void* arg)
{
    thread_info_t* thread_info = (thread_info_t*)arg;

    assert(NULL != thread_info);

    UnblockSignals();
    StartWatching(thread_info->prog_to_watch, thread_info->pid_to_watch,
                  thread_info->argv, SEM_PROCCESS, SEM_THREAD);

    free(thread_info->argv[1]);
    thread_info->argv[1] = NULL;
    free((void*)thread_info->argv);
    thread_info->argv = NULL;
    free(thread_info);
    thread_info = NULL;

    return NULL;
}

static int CreateThread(thread_info_t* thread_info)
{
    if (SUCCESS !=
        pthread_create(&g_panthom_thread, NULL, ThreadFunc, thread_info))
    {
        kill(thread_info->pid_to_watch, SIGTERM);

        return NOT_SUCCESS;
    }

    return SUCCESS;
}

static void CleanUpThread(thread_info_t* thread_info)
{
    UnblockSignals();
    free(thread_info->argv[1]);
    thread_info->argv[1] = NULL;
    free((void*)thread_info->argv);
    thread_info->argv = NULL;
    free(thread_info);
    thread_info = NULL;
}
/*============================= API Fcuns ==============================*/
watchdog_status_t WatchdogControllerStart(const char* watchdog_path,
                                          char* argv[])
{
    thread_info_t* thread_info = NULL;

    assert(NULL != watchdog_path);
    assert(NULL != argv);

    BlockSignals();

    thread_info = (thread_info_t*)malloc(sizeof(thread_info_t));
    if (NULL == thread_info)
    {
        UnblockSignals();

        return WD_THREAD_FAIL;
    }

    thread_info->argv = CreateArgv(watchdog_path, argv);
    if (NULL == thread_info->argv)
    {
        UnblockSignals();
        free(thread_info);
        thread_info = NULL;

        return WD_THREAD_FAIL;
    }

    thread_info->prog_to_watch = watchdog_path;
    thread_info->pid_to_watch =
        CheckAndSpawnWatchdog(watchdog_path, thread_info->argv);
    if (FORK_OR_EXECV_FAILED == thread_info->pid_to_watch)
    {
        CleanUpThread(thread_info);

        return WD_FORK_FAIL;
    }
    if (SUCCESS != CreateThread(thread_info))
    {
        CleanUpThread(thread_info);

        return WD_THREAD_FAIL;
    }

    return WD_SUCCESS;
}

void WatchdogControllerEnd(void)
{
    g_is_stopped = 1;
    pthread_join(g_panthom_thread, NULL);

    sem_unlink(SEM_PROCCESS);
    sem_unlink(SEM_THREAD);

    DEBUG_PRINT(("PID %d finished WatchdogControllerEnd\n", getpid()));
}