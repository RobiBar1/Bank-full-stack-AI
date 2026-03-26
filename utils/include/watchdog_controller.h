#ifndef __WATCHDOG_CONTROLLER__
#define __WATCHDOG_CONTROLLER__

#include <sys/types.h> /* pid_t */

void WatchdogControllerStart(const char* watchdog_exe_path);
void WatchdogControllerEnd(void);

#endif /* __WATCHDOG_CONTROLLER__ */