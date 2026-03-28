#ifndef __WATCHDOG_CONTROLLER__H
#define __WATCHDOG_CONTROLLER__H

#include <sys/types.h> /* pid_t */

void WatchdogControllerStart(const char* watchdog_exe_path);
void WatchdogControllerEnd(void);

#endif /* __WATCHDOG_CONTROLLER__ */