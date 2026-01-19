#ifndef _SORTED_LIST_H_
#define _SORTED_LIST_H_

#include <stddef.h>      /* size_t */

#include "d_link_list.h" /* our d_l_l api */

typedef struct 
{
    dlist_iter_t internal_iter;
    #ifndef NDEBUG
    dlist_t* internal_list;
    #endif
} sorted_list_iter_t;

typedef struct sorted_list sorted_list_t;

/**
 * @desc - creates a list whose elements are sorted according to a specified comparison function
 * comparison is done by key.
 * @param[in] cmp - compare function for sorting the elements in the list
 * The comparison function must return an integer less than, equal to,  or
 * greater than zero if the first argument is considered to be respec‐
 * tively less than, equal to, or greater than the second. If two members
 * compare as equal, their order in the sorted array will be positionally stable.
 * @pre cmp != NULL
 * @return sorted_list_t* - a pointer to a sorted list upon success, NULL otherwise
 * @time complexity - O(1)
*/
sorted_list_t* SortedLCreate(int (*cmp)(const void* one, const void* other));
/**
 * @desc destroys a sorted linked list 
 * @param[in] list - the list to destroy
 * @pre list != NULL
 * @returns void
 * @complexity O(n)
 */
void SortedLDestroy(sorted_list_t* list);
/**
 * @desc checks if the sorted_list is empty
 * @param[in] list - a pointer to the sorted_list
 * @pre list != NULL
 * @return 1 if the list is empty, otherwise 0
 * @complexity: O(1)
 */
int SortedLIsEmpty(const sorted_list_t* list);
/**
 * @desc - counts the number of elements in the list
 * @param[in] list - a sorted list pointer
 * @pre list != NULL
 * @return the number of elements
 * complexity O(n)
 */
size_t SortedLCount(const sorted_list_t* list);
/**
 * @desc checks if both iterators are equal
 * @param[in] one - the first itertor to compare with
 * @param[in] other - the second itertor to compare with
 * @return 1 if the iterators are equal, otherwise 0
 * @complexity: O(1)
 */
int SortedLIsIterEqual(sorted_list_iter_t one, sorted_list_iter_t other);
/**
 * @desc - gets the first element of the sorted list or the return value of SortedLEnd() if list is empty
 * @param[in] list - sorted list pointer
 * @pre list != NULL
 * @return sorted_list_iter_t - iterator to the first element of the list
 * @complexity O(1)
 */
sorted_list_iter_t SortedLBegin(const sorted_list_t* list);
/**
 * @desc - gets the element after the last valid element of the sorted list
 * @param[in] list - sorted list pointer
 * @pre list != NULL
 * @return sorted_list_iter_t - an iterator to the element after the last valid element of the sorted list - "bad iterator".
 * contains invalid data, trying to preform API functions on it will result in failure.
 * @complexity O(1)
 */
sorted_list_iter_t SortedLEnd(const sorted_list_t* list);
/**
 * @desc returns the next iter 
 * @param[in] iter - iterator to an element of the list
 * @pre iter != the return value of SortedLEnd()
 * @return sorted_list_iter_t - iterator to the next element after iter element
 * @complexity: O(1)
 */
sorted_list_iter_t SortedLNext(sorted_list_iter_t iter);
/**
 * @desc returns the previous iter.
 * @param[in] iter - iterator to an element of the list
 * @pre iter != the return value of SortedLBegin().
 * @return sorted_list_iter_t - iterator to the previous element before iter element
 * @complexity: O(1)
 */
sorted_list_iter_t SortedLPrev(sorted_list_iter_t iter);
/**
 * @desc inserts a new element to the sorted list, which contains the specified data 
 * insertion maintains a sorting logic (refer to SortedLCreate()) 
 * @param[in] list - the sorted list that the data will be inserted to
 * @param[in] data - data to be written in the inserted iterator
 * @return sorted_list_iter_t - iterator to the inserted elemnet upon success or the return value of SortedListEnd() upon failure
 * @complexity: O(n)
 */
sorted_list_iter_t SortedLInsert(sorted_list_t* list, const void* data);
/**
 * @desc removes the current iterator from list
 * @param[in] iter - the iterator to remove
 * @pre - iter != the return value of SortedListEnd()
 * @return sorted_list_iter_t - the iterator positioned after the removed iter
 * @complexity: O(1)
 */
sorted_list_iter_t SortedLRemove(sorted_list_iter_t iter);
/**
 * @desc gets the data contained in a specified iterator position
 * @param[in] iter - iterator to the list
 * @pre iter != the return value of SortedListEnd()
 * @return void* - a pointer to the data 
 * @complexity: O(1)
 */
void* SortedLGetData(sorted_list_iter_t iter);
/**
 * @desc removes the first element of the list and returns its value
 * @param[in] list - a sorted list pointer
 * @pre list != NULL
 * @pre list isn't empty
 * @return the value of the extracted element
 * @complexity: O(1)
 */
void* SortedLPopFront(sorted_list_t* list);
/**
 * @desc removes the last element of the list and returns its value
 * @param[in] list - a sorted list pointer
 * @pre list != NULL
 * @pre list isn't empty
 * @return the value of the extracted element
 * @complexity: O(1)
 */
void* SortedLPopBack(sorted_list_t* list);
/**
 * @desc finds the first occurrence of 'data_to_find' in range [from, to)
 * @prarm[in] list - a sorted list pointer
 * @param[in] from - iterator to start the search from
 * @param[in] to - iterator to end the search on
 * @param[in] data_to_find - the data we need to find in the sorted list
 * @pre NULL != list
 * @pre NULL != from 
 * @pre NULL != to
 * @pre from and to need to be from the same sorted list
 * @return the first occurrence iterator the function found as succes, 
 * return 'to' iterator if the function failed
 * @Complexicity: O(n)
 * 
 */
sorted_list_iter_t SortedLFind(const sorted_list_t* list,
                               sorted_list_iter_t from,
                               sorted_list_iter_t to,
                               const void* data_to_find);
/**
 * @desc desc finds the value of the first matched instance in the range [from, to) according to a specified comparison function
 * @param[in] from - an iterator to start the search from
 * @param[in] to - an iterator to end the search on
 * @param[in] data - a value to search for
 * @param[in] match - a comparison function for data (returns 1 if it is a match, 0 otherwise)
 * @pre match != NULL 
 * @pre 'from' and 'to' belong to the same list
 * @pre 'from' position <= 'to' position in list
 * @return dlist_iter_t - an iterator to the matched data, or the return value of SortedLEnd() upon failure
 * @complexity: O(n)
 * */
sorted_list_iter_t SortedLFindIf(sorted_list_iter_t from,
                                 sorted_list_iter_t to,
                                int (*match)(const void* data, const void* param),
                                const void* param);
                                
/**
 * @desc applies an action over the range [from, to)
 * @param[in] from - an iterator to start run from
 * @param[in] to - an iterator to end run on 
 * @param[in] action - a function which executes a modification on the iterator.
 * action should return 0 upon success or a non-zero value upon failure
 * @param[in] param - what action() should be applied onto the iterator value
 * @pre 'from' and 'to' belong to the same sorted_list
 * @pre 'from' position <= 'to' position in list
 * @pre action != NULL
 * @return 0 - all actions on iterators were successful, non-zero otherwise
 * @complexity: O(n)
 */
int SortedLForEach(sorted_list_iter_t from,
                   sorted_list_iter_t to,
                   int (*action)(void* data, void* param),
                   void* param);
/**
 * @desc Merges two sorted lists into dst list, and empties src list
 * @param[in] dst - sorted list that the lists will be merged into
 * @param[in] src - sorted list, will be valid and empty at the end of function
 * @pre src and dst share the same comparison logic (refer to SortedLCreate())
 * @pre 'dst' != NULL
 * @pre 'src' != NULL  
 * @complexity: O(n)
 */
void SortedLMerge(sorted_list_t* dst, sorted_list_t* src);

#endif /* _SORTED_LIST_H_ */
