/*
Writer: Robi
Chcker: Ehud
Date: 09.04.2026

terminal:
gcc -g -O0 -I include/ -I ../ds/include ../ds/src/scheduler.c \
../ds/src/pqueue.c ../ds/src/heap.c ../ds/src/vector.c ../ds/src/uid.c \
../ds/src/task.c -shared -fPIC src/WD_lib.c -lpthread -o libwatchdog.so

------------------------------------------------------->>>
gcc -g -O0 -std=c89 -I ../ds/include src/WD.c ../ds/src/scheduler.c \
../ds/src/uid.c ../ds/src/task.c ../ds/src/pqueue.c ../ds/src/heap.c \
../ds/src/vector.c -L. -lwatchdog -lpthread -o wd


if change the name of file from "wd"
go change the define WD_EXE_PATH in "WD_client.c" file either!
------------------------------------------------------->>>
gcc -g -O0 -I include/ -std=c89 src/WD_client.c -L. -lwatchdog -lpthread

LD_LIBRARY_PATH=. ./a.out
*/
#define _POSIX_C_SOURCE 200809L

#include <assert.h>    /* assert */
#include <fcntl.h>     /* O_CREAT */
#include <limits.h>    /* CHAR_BIT */
#include <pthread.h>   /* pthread_create, pthread_join */
#include <semaphore.h> /* sem_t, sem_open, sem_wait, sem_post */
#include <signal.h>    /* sigaction, kill */
#include <stdio.h>     /* printf, perror */
#include <stdlib.h>    /* unsetenv, getenv, malloc, free */
#include <sys/stat.h>  /* mode constants */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* waitpid, WNOHANG */
#include <unistd.h>    /* fork, execv */

#include "scheduler.h"           /* scheduler_t, SchedulerRun */
#include "watchdog_controller.h" /* watchdog_status_t */

#define UNUSED(x) (void)(x)
#define SUCCESS 0
#define NOT_SUCCESS (-1)

/*========================== use them in struct ======================*/
#define NUM_BITS_USED_IN_STRUCT 2
#define NUM_OF_BYTS_IN_INT 4
#define SIZE_IN_BITS ((CHAR_BIT) * NUM_OF_BYTS_IN_INT - NUM_BITS_USED_IN_STRUCT)

#define WD_INTERVAL 3
#define MAX_MISSED_PINGS 3

#define REVIVED_ENV "WD_REVIVED"
#define WD_PID_ENV "WD_EXISTING_PID"
#define NUM_ARGV_I_USED 3
#define NUM_ARGV_I_USED_AND_NULL (NUM_ARGV_I_USED + 1)

#define WD_SEM_CLIENT "/wd_sem_client"
#define WD_SEM_WD "/wd_sem_wd"

#ifndef NDEBUG
#define DEBUG_PRINT(args) (printf args)
#else
#define DEBUG_PRINT(args)
#endif

typedef struct wd_params
{
    scheduler_t* scheduler;
    const char* wd_path;
    const char* prog_path;
    char** argv;
    sem_t* sem_client;
    sem_t* sem_wd;
    volatile sig_atomic_t* got_signal;
    volatile sig_atomic_t* is_running;
    pthread_t wd_thread;
    pid_t target_pid;
    int argc;
    unsigned int missed_counter : SIZE_IN_BITS;
    unsigned int is_target_child : 1;
    unsigned int is_client : 1;
} wd_params_t;

static wd_params_t* g_client_params = NULL;
static volatile sig_atomic_t g_got_signal = 1;
static volatile sig_atomic_t g_running = 1;

static void Sigusr2Handler(int sig)
{
    UNUSED(sig);
    g_got_signal = 1;
}

static void DummyCleanup(void* param) { UNUSED(param); }

static void CleanUpGlobals(void)
{
    free(g_client_params);
    g_client_params = NULL;
}

static void CleanUpSem(void)
{
    sem_close(g_client_params->sem_client);
    g_client_params->sem_client = NULL;
    sem_unlink(WD_SEM_CLIENT);

    sem_close(g_client_params->sem_wd);
    g_client_params->sem_wd = NULL;
    sem_unlink(WD_SEM_WD);
}

static void InitHandlers(char** revived_env, char** wd_pid_env)
{
    struct sigaction sa = {0};
    sigset_t set = {0};

    assert(NULL != revived_env);
    assert(NULL != wd_pid_env);

    sigemptyset(&sa.sa_mask);
    sa.sa_handler = Sigusr2Handler;
    sigaction(SIGUSR2, &sa, NULL);

    sigemptyset(&set);
    sigaddset(&set, SIGUSR2);
    sigaddset(&set, SIGTERM);
    sigprocmask(SIG_UNBLOCK, &set, NULL);

    *revived_env = getenv(REVIVED_ENV);
    *wd_pid_env = getenv(WD_PID_ENV);
}

static char** CreateWdArgv(const char*** wd_argv, const wd_params_t* params,
                           char* client_pid_str)
{
    size_t i = 0;

    assert(NULL != params);
    assert(NULL != client_pid_str);

    *wd_argv = (const char**)malloc(sizeof(char*) *
                                    (params->argc + NUM_ARGV_I_USED_AND_NULL));
    if (NULL == *wd_argv)
    {
        perror("wd_argv malloc failed");

        _exit(1);
    }

    (*wd_argv)[0] = (char*)params->wd_path;
    (*wd_argv)[1] = client_pid_str;
    (*wd_argv)[2] = (char*)params->prog_path;
    for (; i < params->argc; ++i)
    {
        (*wd_argv)[NUM_ARGV_I_USED + i] = params->argv[i];
    }

    (*wd_argv)[NUM_ARGV_I_USED + params->argc] = NULL;
}

static void ExecWD(wd_params_t* params)
{
    char client_pid_str[32] = {0};
    const char** wd_argv = NULL;
    int i = 0;

    assert(NULL != params);

    sprintf(client_pid_str, "%d", (int)getppid());

    CreateWdArgv(&wd_argv, params, client_pid_str);

    execv(params->wd_path, (char* const*)wd_argv);

    free(wd_argv);
    wd_argv = NULL;
    perror("execv failed in ExecWD()");
    _exit(1);
}

static void ExecClient(wd_params_t* params)
{
    char wd_pid_str[32] = {0};

    assert(NULL != params);

    sprintf(wd_pid_str, "%d", (int)getppid());
    setenv(REVIVED_ENV, "1", 1);
    setenv(WD_PID_ENV, wd_pid_str, 1);
    execv(params->prog_path, params->argv);

    perror("execv failed in ExecClient()");
    _exit(1);
}

static pid_t SpawnTarget(wd_params_t* params)
{
    pid_t pid = 0;
    sigset_t block_set = {0};

    assert(NULL != params);

    sigemptyset(&block_set);
    sigaddset(&block_set, 0 != params->is_client ? SIGUSR1 : SIGUSR2);
    sigprocmask(SIG_BLOCK, &block_set, NULL);

    pid = fork();
    if (0 > pid)
    {
        sigprocmask(SIG_UNBLOCK, &block_set, NULL);

        return NOT_SUCCESS;
    }

    if (0 == pid)
    {
        params->is_client ? ExecWD(params) : ExecClient(params);
    }

    sigprocmask(SIG_UNBLOCK, &block_set, NULL);

    return pid;
}

static int ReviveTarget(wd_params_t* params)
{
    pid_t pid = 0;

    assert(NULL != params);
    DEBUG_PRINT(("%s started reviving %s\n",
                 params->is_client ? "WD thread" : "WD process",
                 params->is_client ? "WD process" : "Client process"));

    *(params->got_signal) = 0;
    if (0 < params->target_pid)
    {
        kill(params->target_pid, SIGKILL);

        if (0 != params->is_target_child)
        {
            waitpid(params->target_pid, NULL, 0);
        }
    }

    pid = SpawnTarget(params);
    if (0 > pid)
    {
        return NOT_SUCCESS;
    }

    if (0 != params->is_client)
    {
        sem_post(params->sem_client);
        while (0 != sem_wait(params->sem_wd))
        {
        }
    }
    else
    {
        sem_post(params->sem_wd);

        while (0 != sem_wait(params->sem_client))
        {
        }

        if (MAX_MISSED_PINGS <= params->missed_counter) /*force yield*/
        {
            kill(pid, SIGSTOP);
            kill(pid, SIGCONT);
        }
    }

    params->missed_counter = 0;
    params->is_target_child = 1;
    params->target_pid = pid;
    DEBUG_PRINT(("%s finished reviving %s\n",
                 params->is_client ? "WD thread" : "WD process",
                 params->is_client ? "WD process" : "Client process"));
    return SUCCESS;
}

static task_status StopRun(wd_params_t* params)
{
    assert(NULL != params);

    *(params->is_running) = 0;
    SchedulerStop(params->scheduler);

    return DO_NOT_REPEAT;
}

task_status CheckAndPingTask(void* param)
{
    wd_params_t* params = (wd_params_t*)param;
    int ping_sig = 0;

    assert(NULL != params);

    ping_sig = 0 != params->is_client ? SIGUSR1 : SIGUSR2;

    if (!*(params->is_running))
    {
        SchedulerStop(params->scheduler);

        return DO_NOT_REPEAT;
    }

    if (*(params->got_signal))
    {
        *(params->got_signal) = 0;
        params->missed_counter = 0;
        DEBUG_PRINT(("%s received ping\n",
                     0 != params->is_client ? "WD thread" : "WD process"));
    }
    else
    {
        ++(params->missed_counter);

        if (MAX_MISSED_PINGS <= params->missed_counter)
        {
            if (SUCCESS != ReviveTarget(params))
            {
                return StopRun(params);
            }
        }
    }

    if (0 < params->target_pid && 0 > kill(params->target_pid, ping_sig))
    {
        if (SUCCESS != ReviveTarget(params) ||
            0 > kill(params->target_pid, ping_sig))
        {
            return StopRun(params);
        }
    }

    return REPEAT;
}

task_status StopCheckTask(void* param)
{
    wd_params_t* params = (wd_params_t*)param;

    assert(NULL != params);

    if (0 == *(params->is_running))
    {
        SchedulerStop(params->scheduler);

        return DO_NOT_REPEAT;
    }

    return REPEAT;
}

static void InitSchedulerAndTasks(wd_params_t* params)
{
    ilrd_uid_t return_val1 = {0};
    ilrd_uid_t return_val2 = {0};

    assert(NULL != params);

    params->scheduler = SchedulerCreate();
    if (NULL == params->scheduler)
    {
        CleanUpGlobals();
        perror("SchedulerCreate() failed in InitSchedulerAndTasks()");

        _exit(1);
    }

    return_val1 = SchedulerAddTask(params->scheduler, WD_INTERVAL,
                                   CheckAndPingTask, DummyCleanup, params);
    return_val2 = SchedulerAddTask(params->scheduler, 1, StopCheckTask,
                                   DummyCleanup, params);
    if (1 == IsILRDUIDEqual(&bad_uid, &return_val1) ||
        1 == IsILRDUIDEqual(&bad_uid, &return_val2))
    {
        perror("bad_UID in InitSchedulerAndTasks()");
        SchedulerDestroy(params->scheduler);
        CleanUpGlobals();

        _exit(1);
    }
}

static watchdog_status_t InitWdParams(const char* prog_path,
                                      const char* watchdog_path, int argc,
                                      char* argv[])
{
    assert(NULL != prog_path);
    assert(NULL != watchdog_path);
    assert(NULL != argv);

    g_client_params = (wd_params_t*)calloc(1, sizeof(wd_params_t));
    if (NULL == g_client_params)
    {

        return WD_THREAD_FAIL;
    }

    g_client_params->wd_path = watchdog_path;
    g_client_params->prog_path = prog_path;
    g_client_params->missed_counter = 0;
    g_running = 1;
    g_client_params->is_running = &g_running;
    g_client_params->argv = argv;
    g_client_params->argc = argc;
    g_client_params->is_client = 1;
    g_got_signal = 1;
    g_client_params->got_signal = &g_got_signal;

    g_client_params->sem_client = sem_open(WD_SEM_CLIENT, O_CREAT, 0666, 0);
    g_client_params->sem_wd = sem_open(WD_SEM_WD, O_CREAT, 0666, 0);
    if (SEM_FAILED == g_client_params->sem_client ||
        SEM_FAILED == g_client_params->sem_wd)
    {
        CleanUpGlobals();

        return WD_THREAD_FAIL;
    }

    return WD_SUCCESS;
}

static watchdog_status_t StartNewWatchdog(void)
{
    assert(NULL != g_client_params);

    if (SUCCESS != ReviveTarget(g_client_params))
    {
        return WD_FORK_FAIL;
    }

    return WD_SUCCESS;
}

static void AttachToExistingWatchdog(const char* wd_pid_env)
{
    assert(NULL != wd_pid_env);
    assert(NULL != g_client_params);

    g_client_params->target_pid = (pid_t)atoi(wd_pid_env);
    g_client_params->is_target_child = 0;

    unsetenv(REVIVED_ENV);
    unsetenv(WD_PID_ENV);

    sem_post(g_client_params->sem_client);

    while (0 != sem_wait(g_client_params->sem_wd))
    {
    }
}

static void* WdThreadFunc(void* arg)
{
    wd_params_t* params = (wd_params_t*)arg;

    assert(NULL != params);

    InitSchedulerAndTasks(params);
    if (0 < params->target_pid)
    {
        kill(params->target_pid, SIGUSR1);
    }

    SchedulerRun(params->scheduler);

    SchedulerDestroy(params->scheduler);
    params->scheduler = NULL;

    return NULL;
}

watchdog_status_t WatchdogControllerStart(const char* prog_path,
                                          const char* watchdog_path, int argc,
                                          char* argv[])
{
    watchdog_status_t status = WD_SUCCESS;
    char* revived_env = NULL;
    char* wd_pid_env = NULL;

    assert(NULL != prog_path);

    status = InitWdParams(prog_path, watchdog_path, argc, argv);
    if (WD_SUCCESS != status)
    {
        return status;
    }

    InitHandlers(&revived_env, &wd_pid_env);
    if (NULL != revived_env && NULL != wd_pid_env)
    {
        AttachToExistingWatchdog(wd_pid_env);
    }
    else
    {
        status = StartNewWatchdog();
        if (WD_SUCCESS != status)
        {
            CleanUpGlobals();

            return status;
        }
    }

    if (SUCCESS != pthread_create(&g_client_params->wd_thread, NULL,
                                  WdThreadFunc, g_client_params))
    {
        CleanUpGlobals();

        return WD_THREAD_FAIL;
    }

    return WD_SUCCESS;
}

void WatchdogControllerEnd(void)
{
    assert(NULL != g_client_params);

    *(g_client_params->is_running) = 0;
    pthread_join(g_client_params->wd_thread, NULL);

    if (0 < g_client_params->target_pid)
    {
        kill(g_client_params->target_pid, SIGTERM);
        waitpid(g_client_params->target_pid, NULL, 0);
        g_client_params->target_pid = 0;
        g_client_params->is_target_child = 0;
    }

    CleanUpSem();
    CleanUpGlobals();
    DEBUG_PRINT(("exit with WatchdogControllerEnd()"));
}
