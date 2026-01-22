#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <stddef.h>     /* size_t */

#include "uid.h"        /* ilrd_uid_t */

/* return status of task_func indecationg for the scheduler how it should proceed */
typedef enum  {
    FAILURE,
    REPEAT,
    DO_NOT_REPEAT       /* DO_NOT_REPEAT frees the task */
} task_status;

/**
 * @desc function does task
 * @return: weather the func should repeat again
 * TODO should recieve void *param
 */
typedef task_status (*task_func_t) (void* param);

typedef void (*cleanup_func_t) (void* param); 

typedef struct scheduler scheduler_t;

/**
 * @desc creates a new scheduler and returns its address
 * @return the address of the new scheduler or NULL if fails
 * @complexity: O(1)
 */
scheduler_t* SchedulerCreate(void);

/**
 * @desc destroy the task
 * @param[in] scheduler - the scheduler we are going to destroy
 * @pre scheduler is not NULL?
 * @complexity: O(n)
 * returns nothing
 */
void SchedulerDestroy(scheduler_t* scheduler);


/**
 * @desc creates a new task and returns its uid
 * @return the ilrd_uid
 * @param[in] scheduler
 * @param[in] time_interval in seconds. helps determine the priority in accordance with the time at addition.
 * @param[in] task_func - the task function 
 * @param[in] cleanup_func 
 * @param[in] param - the parameter for the task functions - cleanup and task
 * @pre task_func != NULL
 * @pre cleanup_func != NULL
 * @complexity: O(1)
 */
ilrd_uid_t SchedulerAddTask(scheduler_t* scheduler, size_t time_interval, task_func_t task_func, 
                            cleanup_func_t cleanup_func, const void* param);

/**
 * @desc removes task from scheduler 
 * @param[in] scheduler
 * @param[in] uid - find the task to be remove by uid
 * @pre scheduler != NULL
 * @pre uid != bad_uid
 * @complexity: O(N)
 */
void SchedulerRemoveTask(scheduler_t* scheduler, ilrd_uid_t uid);


/**
 * @desc runs all the task 
 * @return int - 0 on success, 1 on failure
 * @param[in] scheduler
 * @pre scheduler != NULL
 * @complexity: O(n)
 */
int SchedulerRun(scheduler_t* scheduler);

/**
 * @desc stops the schedulers run
 * @param[in] running scheduler
 * @pre scheduler != NULL
 * @return void
*/
void SchedulerStop(scheduler_t* scheduler);

/**
 * @desc counts the number of tasks in the scheduler
 * @return size_t - number of tasks in the scheduler
 * @param[in] scheduler  
 * @pre scheduler != NULL
 * @complexity: O(n)
 */
size_t SchedulerCount(const scheduler_t* scheduler);

/**
 * @desc if the scheduler is empty, returns 1 else 0
 * @param[in] scheduler - the scheduler we want to check if empty
 * @pre 'scheduler' != NULL
 * @return if the scheduler is empty with no current task running.
 * @complexity: O(1) 
 **/
int SchedulerIsEmpty(const scheduler_t* scheduler);



#endif /* __SCHEDULER_H__ */
    
