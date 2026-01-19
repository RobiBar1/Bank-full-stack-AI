#ifndef __PQUEUE_H__
#define __PQUEUE_H__

#include <stddef.h> /* size_t */

typedef struct pqueue pqueue_t;

/* TODO */
typedef int (*priority_cmp_t) (const void *, const void *);

/**
 * @desc creates a new priority queue and returns its address
 * @return the address of the new priority queue or NULL if fails
 * @pre func != NULL
 * @complexity: O(1)
 */
pqueue_t* PQueueCreate(priority_cmp_t func);

/**
 * @desc frees the allocated space of the priority queue
 * @param[in] queue -  the priority queue to be destroy
 * @pre queue != NULL
 * @return void
 * @complexity: O(n)
 */
void PQueueDestroy(pqueue_t* queue);
/**
 * @desc adds task to the end of the priority queue
 * using the compare function
 * @param[in] queue - the priority queue to add to
 * @param[in] data - the data to add
 * @pre queue not NULL
 * @pre data not NULL
 * @return 0 for successful addition and 1 for fail
 * @complexity: O(1)
 */
int PQueueEnqueue(pqueue_t* queue, const void* data);

/**
 * Description:
 * remove the first element(first will be the next element need go out as priority_cmp_t According to how the function determined) from the priotity queue
 * 
 * param[in]:
 * queue is the queue to remove elemnt from him.
 * 
 * pre:
 * 1. queue not NULL
 * 2. queue not empty
 * 
 * return:
 * 1. return the element that his turn to go out right now by the priority.
 *
 * @complexity: O(1)
 */
void* PQueueDequeue(pqueue_t* queue);

/**
 * @desc - gets the first element in the priority queue
 * @param[in] queue - the queue to look at
 * @pre queue not NULL
 * @pre queue not empty
 * @return void* - a pointer to the first element's value
 * @complexity: O(1)
 */
void* PQueuePeek(const pqueue_t* queue);

/**
 * @desc  count how much elemnts in priority queue, if empty return 0
 * @param[in] queue - the priority queue we want to count her elemnts
 * @pre 'queue' != NULL
 * @return amount of elemnts in the the queue
 * @complexity: O(n) (n being the number of elemnts in the queue)
 **/
size_t PQueueSize(const pqueue_t* queue);

/**
 * @desc if the priority queue is empty, returns 1 else 0
 * @param[in] queue - the priority queue we want to check if empty
 * @pre 'queue' != NULL
 * @return if the priority queue is empty
 * @complexity: O(1) 
 **/
int PQueueIsEmpty(const pqueue_t* queue);

/**
 * @desc removes the matched element from the priotity queue using match func
 * @param[in] queue - the queue to remove from
 * @pre queue not NULL
 * @pre is_match not NULL
 * @pre queue not empty
 * @return void* - the data removed or NULL if not found
 * @complexity: O(n)
 */
void* PQueueRemove(pqueue_t* queue, const void* param, int (*is_match)(const void* data, const void* param));



#endif /* __PQUEUE_H__ */
