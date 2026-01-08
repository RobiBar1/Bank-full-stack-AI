/*
Writer:  Robi
Checker: Roie
Date: 	 08.1.2026
*/
#include <stddef.h> 	 /* for NULL */

#include "d_link_list.h" /* our api */

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
}dlist_node;

struct dlist
{
	dlist_node* head;
	dlist_node* tail;
}dlist; 

typedef struct dlist dlist_t;
typedef struct dlist_node* dlist_iter_t;

dlist_t* DlistGetHead(const dlist_t* list);

/*============================================================================*/
/* ------------------------- Start Convert functions ------------------------ */

static dlist_node* IterToNode(dlist_iter_t iter)
{
	return iter;
}

static dlist_iter_t NodeToIter(dlist_node* node)
{
	return node;
}

/* ------------------------- End Convert functions -------------------------- */
/*============================================================================*/
/* ------------------------- Start Getters & Setters ------------------------ */
struct dlist_node
{
	void* val;
	struct dlist_node* next;
	struct dlist_node* prev;
}dlist_node;

struct dlist
{
	dlist_iter_t head;
	dlist_iter_t tail;
}dlist; 

typedef struct dlist dlist_t;
typedef struct dlist_node* dlist_iter_t;

void* DlistGetVal(const dlist_node* node)
{
	assert (NULL != node);
	
    return node->val;
}

void DlistSetVal(dlist_node* node, const void* val)
{	
	assert (NULL != node);
	
	node->val = val;
}

dlist_iter_t DlistGetNext(const dlist_iter_t iter)
{
	assert (NULL != ((dlist_node*)node));
	
    return convert///next;
}

dlist_node* NodeGetNext(dlist_node* node)
{
	assert (NULL != node);
	
	return node->next;
}

void DlistSetNext(dlist_node* node, dlist_node* next)
{
	assert (NULL != node);
	
	node->next = next;
}

dlist_iter_t DListGetPrev(const dlist_iter_t iter)
{
	assert (NULL != node);
	
    return (dlist_iter_t)(node->prev);
}

void DlistSetPrev(dlist_node* node, dlist_node* prev)
{
    assert (NULL != node);
    
    node->prev = prev;
}

dlist_t* DlistGetHead(const dlist_t* list)
{
	assert (NULL != list);
	
    return dlist_t->head;
    
}

void DlistSetHead(dlist_t* list, dlist_iter_t head)
{
	assert (NULL != list);
	
	list->head = head;
}

dlist_t* DlistGetTail(const dlist_t* list)
{
	assert (NULL != list);
	
    return list->tail;
}

void DlistSetTail(dlist_t* list, dlist_node* tail)
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
	
	new_node = (dlist_node*)malloc(sizeof(struct dlist_node));
	if (NULL == new_node)
	{
		return NULL;
	}
	
	new_node->val = (void*)data;
	new_node->next = next;
	new_node->prev = prev;
	
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
	dlist_node* dummy = NULL;
	dlist_t* list = (dlist_t*)malloc(sizeof(dlist_t));
	if (NULL == list)
	{
		return NULL;
	}
	
	DlistSetTail(list, NULL);
	DlistSetHead(list, NULL);
	dummy = CreateNode(&(DlistGetTail(list)), NULL, NULL);
	if (NULL == dummy)
	{
		free(list); list = NULL;
		return NULL;
	}
	
	DlistSetTail(list, ConvertDListNodeToIter(dummy));
	dummy = CreateNode(list, list, NULL);
	if (NULL == dummy)
	{
		free(list); list = NULL;
		return NULL;
	}
	
	DlistSetHead(list, ConvertDListNodeToIter(dummy));
	DlistSetPrev(DlistGetTail(list), list);
	
	return list;
}

void DListDestroy(dlist_t* list)
{
	slist_iter_t current = NULL;
	slist_iter_t next = NULL;
	
	assert (NULL != list);
	
	current = list->head;	
	while (NULL != current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	
	free(list); list = NULL;
}

int DListPushBack(dlist_t* list, const void* data)
{
	if (DlistGetTail(list) == DListInsert(DlistGetTail(list), data))
	{
		return ALOCATE_MEMORY_FAILED;
	}
	
	return SUCCESS;
}

int DListPushFront(dlist_t* list, const void* data)
{
	return !(DListIsIterEqual(DListInsert(DListBegin((const dlist_t*)list) , data)
	, DlistGetTail(list)));
}

void* DListPopBack(dlist_t* list)
{
	return DListRemove((DlistGetPrev(NodeToIter(DlistGetTail(list)))));
}

void* DListPopFront(dlist_t* list)
{
	return DListRemove((DlistGetNext(NodeToIter(DlistGetHead(list)))));
}

dlist_iter_t DListRemove(dlist_iter_t iter)
{
	
	dlist_node* temp = NULL;
	dlist_node* iter_node = IterToNode(iter);
	
	assert(NULL != DlistGetNext(iter_node));
	assert(NULL != DlistGetPrev(iter_node));
	
	temp = DlistGetNext(iter_node);
	DlistSetVal(iter_node, DlistGetVal(DlistGetNext(iter_node)));
	DlistSetNext(iter_node, (DlistGetNext(DlistGetNext(iter_node)));
	DlistSetPrev(DlistGetNext(DlistGetNext(iter_node)), iter_node);
	if (NULL == DlistGetNext(iter_node))
	{
		DlistSetVal(iter_node, iter);
	}
	
	free(temp); temp = NULL:
	
	return iter;
}

dlist_iter_t DListInsert(dlist_iter_t pos, const void* data)
{
	
	dlist_node* new_node = NULL;
	dlist_node* tmp = NULL;
	dlist_node* iter_node = IterToNode(pos);
	
	assert (NULL != list);

	new_node = CreateNode(data, iter_node, DlistGetPrev(iter_node));
	if (NULL == new_node)
	{
		return DlistGetTail(list);
	}
	
	DlistSetNext(DlistGetPrev(iter_node), new_node);
	DlistSetPrev(DlistGetNext(new_node), new_node);
	
	return iter_pos;
}

void DListSetData(const dlist_iter_t iter, const void* data)
{
	dlist_node* d_node = IterToNode(iter);
	d_node->val = data;
}

void* DListGetData(const dlist_iter_t iter)
{
	dlist_node* d_node = IterToNode(iter);
	
	return d_node->val;
}

int DListIsEmpty(const dlist_t* list)
{
	return (DListBegin(list) != NodeToIter(DlistGetHead(list)));
}

dlist_iter_t DListBegin(const dlist_t* list)
{
	if (DListIsEmpty(list))
	{
		return DlistGetTail(list);
	}
	 	
	return (DlistGetNext(DlistGetHead(list)));
}

dlist_iter_t DListEnd(const dlist_t* list)
{
	assert (NULL != list);
	
	return DlistGetTail(list);
}

int DListIsIterEqual(const dlist_iter_t iter1, const dlist_iter_t iter2)
{
		return (((dlist_node*)iter1) == ((dlist_node*)iter2));
}

void DListSplice(dlist_iter_t from, dlist_iter_t to, dlist_iter_t where)
{
	dlist_node* from_n = IterToNode(from);
	dlist_node* to_n = IterToNode(to);	
	dlist_node* where_n = IterToNode(where);	
	dlist_node* tmp = NULL;
	dlist_node* where_prev = NULL;
	
	assert (NULL != from_n);
	assert (NULL != to_n);
	assert (from != DlistGetHead(list));
	assert (from != DlistGetTail(list));
	assert (where != DlistGetHead(list));
	
	where_prev = IterToNode(DlistGetPrev(where_n));
	DlistSetNext(where_prev, from_n);
	
	tmp = IterToNode(DlistGetPrev(from_n));
	DlistSetNext(tmp, to_n);
	
	tmp = IterToNode(DlistGetPrev(to_n));
	DlistSetNext(tmp, where_n);
	DlistSetPrev(where_n, tmp);
	
	DlistSetPrev(to_n, IterToNode(DlistGetPrev(from_n)));
	DlistSetPrev(from_n, where_prev);	
}
size_t DListCount(const dlist_t* list)
{
	size_t count = 0;
	
	assert (NULL != list);
	
	DListForEach(DListBegin(list),
	 DListEnd(list),CounterToUseInForEach,
	  (void*)(&count));
	  
	return count;

}

int DListForEach(dlist_iter_t from, dlist_iter_t to, int (*dll_action_func)(void* data, void* param), void* param)
{
	int status = 0;
	dlist_node* from_node = NULL;
	
	assert(NULL != to);
	assert(NULL != from);
	assert (NULL != action_func);
	
	while (from != to && SUCCESS == status)
	{
		from_node = IterToNode(from);
		status = action_func(DlistGetVal(from_node), param);
		from = from->next;
	}
	
	return status;

}

dlist_iter_t DListFind(dlist_iter_t from, dlist_iter_t to, const void* data, int (*dll_is_match) (const void * data, const void* param))
{
	assert (NULL != is_match_func);
	dlist_node* from_node = NULL;
	
	while (from != to)
	{
		from_node = IterToNode(from);
		if (is_match_func(DlistGetVal(from_node), (void*)val))
		{
			return from;
		}
		
		from = from->next;
	}
	
	return to;
}

int DListMultiFind(dlist_iter_t from, dlist_iter_t to, const void* data, 
                        int (*dll_is_match) (const void * data, 
                        const void* param), dlist_t* out_list)
{
	dlist_node* new_node = NULL;
	int status_memory = 0;
	
	while (FOUND == DListIsIterEqual(NodeToIter(new_node = (IterToNode(
	DListFind(from, to, data, dll_is_match, out_list)))), to))
	{
		if (ALOCATE_MEMORY_SUCCESS != (status_memory = DListPushBack(out_list,
		 DlistGetVal(new_node))))
		{
			return status_memory;
		}
		
		from = new_node;
	}
	
	return SUCCESS;
}


