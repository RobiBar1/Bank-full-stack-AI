/*
Writer:  Robi
Checker: Max
Date:    04.03.2026
*/

#define _POSIX_C_SOURCE 199309L

#include <assert.h>    /* assert       */
#include <errno.h>     /* errno        */
#include <signal.h>    /* sigaction    */
#include <stdlib.h>    /* atoi         */
#include <sys/types.h> /* pid_t        */
#include <unistd.h>    /* write, sleep */

#define UNUSED(x) (void)(x)

volatile sig_atomic_t g_got_pong = 0;

static void PongHandler(int sig)
{
    int save_val = errno;
    UNUSED(sig);
    g_got_pong = 1;
    errno = save_val;
}

static void Init(sigset_t* empty)
{
    struct sigaction sa = {0};
    sigset_t mask = {0};

    assert(NULL != empty);

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = PongHandler;
    sigaction(SIGUSR2, &sa, NULL);

    sigemptyset(empty);
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    sigprocmask(SIG_BLOCK, &mask, NULL);
}

static void StartGame(sigset_t* empty, pid_t ping_pid)
{
    ssize_t dummy = 0;
    int amount_signals_sends = 50;

    assert(NULL != empty);

    kill(ping_pid, SIGUSR1);
    while (0 < amount_signals_sends)
    {
        sigsuspend(empty);
        if (g_got_pong)
        {
            g_got_pong = 0;
            --amount_signals_sends;
            dummy = write(1, "Pong\n", 5);
            UNUSED(dummy);

            sleep(1);
            kill(ping_pid, SIGUSR1);
        }
    }
}

int main(int argc, char* argv[])
{
    sigset_t empty = {0};
    pid_t ping_pid = 0;

    assert(2 == argc);
    UNUSED(argc);

    ping_pid = (pid_t)(atoi(argv[1]));

    Init(&empty);
    StartGame(&empty, ping_pid);

    return 0;
}
