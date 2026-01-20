#include <assert.h> 	  /* assert 			 */
#include <stdlib.h> 	  /* malloc, free 		 */
#include <stdio.h>  	  /* printf 			 */

#include "pqueue.h"       /* our api 			 */
#include "sorted_list.h"  /* our sorted_list api */

struct pqueue 
{
	sorted_list_t* sorted_list;
};

pqueue_t* PQueueCreate(priority_cmp_t func)
{
	pqueue_t* pq = NULL;
	
	assert (NULL != func);
	
	pq = (pqueue_t*)malloc(sizeof(pqueue_t));
	if (NULL == pq)
	{
		return NULL;
	}
	
	pq->sorted_list = SortedLCreate(func);
	if (NULL == pq->sorted_list)
	{
		free(pq); pq = NULL;
		return NULL;
	}
	
	return pq;
}

void PQueueDestroy(pqueue_t* que)
{
	assert (NULL != que);
	
	SortedLDestroy(que->sorted_list);
	que->sorted_list = NULL;
	free(que); que = NULL;
}

int PQueueEnqueue(pqueue_t* que, const void* data)
{
	assert (NULL != que);
	
	return (SortedLIsIterEqual(SortedLEnd(que->sorted_list),
										SortedLInsert(que->sorted_list, data)));
}

int PQueueIsEmpty(const pqueue_t* que)
{
	assert (NULL != que);
	
	return SortedLIsEmpty(que->sorted_list);
}

void* PQueueDequeue(pqueue_t* que)
{
	assert (NULL != que);
	assert (!PQueueIsEmpty(que));
	
	return SortedLPopFront(que->sorted_list);
}

void* PQueuePeek(const pqueue_t* que)
{
	assert (NULL != que);
	assert (!PQueueIsEmpty(que));
	
	return SortedLGetData(SortedLBegin(que->sorted_list));
}

size_t PQueueSize(const pqueue_t* que)
{
	assert (NULL != que);
	
	return SortedLCount(que->sorted_list);
}

void* PQueueRemove(pqueue_t* que,const void* param, int (*is_match)(const void* data, const void* param))
{
	sorted_list_iter_t to = {0}; 
	sorted_list_iter_t result = {0}; 
	void* data = NULL;
	
	assert (NULL != que);
	assert (!PQueueIsEmpty(que));
	assert (NULL != is_match);
	
	to = SortedLEnd(que->sorted_list);
	result = SortedLFindIf(SortedLBegin(que->sorted_list), to, is_match, param);
	if (SortedLIsIterEqual(to, result))
	{
		return NULL;
	}
	
	data = SortedLGetData(result);
	SortedLRemove(result);
	
	return data;
}

int is_match(const void* one, const void* two)
{
	return (*(int*)one - *(int*)two);	
}

int main()
{
	pqueue_t* pq = PQueueCreate(is_match);
	int x = 9;
	if (NULL != pq)
	{
		printf("create work\n");
	}
	
	printf("size is: %lu\n", PQueueSize(pq));
	printf("pq isemtpy is: %d\n", PQueueIsEmpty(pq));
	PQueueEnqueue(pq, &x);
	printf("size is: %lu\n", PQueueSize(pq));
	printf("pq isemtpy is: %d\n", PQueueIsEmpty(pq));
	printf("pq peek is: %d\n", *((int*)PQueuePeek(pq)));
	printf("pq Dequeue is: %d\n", *((int*)PQueueDequeue(pq)));
	printf("size is: %lu\n", PQueueSize(pq));
	printf("pq isemtpy is: %d\n", PQueueIsEmpty(pq));
	PQueueEnqueue(pq, &x);
	
	x = 5;
	printf("size is: %lu\n", PQueueSize(pq));
	PQueueEnqueue(pq, &x);
	
	x = 7;
	PQueueEnqueue(pq, &x);
	printf("size is: %lu\n", PQueueSize(pq));
	printf("pq isemtpy is: %d\n", PQueueIsEmpty(pq));
	printf("pq peek is: %d\n", *((int*)PQueuePeek(pq)));
	
	PQueueDestroy(pq);
	pq = NULL;
	
	return 0;
}

