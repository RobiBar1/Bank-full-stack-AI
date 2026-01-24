#ifndef __FSA_H__
#define __FSA_H__

#include <stddef.h>     /* size_t */

typedef void* fsa_t;


/**
 * @desc returns the suggested size for the memory pool in bytes
 * @return the size of memeory pool
 * @param[in] chunk_size  the chunk size in bytes
 * @param[in] num_chunks  number of chunks
 * @pre chunk_size > 0
 * @complexity: O(1)
 */
size_t FSASuggestSize(size_t chunk_size, size_t num_chunks);

/**
 * @desc - Prepare fixed sized memory for allocation
 * @return the new memory pool
 * @param[in] chunk_size - size of chunks 
 * @param[in] num_chunks - number of chunks
 * @param[in] memory_pool - memory to manage
 * @pre memory_pool is aligned
 * @pre memory_pool is not null (add it in main editor?)
 * @pre user: memory_pool size must be the return value of FSASuggestSize 
 * @pre chunk_size > 0
 * @complexity: O(n)
 */
fsa_t FSAInit(size_t chunk_size, size_t num_chunks, void* memory_pool);

/**
 * @desc counts the number of free chunks in memory_pool
 * @return the number of free chunks in memory_pool
 * @param[in] memory_pool  the memory pool we count free chunks from
 * @pre NULL != memory_pool
 * @complexity: O(N) (N = chuck_numbers)
 */
size_t FSACountFreeChunks(fsa_t memory_pool);

/**
 * @desc finds a free chunk 
 * @return pointer to free chunk start or null if nor free memory
 * @param[in] memory_pool the FSA to find the free chunck in
 * @pre memory_pool != NULL
 * @complexity: O(1)
 */
void* FSAAlloc(fsa_t memory_pool);

/**
 * @desc free the allocated_memory from memory_pool
 * @param[in] allocated_memory to free
 * @param[in] memory_pool the FSA to remove from
 * @pre allocated_memory not NULL
 * @pre memory_pool not NULL
 * @pre undefined: calling free twice or more for the same chunk
 * @complexity: O(1)
 */
void FSAFree(void* allocated_memory, fsa_t memory_pool);

#endif /* __FSA_H__ */
