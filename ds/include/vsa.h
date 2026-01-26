#ifndef __VSA_H__
#define __VSA_H__

#include <stddef.h>     /* size_t */

typedef void* vsa_t;

/**
 * @desc Initialize a Variable Size Allocator
 * @param memory_pool Pointer to memory buffer to manage
 * @param num_bytes size of the memory buffer in bytes
 * @return Initialized VSA handle
 * @pre memory_pool != NULL
 * @pre memory_pool is aligned to word boundary
 * @pre num_bytes >= 4 * (word_size)
 * @complexity O(1)
 **/
vsa_t VSAInit(void* memory_pool, size_t num_bytes);
/**
 * @desc finds a free address with number bytes available 
 * @return pointer to the start of free byte, or null if failed to find free memory
 * @param[in] vsa is a free address with number bytes available 
 * @param[in] num_bytes: number of bytes
 * @pre vsa != NULL
 * @complexity: O(n)
 */
void* VSAAlloc(vsa_t vsa, size_t num_bytes);

/**
 * @desc - frees the given memory
 * @returns nothing
 * param[in] - address of memory to be freed
 * @pre allocated memory not already freed
 * @time complexity - O(1)
**/
void VSAFree(void* allocated_memory);



/**
 * @desc - calculates the largest available free chunk
 * @returns size_t - largest available free chunk
 * param[in] - pointer to memory buffer to manage
 * @pre vsa != NULL
 * @time complexity - O(n)
**/
size_t VSALargestFreeChunk(vsa_t vsa);



#endif /* __VSA_H__ */
