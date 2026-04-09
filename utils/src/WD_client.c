/*
Writer: Robi
Chcker: Ehud
Date: 09.04.2026
*/
#define _POSIX_C_SOURCE 200809L

#include <assert.h> /* assert */
#include <limits.h> /* PATH_MAX */
#include <stdio.h>  /* printf */
#include <unistd.h> /* sleep */

#include "watchdog_controller.h" /* my_api */

#define WD_EXE_PATH "./wd"

#define UNUSED(x) (void)(x)

int main(int argc, char* argv[])
{
    unsigned int remaining = 50;
    char path[PATH_MAX] = {'\0'};
    ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);

    assert(NULL != argv);

    -1 != len ? printf("Full path to executable: %s\n", path)
              : printf("readlink() error");

    WatchdogControllerStart(path, WD_EXE_PATH, argc, argv);
    while (0 < remaining)
    {
        remaining = sleep(remaining);
    }

    WatchdogControllerEnd();

    return 0;
}