#ifndef __TASK_H__
#define __TASK_H__

#include <stddef.h>     /* size_t */
#include <time.h>       /* time_t */
    
#include "scheduler.h"  /* cleanup_func, task_func */
#include "uid.h"        /* ilrd_uid_t */

typedef struct task task_t;

/**
 * @desc creates a new task and returns its address
 * @return the address of the new task or NULL if fails
 * @param[in] task_func 
 * @param[in] time_interval ( curr time + time interval =  ready_time(priority)) 
 * @param[in] cleanup_func
 * @pre task_func != NULL
 * @pre cleanup_func != NULL
 * @complexity: O(1)
 */
task_t* TaskCreate(task_func_t task_func, cleanup_func_t cleanup_func, size_t time_interval);


/**
 * @desc destroy the task
 * @param[in] task - the task we are going to destroy
 * @pre task is not NULL
 * @complexity: O(1)
 */
void TaskDestroy(task_t* task);

/**
 * @desc Does the task's function 
 * @param[in] task - the task whose function we're going to do
 * @pre task is not NULL
 * @pre task's function is not NULL
 * @complexity: O(1)
 */
task_status TaskDoFunc(const task_t* task);

/**
 * @desc Does the task's function 
 * @param[in] task - the task whose cleanup we're going to do
 * @pre task is not NULL
 * @pre task's function is not NULL
 * @complexity: O(1)
 */
void TaskDoCleanupFunc(const task_t* task, void* param);

/**
 * @desc returns the UID of the task
 * @param[in] task - the task whose UID we want
 * @pre task is not NULL
 * @return the tasks UID
 * @complexity: O(1)
 */
ilrd_uid_t TaskGetUid(const task_t* task);

/**
 * @desc returns the time interval (priority) of a task
 * @param[in] task - the task which we want the time interval of
 * @pre 'task' != NULL
 * @return the task's priority
 * @complexity: O(1)
 **/
size_t TaskGetTimeInterval(const task_t* task);

/**
 * @desc get the ready time (priority)
 * @param[in] task - the task we need to find out the ready time
 * @pre task is not NULL
 * @complexity: O(1)
 */
time_t TaskGetReadyTime(const task_t* task);

/**
 * @desc update the ready time for task recycling
 * @param[in] task - the task we need to update the ready time
 * @pre task is not NULL
 * @complexity: O(1)
 */
void TaskUpdateReadyTime(task_t* task);

#endif /* __TASK_H__ */
