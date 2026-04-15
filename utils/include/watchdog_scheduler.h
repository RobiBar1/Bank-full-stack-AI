#ifndef __WATCHDOG_SCHEDULER__
#define __WATCHDOG_SCHEDULER__

#include <pthread.h>
/* IsStopped,heartbeat flag in c file */

/* this the file with all the scheduler logic */

/**
 * @brief creates the scheduler for the watchdog process (for each side)
 *
 * @param[in] pid - the pid of the procces to send signals to
 * @return 0 on success
 */
int RunScheduler(pid_t* pid);

#endif /* __WATCHDOG_SCHEDULER__ */