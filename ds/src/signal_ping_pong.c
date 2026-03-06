/*
Writer:  Robi
Checker: Max
Date:    04.03.2026
*/

#define _POSIX_C_SOURCE 199309L

#include <errno.h>     /* errno */
#include <signal.h>    /* sig_atomic_t */
#include <sys/types.h> /* kill         */
#include <unistd.h>    /* write, sleep */

#include "signal_ping_pong.h" /* our api */

#define UNUSED(x) (void)(x)

volatile sig_atomic_t g_child_pid;

static void ChildHandler(int sig)
{
    int save_val = errno;

    sleep(1);
    UNUSED(sig);
    g_child_pid = 1;
    errno = save_val;
}

static void ParentHandler(int sig)
{
    int save_val = errno;

    sleep(1);
    UNUSED(sig);
    g_child_pid = 0;
    errno = save_val;
}

void Active(void)
{
    struct sigaction sa = {0};
    sigset_t mask = {0};
    sigset_t empty = {0};
    ssize_t dummy = 0;
    pid_t child_pid = 0;
    int amount_singlas_sends = 0;

    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    sa.sa_handler = ChildHandler;
    sigaction(SIGUSR1, &sa, NULL);

    sa.sa_handler = ParentHandler;
    sigaction(SIGUSR2, &sa, NULL);

    sigemptyset(&empty);
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    child_pid = fork();
    if (0 < child_pid)
    {
        kill(child_pid, SIGUSR1);
    }

    amount_singlas_sends = 5;
    while (amount_singlas_sends--)
    {
        sigsuspend(&empty);
        if (g_child_pid)
        {
            dummy = write(1, "Ping\n", 5);
            UNUSED(dummy);
            kill(getppid(), SIGUSR2);
        }
        else
        {
            dummy = write(1, "Pong\n", 5);
            UNUSED(dummy);
            kill(child_pid, SIGUSR1);
        }
    }
}