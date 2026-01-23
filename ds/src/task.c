/*
Writer:  Robi
Checker: Shahar
Date: 	 20.01.2026
*/
#include <time.h> /* time_t */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */

#include "task.h"
#include "scheduler.h"

struct task
{
	void* param;
	size_t time_interval_sec;
	task_func_t task_func;
	cleanup_func_t task_cleanup;
	time_t time_ready;
	ilrd_uid_t uid;
};

task_t* TaskCreate(task_func_t task_func, cleanup_func_t cleanup_func, size_t time_interval, const void* param)
{
	task_t* task = NULL;
	
	assert (NULL != task_func);
	assert (NULL != cleanup_func);
	
	task = (task_t*)malloc(sizeof(task_t));
	if (NULL == task)
	{
		return NULL;
	}
	
	task->uid = ILRDUIDCreate();
	if (IsILRDUIDEqual(&(task->uid), &bad_uid))
	{
		free(task); task = NULL;
		return NULL;
	}
	
	task->time_interval_sec = time_interval;
	task->task_func = task_func;
	task->task_cleanup = cleanup_func;
	time(&task->time_ready);
	task->time_ready += time_interval;
	task->param = (void*)param;
	
	return task;
}

void TaskDestroy(task_t* task)
{
	assert (NULL != task);
	
	free(task); task = NULL;
}

task_status TaskDoFunc(const task_t* task)
{
	assert (NULL != task);
	
	return task->task_func(task->param);
}

void TaskDoCleanupFunc(const task_t* task)
{
	assert (NULL != task); 
	
	task->task_cleanup(task->param);
}

ilrd_uid_t TaskGetUid(const task_t* task)
{
	assert (NULL != task); 
	
	return task->uid;
}

size_t TaskGetTimeInterval(const task_t* task)
{
	assert (NULL != task); 
	
	return task->time_interval_sec;
}

time_t TaskGetReadyTime(const task_t* task)
{
	assert (NULL != task); 
	
	return task->time_ready;
}

void TaskUpdateReadyTime(task_t* task)
{
	task->time_ready = TaskGetTimeInterval(task) + time(NULL);
}

