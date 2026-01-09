/*
Writer:  Robi
Checker: Roie
Date:    08.1.2026
*/

#include <stddef.h>      /* NULL, size_t */
#include <stdlib.h>      /* malloc, free */
#include <assert.h>      /* assert       */

#include "d_link_list.h" /* our api      */

#define FOUND (0)
#define ALOCATE_MEMORY_SUCCESS (0)
#define ALOCATE_MEMORY_FAILED (1)
#define SUCCESS (0)
#define UNUSED(x) (void)(x)

typedef struct dlist_node
{
	void* val;
	struct dlist_node* next;
	struct dlist_node* prev;
} dlist_node;

struct dlist
{
	dlist_node* head;
	dlist_node* tail;
};

/*============================================================================*/
/* ------------------------- Start Convert functions ------------------------ */

static dlist_node* IterToDLLNode(dlist_iter_t iter)
{
	return (dlist_node*)iter;
}

static dlist_iter_t DLLNodeToIter(dlist_node* node)
{
	return (dlist_iter_t)node;
}

/* ------------------------- End Convert functions -------------------------- */
/*============================================================================*/
/* ------------------------- Start Getters & Setters ------------------------ */

static void* DlistGetVal(const dlist_node* node)
{
	assert (NULL != node);
	
	return node->val;
}

static void DlistSetVal(dlist_node* node, void* val)
{	
	assert (NULL != node);
	
	node->val = val;
}

static dlist_node* NodeGetNext(const dlist_node* node)
{
	assert (NULL != node);
	
	return node->next;
}

static void DlistSetNext(dlist_node* node, dlist_node* next)
{
	assert (NULL != node);
	
	node->next = next;
}

static dlist_node* NodeGetPrev(const dlist_node* node)
{
	assert (NULL != node);
	
	return node->prev;
}

static void DlistSetPrev(dlist_node* node, dlist_node* prev)
{
	assert (NULL != node);
	
	node->prev = prev;
}

static dlist_node* DlistGetHead(const dlist_t* list)
{
	assert (NULL != list);
	
	return list->head;
}

static void DlistSetHead(dlist_t* list, dlist_node* head)
{
	assert (NULL != list);
	
	list->head = head;
}

static dlist_node* DlistGetTail(const dlist_t* list)
{
	assert (NULL != list);
	
	return list->tail;
}

static void DlistSetTail(dlist_t* list, dlist_node* tail)
{
	assert (NULL != list);
	
	list->tail = tail;
}

/* ------------------------- End Getters & Setters -------------------------- */
/*============================================================================*/
/* ------------------------- Start Helper functions ------------------------- */

static dlist_node* CreateNode(const void* data, dlist_node* next, dlist_node* prev)
{
	dlist_node* new_node = NULL;
	
	new_node = (dlist_node*)malloc(sizeof(dlist_node));
	if (NULL == new_node)
	{
		return NULL;
	}
	
	DlistSetVal(new_node, (void*)data);
	DlistSetNext(new_node, next);
	DlistSetPrev(new_node, prev);
	
	return new_node;
}

static int CounterToUseInForEach(void* data, void* counter)
{
	UNUSED(data);
	assert (NULL != counter);
	
	++(*(size_t*)counter);
	
	return SUCCESS;
}

/* ------------------------- End Helper functions --------------------------- */
/*============================================================================*/
/* ------------------------- Start H.file functions ------------------------- */

dlist_t* DListCreate(void)
{
	dlist_node* head_dummy = NULL;
	dlist_node* tail_dummy = NULL;
	dlist_t* list = NULL;
	
	list = (dlist_t*)malloc(sizeof(dlist_t));
	if (NULL == list)
	{
		return NULL;
	}
	
	tail_dummy = CreateNode(NULL, NULL, NULL);
	if (NULL == tail_dummy)
	{
		free(list); list = NULL;
		
		return NULL;
	}
	
	head_dummy = CreateNode(NULL, tail_dummy, NULL);
	if (NULL == head_dummy)
	{
		free(tail_dummy); tail_dummy = NULL;
		free(list); list = NULL;
		
		return NULL;
	}
	
	DlistSetPrev(tail_dummy, head_dummy);
	DlistSetHead(list, head_dummy);
	DlistSetTail(list, tail_dummy);
	
	return list;
}

void DListDestroy(dlist_t* list)
{
	dlist_node* current = NULL;
	dlist_node* next = NULL;
	
	assert (NULL != list);
	
	current = DlistGetHead(list);
	while (NULL != current)
	{
		next = NodeGetNext(current);
		free(current);
		current = next;
	}
	
	free(list); list = NULL;
}

int DListPushBack(dlist_t* list, const void* data)
{
	dlist_iter_t tail_iter = NULL;
	dlist_iter_t result = NULL;
	
	assert (NULL != list);
	
	tail_iter = DLLNodeToIter(DlistGetTail(list));
	result = DListInsert(tail_iter, data);
	
	if (DListIsIterEqual(result, tail_iter))
	{
		return ALOCATE_MEMORY_FAILED;
	}
	
	return SUCCESS;
}

int DListPushFront(dlist_t* list, const void* data)
{
	dlist_iter_t begin_iter = NULL;
	dlist_iter_t result = NULL;
	
	assert (NULL != list);
	
	begin_iter = DListBegin(list);
	result = DListInsert(begin_iter, data);
	if (DListIsIterEqual(result, DLLNodeToIter(DlistGetTail(list))))
	{
		return ALOCATE_MEMORY_FAILED;
	}
	
	return SUCCESS;
}

void* DListPopBack(dlist_t* list)
{
	void* data = NULL;
	dlist_iter_t iter = NULL;
	
	assert (NULL != list);
	
	iter = DListGetPrev(DLLNodeToIter(DlistGetTail(list)));
	data = DListGetData(iter);
	DListRemove(iter);
	
	return data;
}

void* DListPopFront(dlist_t* list)
{
	void* data = NULL;
	dlist_iter_t iter = NULL;
	
	assert (NULL != list);
	
	iter = DListGetNext(DLLNodeToIter(DlistGetHead(list)));
	data = DListGetData(iter);
	DListRemove(iter);
	
	return data;
}

dlist_iter_t DListRemove(dlist_iter_t iter)
{
	dlist_node* prev_node = NULL;
	dlist_node* next_node = NULL;
	dlist_node* iter_node = IterToDLLNode(iter);
	
	assert (NULL != iter_node);
	assert (NULL != NodeGetPrev(iter_node));
	assert (NULL != NodeGetNext(iter_node));
	
	prev_node = NodeGetPrev(iter_node);
	next_node = NodeGetNext(iter_node);
	
	DlistSetNext(prev_node, next_node);
	DlistSetPrev(next_node, prev_node);
	
	free(iter_node); iter_node = NULL;
	
	return DLLNodeToIter(next_node);
}

dlist_iter_t DListInsert(dlist_iter_t pos, const void* data)
{
	dlist_node* new_node = NULL;
	dlist_node* prev_node = NULL;
	dlist_node* iter_node = IterToDLLNode(pos);
	
	assert (NULL != iter_node);
	
	prev_node = NodeGetPrev(iter_node);
	new_node = CreateNode(data, iter_node, prev_node);
	if (NULL == new_node)
	{
		return pos;
	}
	
	DlistSetNext(prev_node, new_node);
	DlistSetPrev(iter_node, new_node);
	
	return DLLNodeToIter(new_node);
}

void DListSetData(const dlist_iter_t iter, const void* data)
{
	dlist_node* d_node = IterToDLLNode(iter);;
	
	assert (NULL != d_node);
	
	DlistSetVal(d_node, (void*)data);
}

void* DListGetData(const dlist_iter_t iter)
{
	dlist_node* d_node = IterToDLLNode(iter);
	
	assert (NULL != d_node);
	
	return DlistGetVal(d_node);
}

int DListIsEmpty(const dlist_t* list)
{
	assert (NULL != list);
	
	return (NodeGetNext(DlistGetHead(list)) == DlistGetTail(list));
}

dlist_iter_t DListBegin(const dlist_t* list)
{
	assert (NULL != list);
	
	return DLLNodeToIter(NodeGetNext(DlistGetHead(list)));
}

dlist_iter_t DListEnd(const dlist_t* list)
{
	assert (NULL != list);
	
	return DLLNodeToIter(DlistGetTail(list));
}

dlist_iter_t DListGetNext(const dlist_iter_t iter)
{
	dlist_node* iter_node = IterToDLLNode(iter);
	
	assert (NULL != iter_node);
	
	return DLLNodeToIter(NodeGetNext(iter_node));
}

dlist_iter_t DListGetPrev(const dlist_iter_t iter)
{
	dlist_node* iter_node = IterToDLLNode(iter);
	
	assert (NULL != iter_node);
	
	return DLLNodeToIter(NodeGetPrev(iter_node));
}

int DListIsIterEqual(const dlist_iter_t iter1, const dlist_iter_t iter2)
{
	return (iter1 == iter2);
}

void DListSplice(dlist_iter_t from, dlist_iter_t to, dlist_iter_t where)
{
	dlist_node* from_n = IterToDLLNode(from);
	dlist_node* to_n = IterToDLLNode(to);
	dlist_node* where_n = IterToDLLNode(where);
	dlist_node* where_prev = NULL;
	
	assert (NULL != from_n);
	assert (NULL != to_n);
	assert (NULL != where_n);
	
	where_prev = NodeGetPrev(where_n);
	DlistSetNext(NodeGetPrev(to_n), where_n);
	DlistSetPrev(where_n, NodeGetPrev(to_n));
	
	DlistSetNext(NodeGetPrev(from_n), to_n);
	DlistSetPrev(to_n, NodeGetPrev(from_n));
	
	DlistSetNext(where_prev, from_n);
	DlistSetPrev(from_n, where_prev);
}

size_t DListCount(const dlist_t* list)
{
	size_t count = 0;
	
	assert (NULL != list);
	
	DListForEach(DListBegin(list), DListEnd(list), CounterToUseInForEach, (void*)(&count));
	
	return count;
}

int DListForEach(dlist_iter_t from, dlist_iter_t to, int (*dll_action_func)(void* data, void* param), void* param)
{
	int status = 0;
	dlist_node* from_node = NULL;
	
	assert (NULL != to);
	assert (NULL != from);
	assert (NULL != dll_action_func);
	
	while (from != to && SUCCESS == status)
	{
		from_node = IterToDLLNode(from);
		status = dll_action_func(DlistGetVal(from_node), param);
		from = DListGetNext(from);
	}
	
	return status;
}

dlist_iter_t DListFind(dlist_iter_t from, dlist_iter_t to, const void* data, int (*dll_is_match)(const void* data, const void* param))
{
	dlist_node* from_node = IterToDLLNode(from);
	
	assert (NULL != from_node);
	assert (NULL != IterToDLLNode(to));
	assert (NULL != dll_is_match);
	
	while (from != to)
	{
		from_node = IterToDLLNode(from);
		if (dll_is_match(DlistGetVal(from_node), data))
		{
			return from;
		}
		
		from = DListGetNext(from);
	}
	
	return to;
}

int DListMultiFind(dlist_iter_t from, dlist_iter_t to, const void* data, 
                   int (*dll_is_match)(const void* data, const void* param), 
                   dlist_t* out_list)
{
	dlist_iter_t found_iter = NULL;
	int status_memory = SUCCESS;
	
	assert (NULL != IterToDLLNode(from));
	assert (NULL != IterToDLLNode(to));
	assert (NULL != dll_is_match);
	assert (NULL != out_list);
	
	found_iter = DListFind(from, to, data, dll_is_match);
	while (!DListIsIterEqual(found_iter, to))
	{
		status_memory = DListPushBack(out_list, DListGetData(found_iter));
		if (ALOCATE_MEMORY_SUCCESS != status_memory)
		{
			return status_memory;
		}
		
		from = DListGetNext(found_iter);
		found_iter = DListFind(from, to, data, dll_is_match);
	}
	
	return SUCCESS;
}
