#include <assert.h> /* assert  */

#include "fsa.h"    /* our api */

static size_t FSAGetAllingedChunkSize(size_t chunk_size)
{
	assert (0 < chunk_size);
	
	if (0 == (chunk_size & (sizeof(size_t) - 1)))
	{
		return chunk_size;
	}
	
	return ((sizeof(size_t) + chunk_size - (chunk_size & (sizeof(size_t) - 1))));
}

size_t FSASuggestSize(size_t chunk_size, size_t num_chunks)
{	
	assert (0 < chunk_size);
	
	return FSAGetAllingedChunkSize(chunk_size) * num_chunks + sizeof(size_t);
}

static size_t* FSAGetAdress(void* memory_pool, size_t i
							 ,size_t num_bytes_per_chunk)
{
	return (size_t*)((((char*)memory_pool) + 
					(i * num_bytes_per_chunk + sizeof(size_t))));
				;
}

fsa_t FSAInit(size_t chunk_size, size_t num_chunks, void* memory_pool)
{
	size_t* curent_adress = NULL;
	size_t num_bytes_per_chunk = 0;
	size_t i = 0;
	
	assert (NULL != memory_pool);
	assert (0 == ((size_t)memory_pool & (sizeof(size_t) - 1)));
	assert (0 < chunk_size);
	
	(*((size_t*)memory_pool)) = (size_t)(((size_t*)memory_pool) + 1);
	num_bytes_per_chunk = FSAGetAllingedChunkSize(chunk_size);
	for (; i < num_chunks - 1; ++i)
	{
		curent_adress = FSAGetAdress(memory_pool, i, num_bytes_per_chunk); 
		*curent_adress = (size_t)(FSAGetAdress
						 (memory_pool, i + 1, num_bytes_per_chunk));
	}
	
	curent_adress = FSAGetAdress(memory_pool, i, num_bytes_per_chunk);
	*curent_adress = (size_t)memory_pool;
	
	return (fsa_t)memory_pool;
}

static char* ConvertFSATForHisType(fsa_t memory_pool)
{
	return ((char*)memory_pool);
}

size_t FSACountFreeChunks(fsa_t memory_pool)
{
	size_t* cast_memory_pool = 
	(size_t*)(*(size_t*)ConvertFSATForHisType(memory_pool));
	size_t counter = 0;
	
	assert (NULL != cast_memory_pool);
	
	while (cast_memory_pool != memory_pool)
	{
		cast_memory_pool = (size_t*)(*cast_memory_pool);
		++counter;
	}
	
	return counter;
}

void* FSAAlloc(fsa_t memory_pool)
{
	size_t* cast_memory_pool = (size_t*)ConvertFSATForHisType(memory_pool);
	void* result = NULL;
	
	assert (NULL != cast_memory_pool);
	
	if (((size_t)cast_memory_pool) == (*cast_memory_pool))
	{
		return NULL;
	}
	
	result = (void*)(*((size_t*)cast_memory_pool));
	*cast_memory_pool = (*((size_t*)result));
	
	return result;
}

void FSAFree(void* alloc_mem, fsa_t mem_pool)
{
	size_t* mem_pool_cast = (size_t*)ConvertFSATForHisType(mem_pool);
	
	assert (NULL != alloc_mem);
	assert (NULL != mem_pool_cast);
	
	*((size_t*)alloc_mem) = *mem_pool_cast;
	*mem_pool_cast = (size_t)alloc_mem;
}
