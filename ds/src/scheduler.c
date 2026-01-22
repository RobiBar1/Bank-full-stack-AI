/*
Writer:  Robi
Checker: Shahar
Date: 	 21.01.2026
*/

#include <time.h> /* time */
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "scheduler.h" /* our api */
#include "pqueue.h"    /* pqueue_t */
#include "task.h"  	   /* task_t */


#define UNUSED(x) void(x)
#define SECCESS (0)
struct scheduler
{
	task_t* current;
	pqueue_t* pq;
	int flag_is_stoped;
	int need_to_remove_self;
};

static int cmp_func(const void* one, const void* two)
{
	ilrd_uid_t uid_one = TaskGetUid((const task_t*)one);
	ilrd_uid_t uid_two = TaskGetUid((const task_t*)two);
	
	 return !(IsILRDUIDEqual((const ilrd_uid_t*)(&uid_one),
	 						 (const ilrd_uid_t*)(&uid_two)));		
}

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
	assert (NULL != sc);
	
	while (!PQueueIsEmpty(sc->pq))
    {
        TaskDestroy(PQueueDequeue(sc->pq));
    }

	PQueueDestroy(sc->pq);
	sc->pq = NULL;
	sc->current = NULL;
	free(sc); sc = NULL;
}
                            
ilrd_uid_t SchedulerAddTask(scheduler_t* sc, size_t time_interval,
		task_func_t task_func, cleanup_func_t cleanup_func,const void* param)
{
	task_t* task = NULL;
	ilrd_uid_t uid = {0};
	
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
	
	uid = TaskGetUid(task);

	
	return uid;
}

void SchedulerRemoveTask(scheduler_t* sc, ilrd_uid_t uid)
{
	assert (NULL != sc);
	assert (!IsILRDUIDEqual(&uid, &bad_uid));
	assert (!PQueueIsEmpty(sc->pq));
	
	TaskDestroy(PQueueRemove(sc->pq, (const void*)&uid, cmp_func));	
}

int SchedulerRun(scheduler_t* sc)
{
	task_status status = REPEAT;
	time_t current_time = 0;
	double time_remain = 0;
	
	assert (NULL != sc);
	
	while (!PQueueIsEmpty(sc->pq) && !(sc->flag_is_stoped))
	{
		sc->current = (task_t*)(PQueueDequeue(sc->pq));
		time(&current_time);
		time_remain = difftime((TaskGetReadyTime((const task_t*)sc->current))
												, current_time);
		if (time_remain > 0)
		{
			sleep(time_remain);
		}
		
		status = TaskDoFunc(sc->current);
		if (sc->need_to_remove_self || FAILURE == status)
		{
			TaskDoCleanupFunc(sc->current);
			TaskDestroy(sc->current);
			sc->need_to_remove_self = 0;
		}
		else if (REPEAT == status)
		{	
			TaskUpdateReadyTime(sc->current);
			PQueueEnqueue(sc->pq, (const void*)sc->current);
		}
		
	}
	
	sc->current = NULL;
	
	return status;
}

void SchedulerStop(scheduler_t* sc)
{
	assert (NULL != sc);
	
	sc->flag_is_stoped = 1;
}

size_t SchedulerCount(const scheduler_t* sc)
{
	assert (NULL != sc);
	
	return PQueueSize(sc->pq);
}

int SchedulerIsEmpty(const scheduler_t* sc)
{
	return (sc->current == NULL && PQueueIsEmpty(sc->pq));
}
