/*
Writer: Robi
Chcker: Ehud
Date: ?

terminal:
gcc -g -O0 -I include/ -I ../ds/include ../ds/src/scheduler.c ../ds/src/pqueue.c
\
../ds/src/heap.c ../ds/src/vector.c ../ds/src/uid.c ../ds/src/task.c \
-shared -fPIC src/WD_lib.c -lpthread -o libwatchdog.so

------------------------------------------------------->>>
gcc -g -O0 -std=c89 -I ../ds/include src/WD.c ../ds/src/scheduler.c
../ds/src/uid.c \
../ds/src/task.c ../ds/src/pqueue.c ../ds/src/heap.c ../ds/src/vector.c -o wd


if change the name of file from "wd"
go change the define WD_EXE_PATH in "WD_client.c" file either!
------------------------------------------------------->>>
gcc -g -O0 -I include/ -std=c89 src/WD_client.c -L. -lwatchdog -lpthread

LD_LIBRARY_PATH=. ./a.out
*/
#define _POSIX_C_SOURCE 200809L

#include <assert.h>    /* assert */
#include <pthread.h>   /* pthread_create, pthread_join */
#include <semaphore.h> /* sem_t */
#include <signal.h>    /* sigaction, kill */
#include <stdio.h>     /* printf, perror */
#include <stdlib.h>    /* unsetenv, getenv */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* waitpid, WNOHANG */
#include <unistd.h>    /* fork, execl */

#include "scheduler.h"           /* scheduler_t, SchedulerRun */
#include "watchdog_controller.h" /* watchdog_status_t */

#define UNUSED(x) (void)(x)
#define SUCCESS 0
#define NOT_SUCCESS (-1)

#define WD_INTERVAL 3
#define MAX_MISSED_PINGS 3

#define STREAM_CH_WRITE_TO 1

#define REVIVED_ENV "WD_REVIVED"
#define WD_PID_ENV "WD_EXISTING_PID"

static volatile sig_atomic_t g_got_signal = 1;
static volatile sig_atomic_t g_running = 1;
static const char* g_watchdog_path = NULL;
static const char* g_prog_path = NULL;
static pthread_t g_wd_thread = 0;
static int g_missed_counter = 0;
static int g_wd_is_child = 0;
static char** g_argv = NULL;
static pid_t g_wd_pid = 0;
static int g_argc = 0;

static void Sigusr2Handler(int sig)
{
    UNUSED(sig);
    g_got_signal = 1;
}

static void DummyCleanup(void* param) { UNUSED(param); }

static void InitHandlers(void)
{
    struct sigaction sa = {0};
    sigset_t set = {0};

    sigemptyset(&sa.sa_mask);
    sa.sa_handler = Sigusr2Handler;
    sigaction(SIGUSR2, &sa, NULL);

    sigemptyset(&set);
    sigaddset(&set, SIGUSR2);
    sigprocmask(SIG_UNBLOCK, &set, NULL);
}

static pid_t SpawnWatchdog(void)
{
    char client_pid_str[32] = {0};
    sigset_t block_set = {0};
    char** wd_argv = NULL;
    pid_t pid = 0;
    int i = 0;

    sprintf(client_pid_str, "%d", (int)getpid());

    sigemptyset(&block_set);
    sigaddset(&block_set, SIGUSR1);
    sigprocmask(SIG_BLOCK, &block_set, NULL);

    wd_argv = (char**)malloc(sizeof(char*) * (g_argc + 4));
    if (NULL == wd_argv)
    {
        sigprocmask(SIG_UNBLOCK, &block_set, NULL);

        return NOT_SUCCESS;
    }

    wd_argv[0] = (char*)g_watchdog_path;
    wd_argv[1] = client_pid_str;
    wd_argv[2] = (char*)g_prog_path;
    for (; i < g_argc; ++i)
    {
        wd_argv[3 + i] = g_argv[i];
    }

    wd_argv[3 + g_argc] = NULL;
    pid = fork();
    if (0 > pid)
    {
        sigprocmask(SIG_UNBLOCK, &block_set, NULL);
        free(wd_argv);
        wd_argv = NULL;

        return NOT_SUCCESS;
    }

    if (SUCCESS == pid)
    {
        execv(g_watchdog_path, wd_argv);
        free(wd_argv);
        wd_argv = NULL;

        _exit(1);
    }

    sigprocmask(SIG_UNBLOCK, &block_set, NULL);
    free(wd_argv);
    wd_argv = NULL;

    return pid;
}

static int ReviveWatchdog(void)
{
    pid_t pid = 0;

    if (0 < g_wd_pid)
    {
        kill(g_wd_pid, SIGKILL);

        if (0 != g_wd_is_child)
        {
            waitpid(g_wd_pid, NULL, 0);
        }
    }

    pid = SpawnWatchdog();
    if (0 > pid)
    {
        perror("fork");

        return WD_FORK_FAIL;
    }

    g_missed_counter = 0;
    g_wd_is_child = 1;
    g_got_signal = 0;
    g_wd_pid = pid;

#ifndef NDEBUG
    printf("client pid=%d revived watchdog pid=%d\n", (int)getpid(),
           (int)g_wd_pid);
#endif

    return WD_SUCCESS;
}

static task_status StopRun(scheduler_t* scheduler)
{
    g_running = 0;
    SchedulerStop(scheduler);

    return DO_NOT_REPEAT;
}

static task_status ClientCheckAndPingTask(void* param)
{
    scheduler_t* scheduler = (scheduler_t*)param;

    assert(NULL != scheduler);

    if (!g_running)
    {
        SchedulerStop(scheduler);

        return DO_NOT_REPEAT;
    }

    if (g_got_signal)
    {
        write(STREAM_CH_WRITE_TO, "wd_thread heartbeat from wd OK\n", 31);
        g_got_signal = 0;
        g_missed_counter = 0;
    }
    else
    {
        write(STREAM_CH_WRITE_TO, "wd_thread watchdog not responding\n", 34);
        ++g_missed_counter;

        if (MAX_MISSED_PINGS <= g_missed_counter)
        {
            if (WD_SUCCESS != ReviveWatchdog())
            {
                return StopRun(scheduler);
            }
        }
    }

    if (0 < g_wd_pid && 0 > kill(g_wd_pid, SIGUSR1))
    {
        if (WD_SUCCESS != ReviveWatchdog())
        {
            return StopRun(scheduler);
        }

        kill(g_wd_pid, SIGUSR1);
    }

    return REPEAT;
}

static task_status ClientStopCheckTask(void* param)
{
    scheduler_t* scheduler = (scheduler_t*)param;

    assert(NULL != scheduler);

    if (!g_running)
    {
        SchedulerStop(scheduler);

        return DO_NOT_REPEAT;
    }

    return REPEAT;
}

static void* WdThreadFunc(void* arg)
{
    scheduler_t* scheduler = NULL;
    ilrd_uid_t return_val1 = {0};
    ilrd_uid_t return_val2 = {0};
    UNUSED(arg);

    scheduler = SchedulerCreate();
    if (NULL == scheduler)
    {
        return NULL;
    }

    return_val1 =
        SchedulerAddTask(scheduler, WD_INTERVAL, ClientCheckAndPingTask,
                         DummyCleanup, scheduler);
    return_val2 = SchedulerAddTask(scheduler, 1, ClientStopCheckTask,
                                   DummyCleanup, scheduler);

    if (SUCCESS != IsILRDUIDEqual(&bad_uid, &return_val1) ||
        SUCCESS != IsILRDUIDEqual(&bad_uid, &return_val2))
    {
        perror("bad_UID in WD_lib.c");

        _exit(1);
    }

    g_got_signal = 0;
    if (0 < g_wd_pid)
    {
        kill(g_wd_pid, SIGUSR1);
    }

    SchedulerRun(scheduler);

    SchedulerDestroy(scheduler);
    scheduler = NULL;

    return NULL;
}

watchdog_status_t WatchdogControllerStart(const char* prog_path,
                                          const char* watchdog_path, int argc,
                                          char* argv[])
{
    char* revived_env = NULL;
    char* wd_pid_env = NULL;
    pid_t pid = 0;

    assert(NULL != prog_path);
    assert(NULL != watchdog_path);
    assert(NULL != argv);

    g_watchdog_path = watchdog_path;
    g_prog_path = prog_path;
    g_missed_counter = 0;
    g_got_signal = 0;
    g_running = 1;
    g_argv = argv;
    g_argc = argc;

#ifndef NDEBUG
    printf("client pid=%d starting watchdog system\n", (int)getpid());
#endif

    InitHandlers();

    revived_env = getenv(REVIVED_ENV);
    wd_pid_env = getenv(WD_PID_ENV);
    if (NULL != revived_env && NULL != wd_pid_env)
    {
        g_wd_pid = (pid_t)atoi(wd_pid_env);
        g_wd_is_child = 0;

        unsetenv(REVIVED_ENV);
        unsetenv(WD_PID_ENV);

#ifndef NDEBUG
        printf("client pid=%d attached to existing watchdog pid=%d\n",
               (int)getpid(), (int)g_wd_pid);
#endif
    }
    else
    {
        pid = SpawnWatchdog();
        if (0 > pid)
        {
            perror("fork");

            return WD_FORK_FAIL;
        }

        g_wd_pid = pid;
        g_wd_is_child = 1;

#ifndef NDEBUG
        printf("client pid=%d spawned watchdog pid=%d\n", (int)getpid(),
               (int)g_wd_pid);
#endif
    }

    if (SUCCESS != pthread_create(&g_wd_thread, NULL, WdThreadFunc, NULL))
    {
        return WD_THREAD_FAIL;
    }

    return WD_SUCCESS;
}

void WatchdogControllerEnd(void)
{
    g_running = 0;

    pthread_join(g_wd_thread, NULL);

    if (0 < g_wd_pid)
    {
        kill(g_wd_pid, SIGTERM);
        if (!g_wd_is_child)
        {
            waitpid(g_wd_pid, NULL, 0);
        }

        g_wd_pid = 0;
        g_wd_is_child = 0;
    }

#ifndef NDEBUG
    printf("client watchdog stopped\n");
#endif
}