/*
Write by: Robi
review by: Hen
date: 04.01.2026
*/

#include <assert.h> 			/* assert */
#include <stdlib.h>				/* malloc */

#include "queue.h"  			/* our api */
#include "singly_linked_list.h" /* SListCreate, etc.. */
#define FAIL (1)
#define SUCCESS (0)

struct queue
{
	slist_t* list;
};

queue_t* QueueCreate(void)
{
	queue_t* que = (queue_t*)malloc(sizeof(queue_t));
	if (NULL == que)
	{
		return NULL;
	}
	
	que->list = NULL;
	que->list = SListCreate();
	if (NULL == que->list)
	{
		free(que); que = NULL;
		return NULL;
	}
	
	return que;
}

void QueueDestroy(queue_t* que)
{
	assert(NULL != que);
	
	SListDestroy(que->list);
	que->list = NULL;
	free(que); que = NULL;
}

int QueueEnqueue(queue_t* que, void* data)
{
	assert(NULL != que);
	
	if (NULL == SListInsert(
		que->list, SListEnd(que->list), data))
	{
		return FAIL;
	}
	
	return SUCCESS;
}

void QueueDequeue(queue_t* que)
{
	assert(NULL != que);
	
	SListRemove(SListBegin(que->list));
}

void* QueuePeek(const queue_t* que)
{
	assert(NULL != que);
	
	return SListGetData(SListBegin(que->list));
}

size_t QueueSize(const queue_t* que)
{
	assert(NULL != que);
	
	return SListCount((const slist_t*)que->list); 	
}

void QueueAppend(queue_t* dst, queue_t* src)
{
	SListAppend(dst->list, src->list);
}

int QueueIsEmpty(const queue_t* que)
{
	assert(NULL != que);
	
	return SListIsEmpty((const slist_t*)que->list);
}
