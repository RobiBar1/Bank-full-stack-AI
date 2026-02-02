#ifndef __BST_H__
#define __BST_H__

#include <stddef.h>/*size_t*/

/*
typedef enum {LEFT, RIGHT, NUM_CHILDREN} children_t;

typedef struct bst_node
{
    void* data;
    bst_node_t* parent;
    bst_node_t* children[NUM_CHILDREN];
} bst_node_t;
*/

/*
typedef struct bst
{
    bst_node_t* end;
    int (*cmp)(const void* data, const void* param);
}
*/

typedef struct bst_node* bst_iter_t;
typedef struct bst bst_t;


/**
 * @desc creates a new empty bst
 * @param cmp - compare function for sorting the elements in the list
 * The comparison function must return an integer less than, equal to, or
 * greater than zero if the first argument is considered to be respec‐
 * tively less than, equal to, or greater than the second. If two members
 * compare as equal, their order in the sorted array will be positionally stable.
 * @pre cmp != NULL
 * @returns pointer to the new tree, or NULL if allocation fails
 * @complexity O(1)
 */
bst_t* BSTCreate(int(*cmp)(const void* data, const void* param));

/**
 * @desc - destroys the bst
 * @param[in] bst - the bst to destroy
 * @returns nothing
 * @pre - bst != NULL
 * @time complexity O(n)
 **/
void BSTDestroy(bst_t* bst);

/**
 * @desc insert element in the bst
 * @param[in] bst - the bst to add the element to
 * @param[in] data - the data to insert
 * @pre bst != NULL
 * @return bst_iter_t - the iterator that added to the tree
 * @complexity: average O(logn), worst case O(n)
 **/
bst_iter_t BSTInsert(bst_t* bst, const void* data);

/**
 * @desc removes the current iterator from bst
 * @param[in] iter - the iterator that we would like to remove
 * @pre - iter != the return value of BSTEnd
 * @return bst_iter_t - the next iterator
 * @complexity: Avarage: O(logn) Worst O(n)
 */
bst_iter_t BSTRemove(bst_iter_t iter);

/**
 * @desc checks if two iterators are equal (are the same iter) to each other
 * @param[in] iter1 - the first iterator
 * @param[in] iter2 - the second iterator
 * @pre - iter1, iter2 are not NULL
 * @return int - 1 if they are equal (are the same iter), 0 if they are not equal
 * @complexity: O(1)
 */
int BSTIsIterEqual(const bst_iter_t iter1, const bst_iter_t iter2);

/**
 * @desc returns the smallest element in the bst
 * @param[in] bst_t* - pointer to bst
 * @pre bst != NULL;
 * @return bst_iter_t - the smallest element in the bst
 * @complexity: O(1)
 */
bst_iter_t BSTBegin(const bst_t* bst);

/**
 * @desc returns the element that is after the largest element in the bst
 * @param[in] bst_t* - pointer to bst
 * @pre bst != NULL;
 * @return bst_iter_t - an iterator to the last element of the bst - "bad iterator"
 * contains invalid data, trying to preform API functions on it will result in undefined behavior
 * @complexity: O(1)
 */
bst_iter_t BSTEnd(const bst_t* bst);

/**
 * @desc: returns the previous element  
 * @param[in] the iter pointing to the node we want to get his prev
 * @returns: the iter to the prev node , undefined if the node is the smallest.
 */
bst_iter_t BSTGetPrev(const bst_iter_t iter);

/**
 * @desc: returns the next element in the bst
 * @param[in] the iter pointing to the node we want to the next of
 * @returns: the iter to the next node or BSTEnd if its the end
 * @Complexicity: average O(logn), worst O(n)
 */
bst_iter_t BSTGetNext(const bst_iter_t iter);

/**
 * @desc: returns the data from the given iterator
 * @param[in] the iter pointing to the node we want to get his data
 * @returns: void* contains the data 
 */
void* BSTGetData(const bst_iter_t iter);

/**
 * @desc finds the first occurrence of 'data' in binary
 * @prarm[in] bst - a binary search tree
 * @param[in] data - the data we need to find in the binary sort tree
 * @pre NULL != bst
 * @return an iterator to the first occurrence of 'data' upon success, or the BSTEnd upon failure
 * @Complexity: average O(logn), worst O(n)
 * 
 */
bst_iter_t BSTFind(const bst_t* bst, const void* data);

/**
 * @desc checks if the bst is empty
 * @param[in] bst - pointer to bst
 * @pre bst != NULL
 * @return 1 if empty, 0 if not empty
 * complextiy O(1)
 */
int BSTIsEmpty(const bst_t* bst);

/**
 * @desc counts the amount of elements in the bst
 * @param[in] bst - the bst to count
 * @pre bst != NULL
 * @return size_t - the number of elements in the bst
 * @complexity: O(n)
 */
size_t BSTCount(const bst_t* bst);


/**
 * @desc applies an action over the range [from, to)
 * @param[in] from - an iterator to start run from
 * @param[in] to - an iterator to end run on (not include that iterator == run until 1 itertator before that).
 * @param[in] action - a function which executes a modification on the iterator.
 * action should return 0 upon success or a non-zero value upon failure
 * @param[in] param - what action() should be applied onto the iterator value
 * @pre 'from' and 'to' belong to the same bst
 * @pre 'from' position <= 'to' position in the bst
 * @pre action,bst,from,to != NULL
 * @return 0 - all actions on iterators were successful, non-zero otherwise
 * @complexity: O(n)
 
 */
int BSTForEach(bst_t* bst, int(*action)(void* data, void* param), bst_iter_t from, bst_iter_t to);



#endif /* __BST_H__ */
