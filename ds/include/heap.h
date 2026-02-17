/* max heap */

#ifndef __HEAP_H__
#define __HEAP_H__

#include <stddef.h> /* size_t */
#include "vector.h" /* vector */

/*
struct heap
{
    int (*cmp)(const void* left ,const void* right);
    vector_t* vector;
};
*/

typedef struct heap heap_t;


/**
 * @brief creates a new empty heap
 * @param capacity - initial size of heap vector
 * @param cmp - compare function for comparing the elements
 *              return 0 if equal, less than 0 if left < right , more than 0 otherwise 
 * @pre cmp != NULL
 * @returns pointer to the new heap, or NULL on failure.
 * 
 * complexity O(1)
 */
heap_t* HeapCreate(size_t capacity, int (*cmp)(const void* data, const void* param));

/**
 * @brief destroy the heap
 * @param heap - the heap to destroy
 * @pre NULL != heap
 * 
 * complexity O(1)
 */
void HeapDestroy(heap_t* heap);

/**
 * @brief insert the element to the heap, saves data by reference.
 * @param[in] heap - the heap to insert the element.
 * @pre heap not NULL   
 * @return 0 if success and non zero other-wise.
 * 
 * @complexity: O(log(n))
 */
int HeapPush(heap_t* heap, const void* data);

/**
 * @brief - remove data from the heap
 * @param[in] heap - the heap to remove from
 * @param[in] data - the data we remove from the heap
 * @param[in] is_match - the function returns 1 if data matches
 * @pre - heap != NULL
 * @pre - is_match != NULL
 * @return data that remove
 * 
 * complexity O(n)
 **/
void* HeapRemove(heap_t* heap, const void* data, int (*is_match)(const void* data, const void* param));

/**
 * @brief checks if the heap is empty
 * @param[in] heap - the heap
 * @pre heap != NULL
 * @return 1 if empty, 0 if not empty
 * 
 * complextiy O(1)
 */
int HeapIsEmpty(const heap_t* heap);
/**
 * @brief counts the number of elements in the heap
 * @param[in] heap - the heap to count the elements from
 * @pre NULL != heap
 * @complexity: O(1)
 */
size_t HeapCount(const heap_t* heap);
/**
 * @brief remove the top element from the heap.
 * @param[in] heap - the heap to take his top element out.
 * @pre heap not NULL
 * @pre heap not empty
 * 
 * @complexity: O(log(n))
 */
void HeapPop(heap_t* heap);

/**
 * @brief gets the top element in the heap
 * @param[in] heap - the heap to look at
 * @pre heap not NULL
 * @pre heap not empty
 * @return void* - a pointer to the top elements value
 * 
 * @complexity: O(1)
 */
void* HeapPeek(const heap_t* heap);

#endif /* __HEAP_H__ */