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
#define TIME_FAIL (-1)
#define PQ_ENQUEUE_FAIL (1)

struct scheduler
{
	task_t* current;
	pqueue_t* pq;
	int flag_is_stoped;
	int need_to_remove_self;
};

/*----------------------- Start Helper Functions -----------------------------*/
static void SchedulerCleanDestroyTask(scheduler_t* sc)
{
	TaskDoCleanupFunc(sc->current);
	TaskDestroy(sc->current);
	sc->need_to_remove_self = 0;
}

static int ActiveFuncRoutine(scheduler_t* sc)
{
	task_status status = REPEAT;
	
	assert (NULL != sc);
		
	status = TaskDoFunc(sc->current);
	if (sc->need_to_remove_self || FAILURE == status 
								|| DO_NOT_REPEAT == status)
	{
		SchedulerCleanDestroyTask(sc);
	}
	else if (REPEAT == status)
	{	
		TaskUpdateReadyTime(sc->current);
		if (SECCESS != PQueueEnqueue(sc->pq, (const void*)sc->current))
		{
			SchedulerCleanDestroyTask(sc);
			
			return PQ_ENQUEUE_FAIL;
		}
	}
	
	return SECCESS;
}

static int TimeCheckUntilRun(scheduler_t* sc)
{
	time_t current_time = 0;
	double time_remain = 1;
	
	assert (NULL != sc);
	
	sc->current = (task_t*)(PQueueDequeue(sc->pq));
	while (time_remain > 0)
	{
		time(&current_time);
		if (TIME_FAIL == current_time)
		{
			return current_time;
		}
		
		time_remain = difftime((TaskGetReadyTime((const task_t*)sc->current))
											, current_time);
		sleep(time_remain);
	}
	
	return SECCESS;
}
/*----------------------- End Helper Functions -------------------------------*/

/*----------------------- Start H.file Functions -----------------------------*/


/*------------------- Start Pointers Functions -----------------------*/
static int CmpFunc(const void* left, const void* right)
{
	ilrd_uid_t uid_left = TaskGetUid((const task_t*)left);
	ilrd_uid_t uid_right = TaskGetUid((const task_t*)right);
	
	 return !(IsILRDUIDEqual((const ilrd_uid_t*)(&uid_left),
	 						 (const ilrd_uid_t*)(&uid_right)));		
}

static int PriorityCmp(const void* one, const void* two)
{	
	assert (NULL != one);
	assert (NULL != two);
	
	return TaskGetReadyTime((const task_t*)one) -
		   TaskGetReadyTime((const task_t*)two);	
}
/*------------------- End Pointers Functions -----------------------*/

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
	void* task = NULL;
	
	assert (NULL != sc);
	
	while (!PQueueIsEmpty(sc->pq))
    {
    	task = PQueueDequeue(sc->pq);
    	
		TaskDoCleanupFunc(task);
        TaskDestroy(task);
    }

	PQueueDestroy(sc->pq);
	sc->pq = NULL;
	sc->current = NULL;
	free(sc); sc = NULL;
}

void SchedulerRemoveTask(scheduler_t* sc, ilrd_uid_t uid)
{
	assert (NULL != sc);
	assert (!IsILRDUIDEqual(&uid, &bad_uid));
	assert (!PQueueIsEmpty(sc->pq));
	
	TaskDestroy(PQueueRemove(sc->pq, (const void*)&uid, CmpFunc));	
}
                    
ilrd_uid_t SchedulerAddTask(scheduler_t* sc, size_t time_interval,
		task_func_t task_func, cleanup_func_t cleanup_func,const void* param)
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

int SchedulerRun(scheduler_t* sc)
{
	int func_status = 0;
	int time_status = 0;
	
	assert (NULL != sc);
	
	while (!PQueueIsEmpty(sc->pq) && !(sc->flag_is_stoped))
	{
		time_status = TimeCheckUntilRun(sc);
		func_status = ActiveFuncRoutine(sc);
		
		if (SECCESS != func_status || SECCESS != time_status)
		{
			return func_status;
		}
	}
	
	sc->current = NULL;
	sc->flag_is_stoped = 0;
	
	return SECCESS;
}

void SchedulerStop(scheduler_t* sc)
{
	assert (NULL != sc);
	
	sc->flag_is_stoped = 1;
}

size_t SchedulerCount(const scheduler_t* sc)
{
	assert (NULL != sc);
	
	return PQueueSize(sc->pq) + (sc->current != NULL);
}

int SchedulerIsEmpty(const scheduler_t* sc)
{
	return (sc->current == NULL && PQueueIsEmpty(sc->pq));
}
/*----------------------- End H.file Functions -----------------------------*/
