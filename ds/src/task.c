#include "task.h"

#include <time.h>

struct task
{
	ilrd_uid_t vid;
	time_t time_ready;
	size_t time_interval_sec;
	task_fanc_t task_func;
	task_cleanup_t task_cleanup;
	void* param;
};

task_t* TaskCreate(task_func_t task_func, cleanup_func_t cleanup_func, size_t time_interval, const void* param)
{
	task_t* task = NULL;
	
	assert (NULL != task_func);
	assert (NULL != cleanup_func);
	
	task = (task_t*)malloc(sizeof(task_t));
	if (NULL != task)
	{
		return NULL;
	}
	
	task->vid = ILRDUIDCreate();
	if (IsILRDUIDEqual(task->vid, bad_uid))
	{
		free(task); task = NULL;
		return NULL;
	}
	
	task->time_interval_sec = time_interval;
	task->task_func = task_func;
	task->task_func = cleanup_func;
	task->param = param;
	time(&task->time_ready);
	task->time_ready += time_interval;
}

void TaskDestroy(task_t* task)
{
	assert (NULL != task);
	
	free(task); task = NULL;
}


