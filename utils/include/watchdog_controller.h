#ifndef __WATCHDOG_CONTROLLER_H__
#define __WATCHDOG_CONTROLLER_H__

#include <sys/types.h> /* pid_t */

typedef enum watchdog_status
{
    WD_SUCCESS,
    WD_FORK_FAIL,
    WD_THREAD_FAIL
} watchdog_status_t;

watchdog_status_t WatchdogControllerStart(const char* prog_path,
                                          const char* watchdog_path, int argc,
                                          char* argv[]);
void WatchdogControllerEnd(void);

#endif /* __WATCHDOG_CONTROLLER_H__ */