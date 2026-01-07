#ifndef __CIRCULAR_BUFFER_H__
#define __CIRCULAR_BUFFER_H__

#include <stddef.h>   /* size_t */
#include <sys/types.h>/* ssize_t */

typedef struct cbuff cbuff_t;

/**
 * @desc creates a new cbuffer and returns its address
 * @param[in] capacity - the cbuff capacity
 * @pre capacity > 0 
 * @return the address of the new cbuffer or NULL if fails
 * @complexity: O(1)
 */
cbuff_t* CbuffCreate(size_t capacity);

/**
 * @desc destroys a given buffer
 * @param[in] cbuff - the cbuff to destroy
 * @pre cbuff not NULL
 * @returns nothing
 * @complexity: O(1)
 */
void CbuffDestroy(cbuff_t* cbuff);

/**
 * @desc write into cbuff MIN(num_bytes, free_space) from src
 * @param[in] cbuff - the cbuff to fill
 * @param[in] src - what to read from and to copy into cbuff
 * @param[in] num_bytes - the amount to read from src write to cbuff
 * @pre cbuff not NULL
 * @pre src not NULL
 * @return ssize_t - the amount of bytes that was actually written into cbuff from src
 * or -1 on fail
 * @complexity: O(n)
 */
ssize_t CbuffWrite(cbuff_t* cbuff, const void* src, size_t num_bytes);

/**
 * @desc reads at most num_bytes from cbuff to dst
 *       will not read more than written bytes
 * @param[in] cbuff - pointer to cbuff to read from 
 * @param[in] dst - poiter to write into
 * @param[in] num_bytes - number of bytes to read from cbuff
 * @pre cbuff != NULL
 * @pre src != NULL
 * @return ssize_t - number of bytes successfully read from cbuff or -1 on fail
 * @complexity: O(n)
 */
ssize_t CbuffRead(cbuff_t* cbuff, void* dst, size_t num_bytes);

/**
 * @desc empties the buffer 
 * @param[in] cbuff - pointer to the buffer
 * @return void
 * @complexity: O(1)
 */
void CbuffFlush(cbuff_t* cbuff);

/**
 * @desc checks whether the buffer is empty
 * @param[in] cbuff - pointer to the buffer
 * @return 1 if the buffer is empty and 0 otherwise
 * @complexity: O(1)
 */
int CbuffIsEmpty(const cbuff_t* cbuff);

/**@desc returns the capacity of the buffer
 * @param[in] cbuff - the buffer
 * @pre cbuf != NULL
 * @return capacity in bytes
 * @complexity: O(1)
 */
size_t CbuffBufSize(const cbuff_t* cbuff);

/**
 * @desc returns number of bytes available for writing
 * @param[in] cbuff - the buffer
 * @pre cbuf != NULL
 * @return free space in bytes
 * @complexity: O(1)
 */
size_t CbuffGetFreeSpace(const cbuff_t* cbuff);

#endif /* __CIRCULAR_BUFFER_H__ */

