/*
Writer:  Robi
Checker: Max
Date:    04.03.2026

run on terminal:
gcc -ansi -pedantic-errors -Wall -Wextra -g -o ping ping.c
gcc -ansi -pedantic-errors -Wall -Wextra -g -o pong pong.c
./ping.c
*/

#define _POSIX_C_SOURCE 199309L

#include <assert.h>    /* assert */
#include <errno.h>     /* errno */
#include <signal.h>    /* sig_atomic_t, sigaction */
#include <sys/types.h> /* kill, pid_t */
#include <unistd.h>    /* write, sleep, fork, execv */

#define UNUSED(x) (void)(x)

volatile sig_atomic_t g_got_pong = 0;

static void ParentHandler(int sig)
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

    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = ParentHandler;
    sigaction(SIGUSR2, &sa, NULL);

    sigemptyset(empty);
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    sigprocmask(SIG_BLOCK, &mask, NULL);
}

static void StartGame(sigset_t* empty, pid_t child_pid)
{
    ssize_t dummy = 0;
    int amount_signals_sends = 50;

    assert(NULL != empty);

    kill(child_pid, SIGUSR1);
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
            kill(child_pid, SIGUSR1);
        }
    }
}

int main(void)
{
    pid_t child_pid = 0;
    sigset_t empty = {0};
    char* args[] = {"./pong", NULL};

    Init(&empty);

    child_pid = fork();
    if (child_pid == 0)
    {
        execv(args[0], args);
        _exit(1); /*when exit from main with bad status i dont want use return
    cos it will flush the parent (lets say shall here) std i/o buffers(like
    printf buffers)*/
    }

    StartGame(&empty, child_pid);

    return 0;
}
