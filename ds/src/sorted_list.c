/*
Writer:  Robi
Checker: Chen San
Data:    10.01.2026
*/

#include <assert.h>      /* assert       */
#include <stdlib.h>      /* malloc, free */

#include "sorted_list.h" /* our api      */

#define UNUSED(X) (void)(X)

typedef int (*cmp)(const void* one, const void* other);

struct sorted_list
{
	cmp func;
	dlist_t* list;
};

static dlist_iter_t SorterIterToDListIter(sorted_list_iter_t s_iter)
{
	return s_iter.internal_iter;
}

static sorted_list_iter_t DListIterToSorterIter(dlist_iter_t d_iter, dlist_t* d_list) 
{
	sorted_list_iter_t sorted_iter = {0};
	
	#ifndef NDEBUG
	assert (NULL != d_list);
	sorted_iter.internal_list = d_list;
	#else
	UNUSED(d_list);
	#endif
	
	sorted_iter.internal_iter = d_iter;
	
	return sorted_iter;
}

sorted_list_t* SortedLCreate(int (*cmp)(const void* one, const void* other))
{
	sorted_list_t* new_list = NULL;
	
	assert (NULL != cmp);
	
	new_list = (sorted_list_t*)malloc(sizeof(sorted_list_t));
	if (NULL == new_list)
	{
		return NULL;
	}
	
	new_list->func = cmp;
	new_list->list = DListCreate();
	
	if (NULL == new_list->list)
	{
		free(new_list); new_list = NULL;
		
		return NULL;
	}
	
	return new_list;
}

void SortedLDestroy(sorted_list_t* list)
{
	assert (NULL != list);
	
	DListDestroy(list->list);
	list->list = NULL;
	
	free(list); list = NULL;
}

int SortedLIsEmpty(const sorted_list_t* list)
{
	assert (NULL != list);
	
	return DListIsEmpty(list->list);
}

size_t SortedLCount(const sorted_list_t* list)
{
	assert (NULL != list);

	return DListCount(list->list);
}

int SortedLIsIterEqual(sorted_list_iter_t one, sorted_list_iter_t other)
{
	
	#ifndef NDEBUG
	assert (one.internal_list == other.internal_list);
	#endif
	
	return DListIsIterEqual(SorterIterToDListIter(one),
							SorterIterToDListIter(other));
}

sorted_list_iter_t SortedLBegin(const sorted_list_t* list)
{
	assert (NULL != list);
	
	return DListIterToSorterIter(DListBegin(list->list), list->list);
}

sorted_list_iter_t SortedLEnd(const sorted_list_t* list)
{
	assert (NULL != list);
	
	return DListIterToSorterIter(DListEnd(list->list), list->list);
}

sorted_list_iter_t SortedLNext(sorted_list_iter_t iter)
{
	dlist_t* internal_list = NULL;
	
	#ifndef NDEBUG
	internal_list = iter.internal_list;
	#endif
	
	return DListIterToSorterIter(DListGetNext(SorterIterToDListIter(iter)), 
								 internal_list);
}

sorted_list_iter_t SortedLPrev(sorted_list_iter_t iter)
{
	dlist_t* internal_list = NULL;
	
	#ifndef NDEBUG
	internal_list = iter.internal_list;
	#endif
	
	return DListIterToSorterIter(DListGetPrev(SorterIterToDListIter(iter)), 
								 internal_list);
}

sorted_list_iter_t SortedLInsert(sorted_list_t* list, const void* data)
{
	dlist_iter_t runner = NULL;
	dlist_iter_t end = NULL;
	
	assert (NULL != list);
	
	runner = DListBegin(list->list);
	end = DListEnd(list->list);
	
	while (!DListIsIterEqual(runner, end) && 
	       0 >= list->func(DListGetData(runner), data))
	{
		runner = DListGetNext(runner);
	}
	
	return DListIterToSorterIter(DListInsert(runner, data), list->list);
}

sorted_list_iter_t SortedLRemove(sorted_list_iter_t iter)
{
	dlist_t* internal_list = NULL;

	#ifndef NDEBUG
	internal_list = iter.internal_list;	
	#endif

	return DListIterToSorterIter(DListRemove(SorterIterToDListIter(iter)), 
								 internal_list);
}

void* SortedLGetData(sorted_list_iter_t iter)
{
	return DListGetData(SorterIterToDListIter(iter));
}

void* SortedLPopFront(sorted_list_t* list)
{
	sorted_list_iter_t first = {0};
	void* data = NULL;
	
	assert (NULL != list);
	
	first = SortedLBegin(list);
	data = SortedLGetData(first);
	SortedLRemove(first);
	
	return data;
}

void* SortedLPopBack(sorted_list_t* list)
{
	sorted_list_iter_t last = {0};
	void* data = NULL;
	
	assert (NULL != list);
	
	last = SortedLPrev(SortedLEnd(list));
	data = SortedLGetData(last);
	SortedLRemove(last);
	
	return data;
}

sorted_list_iter_t SortedLFind(const sorted_list_t* list,
                               sorted_list_iter_t from,
                               sorted_list_iter_t to,
                               const void* data_to_find)
{
	int cmp_result = 0;
	
	assert (NULL != list);
	#ifndef NDEBUG
	assert (from.internal_list == to.internal_list);
	#endif
	
	while (!SortedLIsIterEqual(from, to))
	{
		cmp_result = list->func(SortedLGetData(from), data_to_find);
		
		if (0 == cmp_result)
		{
			return from;
		}
		
		from = SortedLNext(from);
	}
	
	return to;
}

int SortedLForEach(sorted_list_iter_t from,
                   sorted_list_iter_t to,
                   int (*action)(void* data, void* param),
                   void* param)
{	
	assert (NULL != action);
	
	#ifndef NDEBUG
	assert (from.internal_list == to.internal_list);
	#endif
	
	return DListForEach(SorterIterToDListIter(from), 
					    SorterIterToDListIter(to), action, param);
}

sorted_list_iter_t SortedLFindIf(sorted_list_iter_t from,
                                 sorted_list_iter_t to,
                                 int (*match)(const void* data,
                                			  const void* param),
                                 const void* data)
{
	dlist_t* internal_list = NULL;
	
	assert (NULL != match);
	
	#ifndef NDEBUG
	assert (from.internal_list == to.internal_list);
	internal_list = from.internal_list;
	#endif
	
	return DListIterToSorterIter(DListFind(SorterIterToDListIter(from),
	 									   SorterIterToDListIter(to),
										   data, match),
								 internal_list);
}

void SortedLMerge(sorted_list_t* dst, sorted_list_t* src)
{
	assert (NULL != dst);
	assert (NULL != src);
	
	while (!SortedLIsEmpty(src))
	{
		sorted_list_iter_t dst_iter = SortedLBegin(dst);
		sorted_list_iter_t dst_end = SortedLEnd(dst);
		sorted_list_iter_t from = SortedLBegin(src);
		sorted_list_iter_t to = from;
		
		while (!SortedLIsIterEqual(dst_iter, dst_end) &&
		       0 < dst->func(SortedLGetData(from), SortedLGetData(dst_iter)))
		{
			dst_iter = SortedLNext(dst_iter);
		}
		
		while (!SortedLIsIterEqual(to, SortedLEnd(src)) &&
		       (SortedLIsIterEqual(dst_iter, dst_end) ||
		        0 < dst->func(SortedLGetData(dst_iter), SortedLGetData(to))))
		{
			to = SortedLNext(to);
		}
		
		DListSplice(from.internal_iter, to.internal_iter, dst_iter.internal_iter);
	}
}
