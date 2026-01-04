#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stddef.h> /* size_t */

typedef struct queue queue_t;

/**
 * @desc frees the allocated space of the queue
 * @param[in] queue -  the queue to be freed
 * @pre queue != NULL
 * @return void
 * @complexity: O(n)
 */
void QueueDestroy(queue_t* queue);

/**
 * @desc creates a new queue and returns its address
 * @return the address of the new queue or NULL if fails
 * @complexity: O(1)
 */
queue_t* QueueCreate(void);

/**
 * @desc adds data to the end of the queue
 * @param[in] queue - the queue add to
 * @param[in] data - the data to add
 * @pre queue not NULL
 * @pre data not NULL
 * @return 0 for successful addition and 1 for fail
 * @complexity: O(1)
 */
int QueueEnqueue(queue_t* queue, void* data);

/**
 * @desc remove the first element from the queue
 * @param[in] queue - the queue that to remove from
 * @pre queue not NULL
 * @pre queue not empty
 * @return void
 * @complexity: O(1)
 */
void QueueDequeue(queue_t* queue);

/**
 * @desc gets the last element in the queue
 * @param[in] queue - the queue to look at
 * @pre queue not NULL
 * @pre queue not empty
 * @return void* - a pointer to the last elements value
 * @complexity: o(1)
 */
void* QueuePeek(const queue_t* queue);

/**
 * @desc  count how much elemnts in queue, if empty return 0
 * @param[in] queue - the queue we want to count her elemnts
 * @pre 'queue' != NULL
 * @return amount of elemnts in the the queue
 * @complexity: O(n) (n being the number of elemnts in the queue)
 **/
size_t QueueSize(const queue_t* queue);

/**
 * @desc appends src queue to dst queue, resulting in dst-src queue
 * src is an empty queue after appending
 * @param[in] dst - the queue to appended to
 * @param[in] src - the queue that is appended 
 * @pre 'dst' != NULL
 * @pre 'src' != NULL
 * @return void
 * @complexity: O(1)
 */
void QueueAppend(queue_t* dst, queue_t* src);

/**
 * @desc checks whether the list is empty 
 * @param[in] queue - pointer to the queue
 * @return 1 if the queue is empty and 0 otherwise
 * @complexity: O(1)
 */
int QueueIsEmpty(const queue_t* queue);

#endif /* __QUEUE_H__ */
