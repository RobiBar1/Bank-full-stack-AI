/*
Writer: Robi
Chcker: ?
Date: ?

gcc -I include/ -shared -fPIC src/WD_lib.c -o libwatchdog.so

gd -I include/ -std=c89 src/WD_client.c

LD_PRELOAD=./libwatchdog.so ./a.out
*/
#include <stdio.h>
#include <stdlib.h>

#include "watchdog_controller.h"

__attribute__((constructor)) static void watchdog_auto_start(void)
{
    WatchdogControllerStart("hey");
}

__attribute__((destructor)) static void watchdog_auto_end(void)
{
    WatchdogControllerEnd();
}

void WatchdogControllerStart(const char* watchdog_exe_path)
{
    printf("%s\n", watchdog_exe_path);
}

void WatchdogControllerEnd(void) { printf("in Watch End\n"); }