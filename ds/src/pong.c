/*
Writer:  Robi
Checker: Max
Date:    04.03.2026
*/

#define _POSIX_C_SOURCE 199309L

#include <assert.h>    /* assert */
#include <errno.h>     /* errno */
#include <signal.h>    /* sig_atomic_t, sigaction */
#include <sys/types.h> /* kill, getppid, pid_t */
#include <unistd.h>    /* write, sleep */

#define UNUSED(x) (void)(x)

volatile sig_atomic_t g_got_ping = 0;

static void ChildHandler(int sig)
{
    int save_val = errno;
    UNUSED(sig);
    g_got_ping = 1;
    errno = save_val;
}

void Init(sigset_t* empty)
{
    struct sigaction sa = {0};

    assert(NULL != empty);

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = ChildHandler;
    sigaction(SIGUSR1, &sa, NULL);

    sigemptyset(empty);
}

void StartGame(sigset_t* empty)
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
            kill(getppid(), SIGUSR2);
        }
    }
}

int main(void)
{
    sigset_t empty = {0};

    Init(&empty);
    StartGame(&empty);

    return 0;
}