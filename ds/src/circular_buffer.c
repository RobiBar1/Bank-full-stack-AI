/*
Writer:  Robi
Checker: Amir
Date:	 06.01.2026
*/

#include <stddef.h> 		 /* size_t */
#include <string.h>			 /* memmove */
#include "circular_buffer.h" /* our api */

#define MIN_(x, y) (size_t)((x) < (y) ? (x) : (y))
#define MAGIC_NUMNER (0xDEADBEBE)
#define FAILED (-1)
#define SUCCESS (0)

struct cbuff
{
    size_t capacity;
    size_t magic_number;
    size_t read_index;
    size_t write_index;
    void* arr[1];
};


static size_t GetCapacity(cbuff_t* buff)
{
	assert (NULL != buff);
		
    return buff->capacity;
}

static void SetCapacity(size_t new_capacity, cbuff_t* buff)
{
	assert (NULL != buff);
	
    buff->capacity = new_capacity;
}

static size_t GetMegicNumber(cbuff_t* buff)
{
	assert (NULL != buff);
	
    return buff->magic_number;
}

static void SetMegicNumber(size_t new_magic_number, cbuff_t* buff)
{
	assert (NULL != buff);
	
    buff->magic_number = new_magic_number;
}

static size_t GetReadIndex(cbuff_t* buff)
{
	assert (NULL != buff);
	
    return buff->read_index;
}

static void SetReadIndex(size_t new_read_index, cbuff_t* buff)
{
	assert (NULL != buff);
	
    buff->read_index = new_read_index;
}

static size_t GetWriteIndex(cbuff_t* buff)
{
	assert (NULL != buff);
	
    return buff->write_index;
}

static void SetWriteIndex(size_t new_write_index, cbuff_t* buff)
{
	assert (NULL != buff);
	
    buff->write_index = new_write_index;
}

static void* GetArr(cbuff_t* buff)
{
	assert (NULL != buff);
	
    return buff->arr;
}

cbuff_t* CbuffCreate(size_t capacity)
{
	cbuff_t* buff = (cbuff_t*)malloc(sizeof(cbuff_t) * capacity);
	if (NULL == buff)
	{
		return NULL;
	}
	
	SetCapacity(capacity, buff);
	SetMegicNumber(magic_number, buff);
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
	return (GetReadIndex(buff) == GetWriteIndex(buff));
}

size_t CbuffBufSize(const cbuff_t* buff)
{
	return GetCapacity(buff);
}

void CbuffFlush(cbuff_t* buff)
{
	SetWriteIndex(0, buff);	
	SetReadIndex(0, buff);
}

size_t CbuffGetFreeSpace(const cbuff_t* buff)
{
	size_t write = GetWriteIndex(buff);
	size_t read = GetReadIndex(buff);
	size_t can_write_until_end_of_arr = (GetCapacity(buff) + 1) - write;
	size_t can_write_from_begin_arr_until_read = 0;
	size_t can_write_until_read = 0;
	
	if (write == read)
	{
		return GetCapacity(buff);
	}
	else if (write < read)
	{	
		can_write_until_read = read - write - 1;
		
		return MIN_(can_write_until_read, 0);
	}	
	else
	{
		can_write_from_begin_arr_until_read = MIN_(0, read - 1);
		
		return can_write_until_end_of_arr + can_write_from_begin_arr_until_read;
	}		
}

ssize_t CbuffWrite(cbuff_t* buff, const void* src, size_t num_bytes)
{
	size_t bytes_to_write = 0;
	size_t write = GetWriteIndex(buff);
	size_t can_write_until_end_of_arr = (GetCapacity(buff) + 1) - write;
	size_t remain_bytes = 0;
	
	assert (NULL != src);
	
	if (buff->magic_number != magic_number)
	{
		return FAILED;
	}
	
	bytes_to_write = MIN_(CbuffGetFreeSpace(buff), num_bytes);
	if (bytes_to_write <= can_write_until_end_of_arr)
	{
		memmov(((char*)(GetArr(buff))) + write, src, bytes_to_write);
		SetWriteIndex(bytes_to_write + GetWriteIndex(buff), buff);
	}
	else
	{
		memmov(((char*)(GetArr(buff))) + write, src, can_write_until_end_of_arr);
		remain_bytes = (GetCapacity(buff) + 1) - can_write_until_end_of_arr;
		memmov(((char*)GetArr(buff)), ((char*)src) + can_write_until_end_of_arr, remain_bytes);
		SetWriteIndex(remain_bytes, buff);
	}
	
	return SUCCESS; 				
}

ssize_t CbuffRead(cbuff_t* buff, void* dst, size_t num_bytes)
{
	size_t bytes_to_read = 0;
	size_t read = GetReadIndex(buff);
	size_t can_read_until_end_of_arr = (GetCapacity(buff) + 1) - read;
	size_t remain_bytes = 0;
	
	assert (NULL != dst);
	
	if (MAGIC_NUMNER != buff->magic_number)
	{
		return FAILED;
	}
	
	bytes_to_read = MIN_(GetCapacity(buff) - CbuffGetFreeSpace(buff), num_bytes);
	if (bytes_to_read <= can_read_until_end_of_arr)
	{
		memmov(dst, ((char*)(GetArr(buff))) + read, bytes_to_read);
		SetReadIndex(bytes_to_read + + GetReadIndex(buff), buff);
	}
	else
	{
		memmov(dst, ((char*)(GetArr(buff)))) + read, can_read_until_end_of_arr);
		remain_bytes = (GetCapacity(buff) + 1) - can_read_until_end_of_arr;
		memmov(dst + can_read_until_end_of_arr, GetArr(buff), remain_bytes);
		SetReadIndex(remain_bytes, buff);
	}
	
	return SUCCESS; 				
}
