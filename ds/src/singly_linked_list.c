#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */

#include "singly_linked_list.h"

#define SUCCESS (0)

struct slist
{
	slist_iter_t head;
	slist_iter_t tail;
};

struct slist_node
{
	void* val;
	slist_iter_t next;
};

static slist_iter_t CreateNode(const void* data)
{
	slist_iter_t new_node = NULL;
	
	new_node = (slist_iter_t)malloc(sizeof(struct slist_node));
	if (NULL == new_node)
	{
		return NULL;
	}
	
	new_node->val = (void*)data;
	new_node->next = NULL;
	
	return new_node;
}

slist_t* SListCreate(void)
{
	slist_iter_t dummy = NULL;
	slist_t* list = (slist_t*)malloc(sizeof(slist_t));
	
	if (NULL == list)
	{
		return NULL;
	}
	
	list->head = NULL;
	list->tail = NULL;
	dummy = CreateNode(&list->tail);
	if (NULL == dummy)
	{
		free(list); list = NULL;
		return NULL;
	}
	
	list->head = dummy;
	list->tail = dummy;
	
	return list;
}

void SListDestroy(slist_t* list)
{
	slist_iter_t current = NULL;
	slist_iter_t next = NULL;
	
	assert(NULL != list);
	
	current = list->head;	
	while (NULL != current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	
	free(list); list = NULL;
}

slist_iter_t SListInsert(slist_t* list, slist_iter_t iter_pos, const void* data)
{
	slist_iter_t new_node = NULL;
	
	assert(NULL != list);
	assert(NULL != iter_pos);
	
	new_node = CreateNode(iter_pos->val);
	if (NULL == new_node)
	{
		return list->tail;
	}
	
	iter_pos->val = (void*)data;
	new_node->next = iter_pos->next;
	iter_pos->next = new_node;
	
	if (iter_pos == list->tail)
	{
		list->tail = new_node;
	}
	
	return iter_pos;
}

slist_iter_t SListRemove(slist_iter_t iter)
{
	slist_iter_t temp = NULL;
	
	assert(NULL != iter);
	assert(NULL != iter->next);
	
	temp = iter->next;
	iter->val = iter->next->val;
	iter->next = iter->next->next;
	if (NULL == iter->next)
	{
		*((slist_iter_t*)iter->val) = iter;
	}
	
	free(temp);
	
	return iter;
}

slist_iter_t SListFind(slist_iter_t from, slist_iter_t to, const void* val, match_func_t is_match_func)
{
	assert(NULL != is_match_func);
	
	while (from != to)
	{
		if (is_match_func(from->val, (void*)val))
		{
			return from;
		}
		
		from = from->next;
	}
	
	return to;
}

slist_iter_t SListBegin(const slist_t* list)
{
	assert(NULL != list);
	
	return list->head;
}

slist_iter_t SListEnd(const slist_t* list)
{
	assert(NULL != list);
	
	return list->tail;
}

slist_iter_t SListNext(slist_iter_t iter)
{
	assert(NULL != iter);
	assert(NULL != iter->next);
	
	return iter->next;
}

size_t SListCount(const slist_t* list)
{
	size_t count = 0;
	slist_iter_t current = NULL;
	
	assert(NULL != list);
	
	current = list->head;
	while (NULL != current->next)
	{
		++count;
		current = current->next;
	}
	
	return count;
}

int SListIsEmpty(const slist_t* list)
{
	assert(NULL != list);
	
	return (list->head == list->tail);
}

void* SListGetData(slist_iter_t iter)
{
	assert(NULL != iter);
	
	return iter->val;
}

void SListSetData(slist_iter_t iter, const void* data)
{
	assert(NULL != iter);
	
	iter->val = (void*)data;
}

int SListForEach(slist_iter_t from, slist_iter_t to, action_func_t action_func, void* param)
{
	int status = 0;
	
	assert(NULL != action_func);
	
	while (from != to && SUCCESS == status)
	{
		status = action_func(from->val, param);
		from = from->next;
	}
	
	return status;
}

int SListIsIterEqual(slist_iter_t iter1, slist_iter_t iter2)
{
	return (iter1 == iter2);
}

static void MakeListDummy(slist_t* src)
{
	src->tail = src->head;
	src->head->val = &src->tail;
	src->tail->val = &src->tail;		
	src->head->next = NULL;		
	src->tail->next = NULL;		
}

static void CopyHeadToDummy(slist_t* dst, slist_t* src)
{
	dst->tail->val = src->head->val;
	dst->tail->next = src->head->next;
}

void SListAppend(slist_t* dst, slist_t* src)
{	
	assert(NULL != dst);
	assert(NULL != src);
	
	if (SListIsEmpty(src))
	{
		return;
	}
	
	CopyHeadToDummy(dst, src);
	
	dst->tail = src->tail;
	
	MakeListDummy(src);
	dst->tail->val = &dst->tail;
}
