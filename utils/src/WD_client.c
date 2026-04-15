/*
Writer: Robi
Chcker: Ehud
Date: 11.04.2026

terminal:
1. libwatchdog.so:

gcc -g -O0 -I include/ -I ../ds/include ../ds/src/scheduler.c \
../ds/src/pqueue.c ../ds/src/heap.c ../ds/src/vector.c ../ds/src/uid.c \
../ds/src/task.c -shared -fPIC src/WD_lib.c -lpthread -o libwatchdog.so

------------------------------------------------------->>>
2. wd.c:

gcc -g -O0 -std=c89 -I include/ -I ../ds/include src/WD.c \
../ds/src/scheduler.c ../ds/src/uid.c ../ds/src/task.c ../ds/src/pqueue.c \
../ds/src/heap.c ../ds/src/vector.c -L. -lwatchdog -lpthread -o wd

IMPORTENT NOTE AFTER COMPILE WD.C IF CHANGE THE NAME FROM WD:
if change the name of file from "wd"
you most change WD_EXE_PATH in this file("WD_client.c") either!
------------------------------------------------------->>>
3. WD_client.c

gcc -g -O0 -I include/ -std=c89 src/WD_client.c -L. -lwatchdog -lpthread

4. run:

LD_LIBRARY_PATH=. ./a.out
*/

#define _POSIX_C_SOURCE 200809L

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include "watchdog_controller.h" /* WatchdogStart, WatchdogEnd */

#define UNUSED(x) ((void)(x))

#define PATH_MAX 256
#define WD_EXE_PATH "./wd"

int main(int argc, char* argv[])
{
    unsigned int remaining = 50;
    char path[PATH_MAX] = {'\0'};
    ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);

    assert(NULL != argv);

    if (-1 != len)
    {
        printf("Full path to executable: %s\n", path);
        argv[0] = path;
    }
    else
    {
        printf("readlink() error\n");
    }

    WatchdogControllerStart(WD_EXE_PATH, argv);

    while (0 < remaining)
    {
        remaining = sleep(remaining);
    }

    WatchdogControllerEnd();

    return 0;
}
