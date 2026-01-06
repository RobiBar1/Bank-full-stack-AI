/*
Writer:  Robi
Checker: Amir
Date:	 06.01.2026
*/

#include <stddef.h>          /* size_t, offsetof */
#include <string.h>          /* memmove 		 */
#include <stdlib.h>          /* malloc, free     */
#include <assert.h>          /* assert 			 */

#include "circular_buffer.h" /* our api			 */

#define MIN_(x, y) (size_t)((x) < (y) ? (x) : (y))
#define MAGIC_NUMBER (0xDEADBEBE)
#define FAILED (-1)
#define SUCCESS (0)

struct cbuff
{
    size_t capacity;
    size_t magic_number;
    size_t read_index;
    size_t write_index;
    char arr[1];
};


static size_t GetCapacity(cbuff_t* buff)
{
    assert(NULL != buff);
    
    return buff->capacity;
}

static void SetCapacity(size_t new_capacity, cbuff_t* buff)
{
    assert(NULL != buff);
    
    buff->capacity = new_capacity;
}

static size_t GetMagicNumber(cbuff_t* buff)
{
    assert(NULL != buff);
    
    return buff->magic_number;
}

static void SetMagicNumber(size_t new_magic_number, cbuff_t* buff)
{
    assert(NULL != buff);
    
    buff->magic_number = new_magic_number;
}

static size_t GetReadIndex(cbuff_t* buff)
{
    assert(NULL != buff);
    
    return buff->read_index;
}

static void SetReadIndex(size_t new_read_index, cbuff_t* buff)
{
    assert(NULL != buff);
    
    buff->read_index = new_read_index;
}

static size_t GetWriteIndex(cbuff_t* buff)
{
    assert(NULL != buff);
    
    return buff->write_index;
}

static void SetWriteIndex(size_t new_write_index, cbuff_t* buff)
{
    assert(NULL != buff);
    
    buff->write_index = new_write_index;
}

static char* GetArr(cbuff_t* buff)
{
    assert(NULL != buff);
    
    return buff->arr;
}

cbuff_t* CbuffCreate(size_t capacity)
{
    cbuff_t* buff = (cbuff_t*)malloc(offsetof(struct cbuff, arr) + capacity);
    if (NULL == buff)
    {
        return NULL;
    }
    
    SetCapacity(capacity, buff);
    SetMagicNumber(MAGIC_NUMBER, buff);
    SetReadIndex(0, buff);
    SetWriteIndex(0, buff);
    
    return buff;
}

void CbuffDestroy(cbuff_t* buff)
{
    free(buff); buff = NULL;
}

int CbuffIsEmpty(const cbuff_t* buff)
{
    return (GetReadIndex((cbuff_t*)buff) == GetWriteIndex((cbuff_t*)buff));
}

size_t CbuffBufSize(const cbuff_t* buff)
{
    return GetCapacity((cbuff_t*)buff);
}

void CbuffFlush(cbuff_t* buff)
{
    SetWriteIndex(0, buff);
    SetReadIndex(0, buff);
}

size_t CbuffGetFreeSpace(const cbuff_t* buff)
{
    return GetCapacity((cbuff_t*)buff) - GetWriteIndex((cbuff_t*)buff) + GetReadIndex((cbuff_t*)buff);
}

ssize_t CbuffWrite(cbuff_t* buff, const void* src, size_t num_bytes)
{
    size_t free_space = 0;
    size_t bytes_to_write = 0;
    size_t capacity = 0;
    size_t write_pos = 0;
    size_t first_chunk = 0;
    size_t second_chunk = 0;
    
    assert(NULL != src);
    
    if (MAGIC_NUMBER != GetMagicNumber(buff))
    {
        return FAILED;
    }
    
    free_space = CbuffGetFreeSpace(buff);
    bytes_to_write = MIN_(num_bytes, free_space);
	capacity = GetCapacity(buff);
    write_pos = GetWriteIndex(buff) % capacity;
    first_chunk = MIN_(bytes_to_write, capacity - write_pos);
    second_chunk = bytes_to_write - first_chunk;

    
    memmove(GetArr(buff) + write_pos, src, first_chunk);
    memmove(GetArr(buff), (const char*)src + first_chunk, second_chunk);
    SetWriteIndex(GetWriteIndex(buff) + bytes_to_write, buff);
    
    return (ssize_t)bytes_to_write;
}

ssize_t CbuffRead(cbuff_t* buff, void* dst, size_t num_bytes)
{
    size_t available_data = 0;
    size_t bytes_to_read = 0;
    size_t capacity = 0;
    size_t read_pos = 0;
    size_t first_chunk = 0;
    size_t second_chunk = 0;
    
    assert(NULL != dst);
    
    if (MAGIC_NUMBER != GetMagicNumber(buff))
    {
        return FAILED;
    }
    
    available_data = GetWriteIndex(buff) - GetReadIndex(buff);
    bytes_to_read = MIN_(num_bytes, available_data);
	capacity = GetCapacity(buff);
    read_pos = GetReadIndex(buff) % capacity;
    first_chunk = MIN_(bytes_to_read, capacity - read_pos);
    second_chunk = bytes_to_read - first_chunk;

    
    memmove(dst, GetArr(buff) + read_pos, first_chunk);
	memmove((char*)dst + first_chunk, GetArr(buff), second_chunk);
    SetReadIndex(GetReadIndex(buff) + bytes_to_read, buff);
    
    return (ssize_t)bytes_to_read;
}
