#include <assert.h> /* assert  */
#include <stdlib.h> /* malloc */

#include "vsa.h" 	/* our api */

/*-------------------- Sign numbers ---------------------------------*/
#define MAGIC_NUMBER (0xDEADBEEF)
#define MAGIC_NUMBER_END (0xDEADBEBE)
/*-------------------- Sizes ----------------------------------------*/
#define META_STRUCT_SIZE_IN_BYTES (sizeof(meta_data_t))
#define WORD_SIZE (sizeof(size_t))
/*-------------------- Calc things-----------------------------------*/
#define BLOCK_NOT_FREE(tmp) !(tmp & FLAG_BYTES_FREE)
#define MIN_FREE_SPACE_BYTES (META_STRUCT_SIZE_IN_BYTES + (2 * WORD_SIZE))
/*-------------------- Magic numbers in code ------------------------*/
#define FLAG_BYTES_FREE (1)
#define ALL_MEM (NULL)
/*-------------------- Return macros --------------------------------*/
#define SUCCESS (0)
#define END_OF_MEMORY (1)

typedef struct meta
{
	size_t size;
	#ifndef NDEBUG
	size_t sign;
	#endif
}meta_data_t;

/*--------------------------- Start Helper Functions -------------------------*/
static int IsMemFree(const meta_data_t* metadata)
{
	return metadata->size & FLAG_BYTES_FREE;
}

static meta_data_t* GetNext(const meta_data_t* metadata)
{
	return (meta_data_t*)((char*)(metadata) + META_STRUCT_SIZE_IN_BYTES
	 	   + metadata->size - IsMemFree(metadata));
}

static size_t AlignSizeUp(size_t num_bytes)
{
	assert (0 < num_bytes);
	
	if (!(num_bytes & (sizeof(size_t) - 1)))
	{
		return num_bytes;
	}
	
	return ((sizeof(size_t) + num_bytes - (num_bytes & (sizeof(size_t) - 1))));
}

static size_t* GetNextFreeBlock(size_t** runner)
{
	assert (NULL != runner);
	assert (NULL != *runner);
	
	while (MAGIC_NUMBER_END != **runner)
	{
		if (BLOCK_NOT_FREE(**runner))
		{
			*runner = (size_t*)GetNext((meta_data_t*)(*runner));
		}
		else
		{
			return *runner;
		}
	}
	
	*runner = NULL;
	
	return *runner;
}

static int DeFregUntilBlock(size_t** first_free_block,
						    size_t** second_free_block)
{	
	*second_free_block = (size_t*)GetNext((meta_data_t*)(*first_free_block));
	while (MAGIC_NUMBER_END != **second_free_block)
	{
		if (BLOCK_NOT_FREE(**second_free_block))
		{
			return SUCCESS;
		}
		
		**first_free_block += **second_free_block - FLAG_BYTES_FREE
							 + sizeof(meta_data_t);
		*second_free_block = (size_t*)
							 GetNext((meta_data_t*)(*second_free_block));
	}
	
	return END_OF_MEMORY;
}

static vsa_t DeFreg(vsa_t vsa, size_t* num_bytes)
{
	size_t* first_free_block = NULL;
	size_t* second_free_block = NULL;
	size_t* tmp = NULL;
	
	assert (NULL != vsa);
	
	first_free_block = (size_t*)vsa;
	GetNextFreeBlock(&first_free_block);
	if(NULL == num_bytes)
	{
		while (NULL != first_free_block
		 	   && !DeFregUntilBlock(&first_free_block, &second_free_block))
		{
			first_free_block = second_free_block;
			GetNextFreeBlock(&first_free_block);
		}
	}
	else
	{	
		while (NULL != first_free_block)
		{
			if (((*first_free_block) + sizeof(meta_data_t)) > *num_bytes)
			{
				return first_free_block;
			}
			else
			{
				tmp = (size_t*)GetNext((meta_data_t*)(first_free_block));
				if (MAGIC_NUMBER_END == *tmp)
				{
					return NULL;
				}
				else if (BLOCK_NOT_FREE(*tmp))
				{
					first_free_block = tmp;
					GetNextFreeBlock(&first_free_block);
				}
				else
				{
					*first_free_block += *tmp - 1 + sizeof(meta_data_t);
				}
			}
		} 
	}
	
	return vsa;
}

/*--------------------------- End Helper Functions -------------------------*/

/*--------------------------- Start H.file Functions -------------------------*/

vsa_t VSAInit(void* memory_pool, size_t num_bytes)
{
	meta_data_t meta = {0};
	
	assert (NULL != memory_pool);
	assert (num_bytes >= MIN_FREE_SPACE_BYTES);
	assert (!(((size_t)memory_pool) & (sizeof(size_t) - 1)));
	
	meta.size = (num_bytes - META_STRUCT_SIZE_IN_BYTES - WORD_SIZE) | FLAG_BYTES_FREE;
	#ifndef NDEBUG
	meta.sign = MAGIC_NUMBER;
	#endif
	
	*((meta_data_t*)memory_pool) = meta;
	*(size_t*)((char*)memory_pool + num_bytes - WORD_SIZE) = MAGIC_NUMBER_END;  
	 
	return (vsa_t)(memory_pool);
}

void* VSAAlloc(vsa_t vsa, size_t num_bytes)
{
	size_t* runner = (size_t*)vsa;
	size_t* addres_to_return = NULL;
	size_t last_num = 0;
	meta_data_t meta = {0};
	
	assert (NULL != vsa);
	assert (0 < num_bytes);
	
	num_bytes = AlignSizeUp(num_bytes);
	DeFreg(vsa, ALL_MEM);
	
	while (num_bytes > 0)
	{
		GetNextFreeBlock(&runner);
		if (NULL == runner)
		{
			return NULL;
		}
		else if (num_bytes <= *runner)
		{
			if (*runner - num_bytes - FLAG_BYTES_FREE <= sizeof(meta_data_t))
			{
				--(*runner);
				return ((void*)((char*)runner + sizeof(meta_data_t)));
			}
			else
			{
				last_num = *runner;
				*runner = num_bytes;
				addres_to_return = runner;
				runner = (size_t*)GetNext((meta_data_t*)(runner));
				meta.size = last_num - num_bytes - sizeof(meta_data_t);
				#ifndef NDEBUG
				meta.sign = MAGIC_NUMBER;
				#endif
				*(meta_data_t*)runner = meta;
				
				return (void*)((char*)addres_to_return + sizeof(meta_data_t));
			}
		}
	}
	
	return NULL;
}

size_t VSALargestFreeChunk(vsa_t vsa)
{
	size_t* runner = NULL;
	size_t max = 0;
	size_t is_mem_free = 0;
	
	assert (NULL != vsa);
	
	DeFreg(vsa, ALL_MEM);
	runner = (size_t*)vsa;
	GetNextFreeBlock(&runner);
	while (NULL != runner)
	{
		is_mem_free = (size_t)IsMemFree((meta_data_t*)runner);
		if (is_mem_free && max < *runner - is_mem_free)
		{
			max = *runner - is_mem_free;
		}
		
		runner = (size_t*)GetNext((meta_data_t*)runner);
		GetNextFreeBlock(&runner);
	}
	
	return max;
}

void VSAFree(void* allocated_memory)
{
	assert (NULL != allocated_memory);
	
	*(size_t*)((char*)allocated_memory - sizeof(meta_data_t)) = 
	(*((size_t*)((char*)allocated_memory - sizeof(meta_data_t)))) | 1;
}

/*--------------------------- End H.file Functions ---------------------------*/
