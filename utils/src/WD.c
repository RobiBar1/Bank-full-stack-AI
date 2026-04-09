/*
Writer: Robi
Chcker: Ehud
Date: 09.04.2026
*/
#define _POSIX_C_SOURCE 200809L

#include <assert.h>    /* assert */
#include <fcntl.h>     /* O_CREAT */
#include <limits.h>    /* CHAR_BIT */
#include <pthread.h>   /* pthread_t */
#include <semaphore.h> /* sem_t, sem_open, sem_post, sem_close */
#include <signal.h>    /* sigaction, kill */
#include <stdio.h>     /* perror */
#include <stdlib.h>    /* atoi */
#include <sys/stat.h>  /* mode constants */
#include <sys/types.h> /* pid_t */
#include <unistd.h>    /* _exit */

#include "scheduler.h" /* scheduler_t, SchedulerRun */
#include "uid.h"       /* ilrd_uid_t */

#define WD_INTERVAL 3
#define WD_SEM_CLIENT "/wd_sem_client"
#define WD_SEM_WD "/wd_sem_wd"

/*========================== use them in struct ======================*/
#define NUM_BITS_USED_IN_STRUCT 2
#define NUM_OF_BYTS_IN_INT 4
#define SIZE_IN_BITS ((CHAR_BIT) * NUM_OF_BYTS_IN_INT - NUM_BITS_USED_IN_STRUCT)

#define UNUSED(x) (void)(x)
#define SUCCESS 0
#define ALLOC_FAIL 1

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

extern task_status CheckAndPingTask(void* param);
extern task_status StopCheckTask(void* param);

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
    sigaddset(&set, SIGTERM);
    sigprocmask(SIG_UNBLOCK, &set, NULL);
}

static void DummyCleanup(void* param) { UNUSED(param); }

static void InitWdProcessParams(wd_params_t* params, int argc, char* argv[])
{
    assert(NULL != params);
    assert(NULL != argv);

    params->target_pid = (pid_t)atoi(argv[1]);
    params->prog_path = argv[2];
    params->argv = &argv[3];
    params->argc = argc - 3;
    params->is_target_child = 0;
    params->missed_counter = 0;
    params->is_client = 0;
    params->got_signal = &g_got_ping;
    params->is_running = &g_running;
    params->wd_path = argv[0];

    params->sem_client = sem_open(WD_SEM_CLIENT, O_CREAT, 0666, 0);
    params->sem_wd = sem_open(WD_SEM_WD, O_CREAT, 0666, 0);

    if (SEM_FAILED == params->sem_client || SEM_FAILED == params->sem_wd)
    {
        perror("wd sem_open failed");

        _exit(1);
    }
}

int main(int argc, char* argv[])
{
    wd_params_t task_param = {0};
    ilrd_uid_t return_val1 = {0};
    ilrd_uid_t return_val2 = {0};

    assert(3 < argc);
    assert(NULL != argv);

    task_param.scheduler = SchedulerCreate();
    if (NULL == task_param.scheduler)
    {
        return ALLOC_FAIL;
    }

    InitWdProcessParams(&task_param, argc, argv);
    InitHandlers();

    return_val1 = SchedulerAddTask(task_param.scheduler, WD_INTERVAL,
                                   CheckAndPingTask, DummyCleanup, &task_param);
    return_val2 = SchedulerAddTask(task_param.scheduler, 1, StopCheckTask,
                                   DummyCleanup, &task_param);

    if (IsILRDUIDEqual(&bad_uid, &return_val1) ||
        IsILRDUIDEqual(&bad_uid, &return_val2))
    {
        perror("bad_UID in WD.c");
        SchedulerDestroy(task_param.scheduler);

        _exit(1);
    }

    sem_post(task_param.sem_wd);
    while (0 != sem_wait(task_param.sem_client))
    {
    }

    SchedulerRun(task_param.scheduler);

    SchedulerDestroy(task_param.scheduler);
    task_param.scheduler = NULL;

    sem_close(task_param.sem_wd);
    task_param.sem_wd = NULL;
    sem_close(task_param.sem_client);
    task_param.sem_client = NULL;

    return SUCCESS;
}