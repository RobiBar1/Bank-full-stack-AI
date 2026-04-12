#ifndef __WATCHDOG_CONTROLLER_H__
#define __WATCHDOG_CONTROLLER_H__

#include <sys/types.h> /* pid_t */

typedef enum watchdog_status
{
    WD_SUCCESS,
    WD_FORK_FAIL,
    WD_THREAD_FAIL
} watchdog_status_t;

/*
 * @Description:
 * this function will create the system that will guard your proccess and will
 * auto restart him if he will crash without active WatchdogControllerEnd() that
 * mark the end of your guard area in code.
 * @pre:
 * watchdog_path != NUll
 * argv != NULL
 * @if use you MOST:
 * call that funcion before create any thread.
 * you wont be allowed to use SIGUSR1, SIGUSR2 or overide there signal handlers.
 * @return:
 * WD_SUCCESS if the system is up.
 */

watchdog_status_t WatchdogControllerStart(const char* watchdog_path,
                                          char* argv[]);
void WatchdogControllerEnd(void);

#endif /* __WATCHDOG_CONTROLLER_H__ */