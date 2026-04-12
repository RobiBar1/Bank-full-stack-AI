/*
Writer: Robi
Chcker: Ehud
Date: 11.04.2026
*/

#define _POSIX_C_SOURCE 200809L

#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "watchdog_controller.h"

/*================================= DEFINES ==================================*/
#define PROG_NAME (2)
#define PID (1)

#define MAX_PID_LEN (16)

#define SEM_PROCCESS "/process_ready"
#define SEM_THREAD "/watchdog_thread_ready"

#define EVN_VARIABLE_NAME "WD_PID"

int main(int argc, char* argv[])
{
    char pid_value[MAX_PID_LEN] = {0};
    pid_t pid = 0;

    assert(NULL != argv);
    assert(3 <= argc);

    snprintf(pid_value, MAX_PID_LEN, "%d", getpid());
    setenv(EVN_VARIABLE_NAME, pid_value, 1);

    pid = (pid_t)strtol(argv[PID], NULL, 10);

    StartWatching(argv[PROG_NAME], pid, argv + PROG_NAME, SEM_THREAD,
                  SEM_PROCCESS);
    return 0;
}