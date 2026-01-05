#ifndef __SINGLY_LINKED_LIST_H__
#define __SINGLY_LINKED_LIST_H__

#include <stddef.h> /* size_t */

typedef struct slist slist_t;
typedef struct slist_node* slist_iter_t;

typedef int (*action_func_t)(void* data, void* param);
typedef int (*match_func_t)(const void* data, void* param);

/**
 * @desc creates a new Singly linked list and returns it's address
 * @return the address to the new allocated vector :) or NULL if failes :( 
 */
slist_t* SListCreate(void);
/**
 * @desc frees allocated memory of a singly linked list
 * @param[in] list_t* - list pointer
 * @return nothing
 * time complexity: O(n)
 */
void SListDestroy(slist_t* list);
/**
 * @desc inserts new element before pos
 * @param[in] list - list_t pointer in case of changing the head
 * @param[in] pos - iterator before which the content will be inserted
 * @param[in] data - data to insert
 * @pre list != NULL
 * @return slist_iter_t - iterator pointing to the inserted data
 * @complexity O(1)
 */
slist_iter_t SListInsert(slist_t* list, slist_iter_t pos, const void* data);
/**
 * @desc removes the current iterator
 * @param[in] iter - the iterator that we would like to remove
 * @return slist_iter_t - iterator pointing to the data
 */
slist_iter_t SListRemove(slist_iter_t iter);

/**
 * @desc desc finds first instance of value between from and to not including to
 * @param[in] from - iterator to start the search from
 * @param[in] to - iterator to end the search on
 * @param[in] val - value to search for
 * @param[in] cmp_function - a comparison function for val (returns 1 if it is a match, zero otherwise)
 * @pre val != NULL
 * @pre cmp_func != NULL 
 * @pre from must come before to and they must be in the same list
 * @return itr_t - an iterator to the value, or to on failure
 * @time complexity: O(n)
 * */
slist_iter_t SListFind(slist_iter_t from, slist_iter_t to, const void* val, match_func_t is_match_func);
/**
 * @desc get the head of the linked list
 * @param[in] list - pointer to the linked list
 * @return iter_t - an iterator to the head of the linked list
 * time complexicity: O(1)
 * */
slist_iter_t SListBegin(const slist_t* list);
/**
 * @desc Returns an iterator past the last element of list
 * @param[in] slist_t - single linked list
 * @pre list != NULL
 * @return iter_t - an iterator past the last element, not part of the list elements
 * time complexicity: O(1)
 */
slist_iter_t SListEnd(const slist_t* list);
/**
 * @desc get the next iterator 
 * @param[in] from - iterator
 * @pre iter != end of list
 * @return iter_t - return the next iterator
 * */
slist_iter_t SListNext(slist_iter_t iter);
/**
 * @desc get the number of elements in the list.
 * @param[in] list - Single Link List.
 * @pre list != NULL
 * @return size_t - return the amount of elements.
 * */
size_t SListCount(const slist_t* list);
/**
 * @desc checks if the list is empty or not
 * @param[in] list - pointer to the singly linked list
 * @return 1 if the list is empty and 0 otherwise
 * time complexity: O(1)
 */
int SListIsEmpty(const slist_t* slist);
/**
 * @desc gets the data from a given iterator position
 * @param[in] iter - iterator pointing to the node contain data
 * @pre iter is vaild (not NULL, not end)
 * @return pointer to the data
 */
void* SListGetData(slist_iter_t iter);

/**
 * @desc Sets/updates the data at the given iterator position
 * @param[in] iter - iterator pointing to the node to update
 * @param[in] data - pointer to the new data to set
 * @pre iter is valid (not NULL, not End)
 * @return void
 */
void SListSetData(slist_iter_t iter, const void* data);

/**
 * @desc applies an action over a range
 * @param[in] from - starting iterator
 * @param[in] to - ending iterator (doesn't apply on it, meaning, not including)
 * @param[in] action_func - the function executing the modification to the iterator value
 *                          has 0 - on success, [other] - on failure
 * @param[in] param - what action_func applies onto the iterator value
 * @pre 'from' and 'to' belong to the same slist
 * @pre 'from' appears before (or is equal to) 'to' in slist
 * @pre action_func != NULL
 * @return 0 - all action_func on iterators were successful 1, - otherwise
 */
int SListForEach(slist_iter_t from, slist_iter_t to, action_func_t action_func, void* param);

/**
 * @desc applies an action over a range
 * @param[in] iter1 - the first itertor to compare with
 * @param[in] iter2 - the second iterator to compare with
 * @return 1 if both iterators are equal, 0 otherwise
 * time complexicity: O(1)
 */
int SListIsIterEqual(slist_iter_t iter1, slist_iter_t iter2);

void SListAppend(slist_t* dst, slist_t* src);

#endif /* __SINGLY_LINKED_LIST_H__ */
