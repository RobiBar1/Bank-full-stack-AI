/*
Writer:  Robi
Checker: Omer
Date: 	 21.01.2026
*/

#include <assert.h> /* assert 			         */
#include <stdio.h>  /* printf 			         */
#include <stdlib.h> /* malloc, free 		     */

#include "heap.h"   /* our heap api  			 */
#include "pqueue.h" /* our api 			 		 */

struct pqueue
{
    heap_t* heap;
};

pqueue_t* PQueueCreate(priority_cmp_t func)
{
    pqueue_t* pq = NULL;

    assert(NULL != func);

    pq = (pqueue_t*)malloc(sizeof(pqueue_t));
    if (NULL == pq)
    {
        return NULL;
    }

    pq->heap = HeapCreate(1, func);
    if (NULL == pq->heap)
    {
        free(pq);
        pq = NULL;
        return NULL;
    }

    return pq;
}

void PQueueDestroy(pqueue_t* que)
{
    assert(NULL != que);

    HeapDestroy(que->heap);
    que->heap = NULL;
    free(que);
    que = NULL;
}

int PQueueEnqueue(pqueue_t* que, const void* data)
{
    assert(NULL != que);

    return HeapPush(que->heap, data);
}

int PQueueIsEmpty(const pqueue_t* que)
{
    assert(NULL != que);

    return HeapIsEmpty(que->heap);
}

void* PQueueDequeue(pqueue_t* que)
{
    void* data = NULL;

    assert(NULL != que);
    assert(!HeapIsEmpty(que->heap));

    data = HeapPeek(que->heap);
    HeapPop(que->heap);

    return data;
}

void* PQueuePeek(const pqueue_t* que)
{
    assert(NULL != que);
    assert(!HeapIsEmpty(que->heap));

    return HeapPeek(que->heap);
}

size_t PQueueSize(const pqueue_t* que)
{
    assert(NULL != que);

    return HeapCount(que->heap);
}

void* PQueueRemove(pqueue_t* que, const void* param,
                   int (*is_match)(const void* data, const void* param))
{
    assert(NULL != que);
    assert(NULL != is_match);

    return HeapRemove(que->heap, param, is_match);
}
