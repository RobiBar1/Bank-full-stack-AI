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
	
	dummy = CreateNode(list);
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
	void* temp_data = NULL;
	
	assert(NULL != list);
	assert(NULL != iter_pos);
	
	temp_data = iter_pos->val;
	iter_pos->val = (void*)data;
	new_node = CreateNode(temp_data);
	if (NULL == new_node)
	{
		iter_pos->val = temp_data;
		
		return list->tail;
	}
	
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
	slist_iter_t next_iter = NULL;
	slist_t* list = NULL;
	
	assert(NULL != iter);
	assert(NULL != iter->next);
	
	next_iter = iter->next;
	if (NULL == next_iter->next)
	{
		list = (slist_t*)next_iter->val;
		iter->next = NULL;
		free(next_iter); next_iter = NULL;
		list->tail = iter;
		iter->val = (void*)list;
	}
	else
	{
		iter->val = next_iter->val;
		iter->next = next_iter->next;
		free(next_iter); next_iter = NULL;
	}
	
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
	while (current != list->tail)
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
