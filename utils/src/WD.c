#define _POSIX_C_SOURCE 200809L

#include <assert.h>    /* assert */
#include <limits.h>    /* CHAR_BIT */
#include <signal.h>    /* sigaction, kill */
#include <stdio.h>     /* printf, perror */
#include <stdlib.h>    /* atoi, setenv */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* waitpid, WNOHANG */
#include <unistd.h>    /* fork, getpid */

#include "scheduler.h" /* scheduler_t, SchedulerRun */
#include "uid.h"       /* pid_t */

#define WD_INTERVAL 3
#define MAX_MISSED_PINGS 3

#define REVIVED_ENV "WD_REVIVED"
#define WD_PID_ENV "WD_EXISTING_PID"

#define SIZE_IN_BITS ((CHAR_BIT) * 4 - 1)

#define UNUSED(x) (void)(x)
#define SUCCESS 0
#define ALLOC_FAIL 1
#define FORK_FAIL 2

typedef struct task_params
{
    scheduler_t* scheduler;
    const char* client_path;
    char** client_argv;
    pid_t client_pid;
    unsigned int missed_counter : SIZE_IN_BITS;
    unsigned int client_is_child : 1;
} task_params_t;

static volatile sig_atomic_t g_got_ping = 1;
static volatile sig_atomic_t g_running = 1;

static void Sigusr1Handler(int sig)
{
    UNUSED(sig);
    g_got_ping = 1;
}

static void SigtermHandler(int sig)
{
    UNUSED(sig);
    g_running = 0;
}

static void InitHandlers(void)
{
    struct sigaction sa = {0};
    sigset_t set = {0};

    sa.sa_handler = Sigusr1Handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);

    sa.sa_handler = SigtermHandler;
    sigaction(SIGTERM, &sa, NULL);

    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigprocmask(SIG_UNBLOCK, &set, NULL);
}

static void DummyCleanup(void* param) { UNUSED(param); }

static int ReviveClient(task_params_t* params)
{
    char wd_pid_str[32] = {0};
    sigset_t block_set = {0};
    pid_t pid = 0;

    assert(NULL != params);

    if (0 < params->client_pid)
    {
        kill(params->client_pid, SIGKILL);

        if (0 != params->client_is_child)
        {
            waitpid(params->client_pid, NULL, 0);
        }
    }

    sigemptyset(&block_set);
    sigaddset(&block_set, SIGUSR2);
    sigprocmask(SIG_BLOCK, &block_set, NULL);

    pid = fork();
    if (0 > pid)
    {
        perror("fork");
        sigprocmask(SIG_UNBLOCK, &block_set, NULL);

        return FORK_FAIL;
    }

    if (0 == pid)
    {
        sprintf(wd_pid_str, "%d", (int)getppid());
        setenv(REVIVED_ENV, "1", 1);
        setenv(WD_PID_ENV, wd_pid_str, 1);
#ifndef NDEBUG
        printf("process WD debug: trying to execv: %s\n", params->client_path);
#endif
        execv(params->client_path, params->client_argv);
        perror("process WD debug: execv failed");

        _exit(1);
    }

    sigprocmask(SIG_UNBLOCK, &block_set, NULL);
    params->client_is_child = 1;
    params->missed_counter = 0;
    params->client_pid = pid;
    g_got_ping = 0;
#ifndef NDEBUG
    printf("process WD: revived client pid=%d\n", (int)params->client_pid);
#endif

    return SUCCESS;
}

static task_status StopRun(scheduler_t* scheduler)
{
    g_running = 0;
    SchedulerStop(scheduler);

    return DO_NOT_REPEAT;
}

static task_status WdHeartbeatTask(void* param)
{
    task_params_t* params = (task_params_t*)param;

    assert(NULL != params);

    if (!g_running)
    {
        return StopRun(params->scheduler);
    }

    if (g_got_ping)
    {
#ifndef NDEBUG
        printf("process WD: client responding\n");
#endif
        g_got_ping = 0;
        params->missed_counter = 0;
    }
    else
    {
#ifndef NDEBUG
        printf("process WD: client not ping\n");
#endif
        ++params->missed_counter;
        if (MAX_MISSED_PINGS <= params->missed_counter)
        {
            if (SUCCESS != ReviveClient(params))
            {
                return StopRun(params->scheduler);
            }
        }
    }

    if (0 > kill(params->client_pid, SIGUSR2))
    {
        if (SUCCESS != ReviveClient(params) ||
            0 > kill(params->client_pid, SIGUSR2))
        {
            return StopRun(params->scheduler);
        }
    }

    return REPEAT;
}

static task_status WdStopCheckTask(void* param)
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

int main(int argc, char* argv[])
{
    task_params_t task_param = {0};
    ilrd_uid_t return_val1 = {0};
    ilrd_uid_t return_val2 = {0};

    assert(3 < argc);
    assert(NULL != argv);

    task_param.scheduler = SchedulerCreate();
    if (NULL == task_param.scheduler)
    {
        return ALLOC_FAIL;
    }

    task_param.client_pid = (pid_t)atoi(argv[1]);
    task_param.client_path = argv[2];
    task_param.client_argv = &argv[3];
    task_param.client_is_child = 0;
    task_param.missed_counter = 0;

#ifndef NDEBUG
    printf("wd pid=%d start, watch client pid=%d\n", (int)getpid(),
           (int)task_param.client_pid);
#endif
    InitHandlers();

    return_val1 = SchedulerAddTask(task_param.scheduler, WD_INTERVAL,
                                   WdHeartbeatTask, DummyCleanup, &task_param);
    return_val2 = SchedulerAddTask(task_param.scheduler, 1, WdStopCheckTask,
                                   DummyCleanup, task_param.scheduler);

    if (0 != IsILRDUIDEqual(&bad_uid, &return_val1) ||
        0 != IsILRDUIDEqual(&bad_uid, &return_val2))
    {
        perror("bad_UID in WD.c");

        _exit(1);
    }

    g_got_ping = 0;
    kill(task_param.client_pid, SIGUSR2);
    SchedulerRun(task_param.scheduler);

    SchedulerDestroy(task_param.scheduler);
    task_param.scheduler = NULL;
#ifndef NDEBUG
    printf("wd proc exiting\n");
#endif

    return SUCCESS;
}