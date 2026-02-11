#ifndef __AVL_H__
#define __AVL_H__

#include <stddef.h>/*size_t*/


/*

typedef enum {LEFT, RIGHT, NUM_CHILDREN} children_t;

typedef struct avl_node
{
	void* data;
    struct avl_node* children[NUM_CHILDREN];
    int balance_factor;
} avl_node_t;


struct avl
{
    avl_node_t* dummy;
    int (*cmp)(const void* data, const void* param);
};
*/

/* ALL DATA IN THE TREE IS UNIQUE */

typedef enum {POST, PRE, IN} traversal_ord_t;

typedef struct avl avl_t;

/**
 * @desc creates a new empty avl
 * @param cmp - compare function for sorting the elements in the list
 * The comparison function must return an integer less than, equal to, or
 * greater than zero if the first argument is less than, equal to, or greater than the second. 
 * @pre cmp != NULL
 * @returns pointer to the new avl tree, or NULL on failure.
 * @complexity O(1)
 */
avl_t* AVLCreate(int (*cmp)(const void* first, const void* second)); 

/**
 * @desc - destroys the avl
 * @param[in] avl - the avl to destroy
 * @pre - avl != NULL
 * @complexity O(n)
 **/
void AVLDestroy(avl_t* avl);

/**
 * @desc insert element in the avl, cannot insert the same data twice
 * @param[in] avl - the avl to add the element to
 * @param[in] data - the data to insert
 * @pre avl != NULL
 * @pre data is not already in the avl
 * @return int - 0 if successed or 1 if failed. 
 * @complexity: O(log n)
 **/
int AVLInsert(avl_t* avl, const void* data);

/**
 * @desc removes the data from the avl
 * @param[in] avl - the avl
 * @param[in] data - data to remove from the tree
 * @pre avl != NULL
 * @pre data to remove is in the avl
 * @complexity: O(log n)
 */
void AVLRemove(avl_t* avl, const void* data);

/**
 * @desc checks if the avl is empty
 * @param[in] avl_t - the avl
 * @pre avl != NULL
 * @return 1 if empty, 0 if not empty
 * complextiy O(1)
 */
int AVLIsEmpty(const avl_t* avl);

/**
 * @desc counts the amount of elements in the avl
 * @param[in] avl - the avl
 * @pre avl != NULL
 * @return size_t - the number of elements in the avl
 * @complexity: O(n)
 */
size_t AVLCount(const avl_t* avl);

/**
 * @desc returns the height of the avl (from the root)
 * @param[in] avl - the avl
 * @pre avl != NULL
 * @return size_t - the height of the avl
 * @complexity: O(n)
 */
long AVLHeight(const avl_t* avl);

/**
 * @desc finds data in the avl
 * @param[in] avl - the avl
 * @param[in] data - data to find in the avl
 * @pre avl != NULL
 * @return the data found of data is in the avl, NULL otherwise
 * @complexity: O(log n)
 */
void* AVLFind(const avl_t* avl, const void* data);


/**
 * @desc applies an action over the avl
 * @param[in] avl - the avl
 * @param[in] action - a function which executes a function on the avl.
 * action should return 0 upon success or a non-zero value upon failure
 * @param[in] param - what action() should be applied onto the avl
 * @param[in] traversal_ord - the order the avl is traversed on.
 * @pre avl != NULL
 * @pre action != NULL
 * @return 0 - all actions on avl were successful, non-zero otherwise
 * @complexity: O(log n)
 */
int AVLForEach(const avl_t* avl, int (*action)(const void* data, void* param), void* param, traversal_ord_t traversal_ord);

/*  Advanced  */

/**
 * @desc Finds all the elements from arr in the avl and puts the data in out_arr
 * @param[in] avl - the avl
 * @param[in] arr - an array of data to find
 * @param[out] out_arr - an array to put the found datas
 * @pre avl != NULL
 * @pre arr != NULL
 * @pre out_arr != NULL
 * @return nothing
 * @complexity: O(nlog n)
 */
void AVLMultiFind(const avl_t* avl, const void* arr, void* out_arr);

/**
 * @desc removes all the elements from arr in the avl
 * @param[in] avl - the avl
 * @param[in] arr - an array of data to find
 * @pre avl != NULL
 * @pre arr != NULL
 * @return nothing
 * @complexity: O(nlog n)
 */
void MultiRemove(const avl_t* avl, const void* arr);


#endif /* __AVL_H__ */          
