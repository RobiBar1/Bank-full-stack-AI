/*
Writer:  Robi
Checker: Max
Date:    04.03.2026

gcc -ansi -pedantic-errors -Wall -Wextra -g -o terminal_ping terminal_ping.c
gcc -ansi -pedantic-errors -Wall -Wextra -g -o terminal_pong terminal_pong.c
./terminal_ping
*/

#define _POSIX_C_SOURCE 199309L

#include <assert.h>    /* assert       */
#include <errno.h>     /* errno        */
#include <signal.h>    /* sigaction    */
#include <stdio.h>     /* printf       */
#include <sys/types.h> /* pid_t        */
#include <unistd.h>    /* write, sleep */

#define UNUSED(x) (void)(x)

volatile sig_atomic_t g_got_ping = 0;
volatile sig_atomic_t g_pong_pid = 0;

static void PingHandler(int sig, siginfo_t* info, void* context)
{
    int save_val = errno;
    UNUSED(sig);
    UNUSED(context);
    g_got_ping = 1;
    g_pong_pid = info->si_pid;
    errno = save_val;
}

static void Init(sigset_t* empty)
{
    struct sigaction sa = {0};
    sigset_t mask = {0};

    assert(NULL != empty);

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = PingHandler;
    sigaction(SIGUSR1, &sa, NULL);

    sigemptyset(empty);
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    sigprocmask(SIG_BLOCK, &mask, NULL);
}

static void StartGame(sigset_t* empty)
{
    ssize_t dummy = 0;
    int amount_signals_sends = 50;

    assert(NULL != empty);

    while (0 < amount_signals_sends)
    {
        sigsuspend(empty);
        if (g_got_ping)
        {
            g_got_ping = 0;
            --amount_signals_sends;
            dummy = write(1, "Ping\n", 5);
            UNUSED(dummy);

            sleep(1);
            kill((pid_t)g_pong_pid, SIGUSR2);
        }
    }
}

int main(void)
{
    sigset_t empty = {0};

    Init(&empty);
    printf("ping PID: %d\nwaiting for pong now\n", getpid());
    StartGame(&empty);

    return 0;
}
