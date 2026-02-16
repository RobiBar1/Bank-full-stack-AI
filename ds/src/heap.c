#include <assert.h> /* assert       */
#include <stdlib.h> /* malloc       */

#include "heap.h"   /* our api      */

#define SUCCESS 0
#define NOT_SUCCESS 1

#define GET_PARENT(i) (((i) - 1) / 2)
#define GET_LEFT_CHILD(i) ((i) * 2 + 1)
#define GET_RIGHT_CHILD(i) ((i) * 2 + 2)

static void HeapifyDownRecursive(heap_t* heap, size_t my_index, size_t capacity_last_index);

struct heap
{
    int (*cmp)(const void* left ,const void* right);
    vector_t* vector;
};

heap_t* HeapCreate(size_t capacity, int (*cmp)(const void* data, const void* param))
{
    heap_t* heap = NULL;

    assert(NULL != cmp);

    heap = (heap_t*)malloc(sizeof(heap_t));
    if (NULL == heap)
    {
        return NULL;
    }

    heap->vector = VectorCreate(capacity, sizeof(size_t));
    if (NULL == heap->vector)
    {
        free(heap); heap = NULL;
        return NULL;
    }

    heap->cmp = cmp;

    return heap;
}

void HeapDestroy(heap_t* heap)
{
    assert (NULL != heap);

    VectorDestroy(heap->vector);
    free(heap); heap = NULL;
}

int HeapIsEmpty(const heap_t* heap)
{
    assert (NULL != heap);

    return (0 == VectorGetSize(heap->vector));
}

size_t HeapCount(const heap_t* heap)
{
    assert (NULL != heap);

    return VectorGetSize(heap->vector);
}

void* HeapPeek(const heap_t* heap)
{
    assert (NULL != heap);

    return *(void**)VectorGetAccess(heap->vector, 0);
}

static void Swap(void** left, void** right)
{
    void* tmp = NULL;

    assert (NULL != left);
    assert (NULL != right);

     tmp = *left;
     *left = *right;
     *right = tmp;
}

/*@pre index >= 0*/
static void HeapifyUp(heap_t* heap, size_t index)
{
    void** current = NULL;
    void** parent = NULL;

    assert (NULL != heap);

    current = VectorGetAccess(heap->vector, index);
    parent = (0 >= index ? current : VectorGetAccess(heap->vector, GET_PARENT(index)));
    while (0 < heap->cmp(*current, *parent))
    {
        Swap(current, parent);
        index = GET_PARENT(index);
        parent = (0 == index ? current : VectorGetAccess(heap->vector, GET_PARENT(index)));
    }
}

int HeapPush(heap_t* heap, const void* data)
{
    assert (NULL != heap);

    if (SUCCESS != VectorPushBack(heap->vector, (void*)(&data)))
    {
        return NOT_SUCCESS;
    }

    HeapifyUp(heap, VectorGetSize(heap->vector) - 1);

    return SUCCESS;
}

static size_t GetBigerIndex(heap_t* heap, size_t index_left, size_t index_right)
{
    assert(NULL != heap);

    if (0 < heap->cmp(*(void**)VectorGetAccess(heap->vector, index_left), *(void**)VectorGetAccess(heap->vector, index_right)))
    {
        return index_left;
    }

    return index_right;
}

static void CheckIfKeepHeapifyDown(heap_t* heap, size_t my_index, size_t other_inndex, size_t capacity_last_index)
{
    assert (NULL != heap);

    if (my_index != GetBigerIndex(heap, my_index, other_inndex))
    {
        Swap((void**)VectorGetAccess(heap->vector, my_index), (void**)VectorGetAccess(heap->vector, other_inndex));
        HeapifyDownRecursive(heap, my_index, capacity_last_index);
    }
}


static void HeapifyDownRecursive(heap_t* heap, size_t my_index, size_t capacity_last_index)
{
    size_t index_left = 0;
    size_t index_right = 0;
    size_t bigger_side = 0;

    assert (NULL != heap);
    
    index_left = GET_LEFT_CHILD(my_index);
    if (capacity_last_index <= index_left)
    {
        return;
    }
    
    index_right = GET_RIGHT_CHILD(my_index);
    if (capacity_last_index > index_right)
    {
        bigger_side = GetBigerIndex(heap, index_left, index_right);
        CheckIfKeepHeapifyDown(heap, my_index, bigger_side, capacity_last_index);

        return;
    }

    CheckIfKeepHeapifyDown(heap, my_index, index_left, capacity_last_index);
}

void HeapPop(heap_t* heap)
{
    assert (NULL != heap);

    if (VectorGetCapacity(heap->vector))
    {
        Swap(VectorGetAccess(heap->vector, 0), VectorGetAccess(heap->vector, VectorGetSize(heap->vector) - 1));
    }

    VectorPopBack(heap->vector);
    HeapifyDownRecursive(heap, 0, VectorGetSize(heap->vector));
}

static void* Find(vector_t* head, size_t from, size_t to, size_t* element_index,
     const void* data ,int (*is_match)(const void* data, const void* param))
{
    void* element_to_remove = NULL;

    assert (NULL != head);
    assert (NULL != is_match);

    for (; from < to; ++from)
    {
        element_to_remove = *(void**)VectorGetAccess(head, from);
        if(is_match(data, element_to_remove))
        {
            *element_index = from;

            return element_to_remove;
        }
    }

    return element_to_remove;
}

void* HeapRemove(heap_t* heap, const void* data, int (*is_match)(const void* data, const void* param))
{   
    void* element_to_remove = NULL;
    size_t element_index = 0;

    assert (NULL != heap);
    assert (NULL != is_match);

    element_to_remove = Find(heap->vector, element_index, VectorGetSize(heap->vector), &element_index, data, is_match);
    *(void**)VectorGetAccess(heap->vector, element_index) = *(void**)VectorGetAccess(heap->vector, VectorGetSize(heap->vector) - 1);
    VectorPopBack(heap->vector);

    if (0 < element_index && element_index < VectorGetSize(heap->vector) &&
     0 < heap->cmp(*(void**)VectorGetAccess(heap->vector, element_index),
    *(void**)VectorGetAccess(heap->vector, GET_PARENT(element_index))))
    {
        HeapifyUp(heap, element_index);
    }
    else
    {
        HeapifyDownRecursive(heap, element_index, VectorGetSize(heap->vector));
    }
    
    return element_to_remove;
}