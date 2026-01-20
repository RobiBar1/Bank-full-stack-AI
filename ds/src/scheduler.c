#include "scheduler.h"
#include "pqueue.h"

struct scheduler
{
	task_t* current;
	pqueue_t* pq;
	int flag_is_stoped;
	int need_to_remove_self;
};

static int PriorityCmp(const void* one, const void* two)
{
	assert (NULL != one);
	assert (NULL != two);
	
	return TaskGetReadyTime((const task_t*)one) -
		   TaskGetReadyTime((const task_t*)two);	
}

scheduler_t* SchedulerCreate(void)
{
	scheduler_t* sc = (scheduler_t*)malloc(sizeof(scheduler_t));
	if (NULL == sc)
	{
		return NULL;
	}
	
	sc->pq = PQueueCreate(PriorityCmp);
	if (NULL == sc->pq)
	{
		free(sc); sc = NULL;
		return NULL;
	}
	
	sc->flag_is_stoped = 0;
	sc->need_to_remove_self = 0;
	sc->current = NULL;
	
	return sc;
}

void SchedulerDestroy(scheduler_t* sc)
{
	assert (NULL != scheduler);
	
	PQueueDestroy(sc->pq);
	sc->pq = NULL;
	TaskDestroy(sc->current);
	sc->current = NULL;
	free(sc); sc = NULL;
}

ilrd_uid_t SchedulerAddTask(scheduler_t* sc, size_t time_interval,
		task_func_t task_func, cleanup_func_t cleanup_func, void* param)
{
	task_t* task = NULL;
	
	assert (NULL != sc);
	
	task = TaskCreate(task_func, cleanup_func, time_interval, param);
	if (NULL == task)
	{
		return bad_uid;
	}
	
	if (PQueueEnqueue(sc->pq, task))
	{
		TaskDestroy(task);
		task = NULL;
		
		return bad_uid;
	}
	
	return TaskGetUid(task);
}
