#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stddef.h>

typedef struct vector vector_t;

/**
 * @desc inserts a new element to the end of the vector
 * @param[in] vector - vector pointer
 * @param[in] data - pointer to value to be inserted
 * @pre vector != NULL
 * @pre data != NULL
 * @return int - 0 on success , non-zero value on failure 
 */
 
int VectorPushBack(vector_t* vector, const void* data);
/**
 * @desc removes the last element in the vectro
 * param[in] vector - vector pointer
 * @pre vector != NULL
 * @returns nothing
 */
void VectorPopBack(vector_t* vector);
/**
 * @desc get an access to an element in the vector 
 * @param[in] vector_t - vector pointer
 * @param[in] index - index to value to be either edited or used 
 * @pre vector != NULL
 * @pre index < size 
 * @return void* - pointer to the element , undefined on failure 
 */
void* VectorGetAccess(const vector_t* vector, size_t index);
/**
 * @desc creates a new vector and return's its address
 * @param[in] capacity - the capacity of the created vector
 * @param[in] element_size - the size of each element
 * @return nothing
 */
vector_t* VectorCreate(size_t capacity, size_t element_size);
/**
 * @desc frees allocated memory of a vector
 * @param[in] vector - vector pointer
 * @return nothing
 */
void VectorDestroy(vector_t* vector);
/**
 * @desc a simple function that retrives the vector's size 
 * @param[in] vector_t* - the vector which we will return the size from
 * @pre vector != NULL
 * @return size_t - number of elements in the vector
 */
size_t VectorGetSize(const vector_t* vector);

/** @desc gets the capacity of the vector
 * @param[in] vector - vector pointer
 * @pre vector != NULL
 * @return size_t - capacity of the vector
 **/
size_t VectorGetCapacity(const vector_t* vector);
/**
 * @desc  Updates the vector's capacity to the given value if it is larger
 * than the current capacity. If the requested capacity is smaller
 * than the current capacity, the vector remains unchanged.
 * param[in] - vector_t*: pointer to vector
 * param[in] - new_capacity requested new capacity 
 * @pre vector !=NULL
 * @return int - 0 on success , non_zero value on failure
**/
int VectorReserve(vector_t* vector, size_t new_capacity);
/** 
 * @desc shrinks capacity by growth factor
 * @param[in] - vector_t*: pointer to vector
 * @pre vector != NULL
 * @return int - 0 on success , non-zero value on failure 
 */
int VectorShrink(vector_t* vector);


#endif /*__VECTOR_H__*/                                                              
