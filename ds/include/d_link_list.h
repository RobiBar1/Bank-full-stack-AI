#ifndef _DLIST_H_
#define _DLIST_H_

#include <stddef.h> /* size_t */

typedef struct dlist dlist_t;
typedef struct dlist_node* dlist_iter_t;

/**
 * @desc creates a new empty doubly linked list
 * @param none
 * @returns pointer to the new list, or NULL if allocation fails
 * @complexity O(1)
 */
dlist_t* DListCreate(void);
/**
 * @desc destroys a doubly linked list 
 * @param[in] list - the list to destroy
 * @pre list != NULL
 * @returns void
 * @complexity O(n)
 */
void DListDestroy(dlist_t* list);
/**
 * @desc gets the first element of the linked list
 * @param[in] list - a pointer to the linked list
 * @pre list != NULL
 * @return dlist_iter_t - an iterator to the first element of the linked list 
 * @complexity: O(1)
 * */
dlist_iter_t DListBegin(const dlist_t* list);
/**
 * @desc gets the last element of the linked list
 * @param[in] list - doubly linked list
 * @pre list != NULL
 * @return dlist_iter_t - an iterator to the last element of the linked list - "bad iterator".
 * contains invalid data, trying to preform API functions on it will result in failure.
 * @complexity: O(1)
 */
dlist_iter_t DListEnd(const dlist_t* list);
/**
 * @desc gets the iterator after iter
 * @param[in] iter - iterator
 * @pre iter != the return value of DListEnd()
 * @return dlist_iter_t - the next iterator
 * @complexity: O(1)
 */
dlist_iter_t DListGetNext(const dlist_iter_t iter);
/**
 * @desc gets the iterator previous to iter
 * @param[in] iter - iterator
 * @pre iter != start of list (head)
 * @return iter_t - the prev iterator
 * @complexity: O(1)
 */
dlist_iter_t DListGetPrev(const dlist_iter_t iter);
/**
 * @desc checks if iter1 = iter2
 * @param[in] iter1 - the first itertor to compare with
 * @param[in] iter2 - the second itertor to compare with
 * @return 1 if the iterators are equal, otherwise 0
 * @complexity: O(1)
 */
int DListIsIterEqual(const dlist_iter_t iter1, const dlist_iter_t iter2);
/**
 * @desc gets the data of a given iterator position
 * @param[in] iter - iterator to the list
 * @pre iter != the return value of DListEnd()
 * @return void* - a pointer to the data
 * @complexity: O(1)
 */
void* DListGetData(const dlist_iter_t iter);
/**
 * @desc writes data to a given iterator position
 * @param[in] iter - iterator to list
 * @param[in] data - the data to write
 * @pre iter != the return value of DListEnd()
 * @return void
 * @complexity: O(1)
*/
void DListSetData(const dlist_iter_t iter, const void* data);
/**
 * @desc inserts new iter to the list before pos, with val = data
 * @param[in] pos - iterator before which the content will be inserted
 * @param[in] data - data to be written in inserted iteratot
 * * @return dlist_iter_t - iterator to the inserted data upon success or the return value of DListEnd() upon failure
 * @complexity: O(1)
 */
dlist_iter_t DListInsert(dlist_iter_t pos, const void* data);
/**
 * @desc removes the current iterator from list
 * @param[in] iter - the iterator that we would like to remove
 * @pre - iter != the return value of DListEnd()
 * @return dlist_iter_t - the iterator after removed iter
 * @complexity: O(1)
 */
dlist_iter_t DListRemove(dlist_iter_t iter);
/**
 * @desc inserts a new iter at the beginning of the dlist
 * @param[in] list - pointer to the list to insert to 
 * @param[in] data - pointer to value to be inserted
 * @pre list != NULL
 * @return int - 0 upon success and 1 upon failure
 * @complexity: O(1)
 */
int DListPushFront(dlist_t* list, const void* data);
/**
 * @desc inserts a new iter at the end of the dlist
 * @param[in] list - pointer to the list to insert to
 * @param[in] data - pointer to value to be inserted
 * @pre list != NULL
 * @return int - 0 upon success and 1 upon failure
 * @complexity: O(1)
 */
int DListPushBack(dlist_t* list, const void* data);
/**
 * @desc removes the first element of the list and returns its value
 * @param[in] list - doubly linked list pointer
 * @pre list != NULL
 * @return the value of the extracted value
 * @complexity: O(1)
 */
void* DListPopFront(dlist_t* list);
/**
 * @desc removes the last element in the list and returns its value
 * @param[in] list - doubly linked list pointer
 * @pre list isn't empty
 * @return the value of the extracted value
 * @complexity: O(1)
 */
void* DListPopBack(dlist_t* list);
/**
 * @desc desc finds first instance of value in the range [from, to)
 * @param[in] from - iterator to start the search from
 * @param[in] to - iterator to end the search on
 * @param[in] data - value to search for
 * @param[in] dll_is_match - a comparison function for data (returns 1 if it is a match, 0 otherwise)
 * @pre dll_is_match != NULL 
 * @pre 'from' and 'to' belong to the same slist
 * @pre 'from' appears before (or is equal to) 'to' in slist
 * @return dlist_iter_t - an iterator to the matched data, or the to upon failure
 * @complexity: O(n)
 * */
dlist_iter_t DListFind(dlist_iter_t from, dlist_iter_t to, const void* data, int (*dll_is_match) (const void * data, const void* param));
/**
 * @desc finds all instances of value in the range [from, to)
 * @param[in] from - iterator to start the search from
 * @param[in] to - iterator to end the search on
 * @param[in] data - value to search for
 * @param[in] dll_is_match - a comparison function for val (returns 1 if it is a match, 0 otherwise).
 * @param[out] out_list - list of iterators to the values that were matched from first to last. 
 * @pre dll_is_match != NULL 
 * @pre out_list != NULL 
 * @pre 'from' and 'to' belong to the same slist
 * @pre 'from' appears before (or is equal to) 'to' in slist
 * @return int - 0 upon success and 1 upon failure
 * @complexity: O(n)
 * */
int DListMultiFind(dlist_iter_t from, dlist_iter_t to, const void* data, 
                        int (*dll_is_match) (const void * data, const void* param),
                        dlist_t* out_list);
/**
 * @desc applies an action over the range [from, to)
 * @param[in] from - starting iterator
 * @param[in] to - ending iterator (doesn't apply on it, meaning, not including)
 * @param[in] dll_action_func - the function executing the modification to the iterator value is 0 - on success, [other] - on failure
 * @param[in] param - what dll_action_func applies onto the iterator value
 * @pre 'from' and 'to' belong to the same slist
 * @pre 'from' appears before (or is equal to) 'to' in slist
 * @pre action_func != NULL
 * @return 0 - all action_func on iterators were successful, non-zero otherwise
 * @complexity: O(n) x
 */
int DListForEach(dlist_iter_t from, dlist_iter_t to, int (*dll_action_func)(void* data, void* param), void* param);
/**
 * @desc removes a range of element [from, to) from one list and inserts them before where of another list
 * after splicing, the element previous to from in the original list will be previous to the element after to
 * @param[in] from - iterator to the first element to move
 * @param[in] to - iterator to the last element to move (excluded)
 * @param[in] where - iterator to the element to move the elements before
 * @pre 'from' and 'to' belong to the same slist
 * @pre 'from' appears before (or is equal to) 'to' in slist
 * @return nothing
 * @complexity: O(1)
 */ 
void DListSplice(dlist_iter_t from, dlist_iter_t to, dlist_iter_t where);
/**
 * @desc counts the amount of elements in the list
 * @param[in] list - the list to count
 * @pre list - a doubly linked list
 * @return size_t - the number of elements in the list
 * @complexity: O(n)
 */
size_t DListCount(const dlist_t* list);
/**
 * @desc checks if the list is empty or not
 * @param[in] list - pointer to the doubly linked list
 * @pre NULL != list
 * @return 1 if the list is empty, otherwise 0
 * @complexity: O(1)
 */
int DListIsEmpty(const dlist_t* list);

#endif /* _DLIST_H_ */
