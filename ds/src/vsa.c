#include <assert.h> /* assert  */
#include <stdlib.h> /* malloc */

#include "vsa.h" 	/* our api */

#define BLOCK_NOT_FREE(tmp) !(tmp & FLAG_BYTES_FREE)
#define META_STRUCT_SIZE (sizeof(meta_data_t))
#define WORD_SIZE (sizeof(size_t))
#define MIN_FREE_SPACE_BYTES (META_STRUCT_SIZE + (2 * WORD_SIZE))
#define FLAG_BYTES_FREE (1)
#define MAGIC_NUMBER (0xDEADBEEF)
#define MAGIC_NUMBER_END (0xDEADBEBE)
#define NEXT_META_PLACE_FROM_USED(runner) \
					   ((runner) + sizeof(meta_data_t) + (*(runner)))
#define NEXT_META_PLACE_FROM_FREE(runner) \
					   ((runner) + sizeof(meta_data_t) \
					    + (*(runner)) - FLAG_BYTES_FREE)
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
static size_t AlignSizeUp(size_t num_bytes)
{
	assert (0 < num_bytes);
	
	if (!(num_bytes & (sizeof(size_t) - 1)))
	{
		return num_bytes;
	}
	
	return ((sizeof(size_t) + num_bytes - (num_bytes & (sizeof(size_t) - 1))));
}

static size_t* GetNextFreeBlock(size_t* vsa, size_t** runner)
{
	assert (NULL != vsa);
	assert (NULL != runner);
	assert (NULL != *runner);
	
	while (MAGIC_NUMBER_END != **runner)
	{
		if (BLOCK_NOT_FREE(**runner))
		{
			*runner = NEXT_META_PLACE_FROM_USED(*runner);
		}
		else
		{
			return *runner;
		}
	}
	
	return NULL;
}

static int DeFregUntilBlock(size_t** first_free_block,
						    size_t** second_free_block)
{	
	*second_free_block = NEXT_META_PLACE_FROM_FREE(*first_free_block);
	while (MAGIC_NUMBER_END != **second_free_block)
	{
		if (BLOCK_NOT_FREE(**second_free_block))
		{
			return SUCCESS;
		}
		
		**first_free_block += **second_free_block - FLAG_BYTES_FREE
							 + sizeof(meta_data_t);
		*second_free_block = NEXT_META_PLACE_FROM_FREE(*second_free_block);
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
	GetNextFreeBlock((size_t*)vsa, &first_free_block);
	if(NULL == num_bytes)
	{
		while (NULL != first_free_block
		 	   && !DeFregUntilBlock(&first_free_block, &second_free_block))
		{
			GetNextFreeBlock(first_free_block, &first_free_block);
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
				tmp = NEXT_META_PLACE_FROM_FREE(first_free_block);
				if (MAGIC_NUMBER_END == *tmp)
				{
					return NULL;
				}
				else if (BLOCK_NOT_FREE(*tmp))
				{
					GetNextFreeBlock(tmp, &first_free_block);
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
	
	meta.size = (num_bytes - META_STRUCT_SIZE - WORD_SIZE) | FLAG_BYTES_FREE;
	#ifndef NDEBUG
	meta.sign = MAGIC_NUMBER;
	#endif
	
	*((meta_data_t*)memory_pool) = meta;
	*((size_t*)((char*)memory_pool) + num_bytes - WORD_SIZE) = MAGIC_NUMBER_END;  
	 
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
	while (num_bytes > 0)
	{
		DeFreg(vsa, &num_bytes);
		GetNextFreeBlock((size_t*)vsa, &runner);
		if (NULL == runner)
		{
			return NULL;
		}
		else if (num_bytes <= *runner)
		{
			if (*runner - num_bytes - FLAG_BYTES_FREE <= sizeof(meta_data_t))
			{
				--(*runner);
				return runner + sizeof(meta_data_t);
			}
			else
			{
				last_num = *runner;
				*runner = num_bytes;
				addres_to_return = runner;
				runner = NEXT_META_PLACE_FROM_USED(runner);
				meta.size = last_num - num_bytes - sizeof(meta_data_t)
				 - FLAG_BYTES_FREE;
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
	
	assert (NULL != vsa);
	
	runner = (size_t*)vsa;
	GetNextFreeBlock((size_t*)vsa, &runner);
	while (NULL != runner)
	{
		if (max < *runner && !(BLOCK_NOT_FREE(*runner)))
		{
			max = *runner;
		}
		
		runner = NEXT_META_PLACE_FROM_FREE(runner);
		GetNextFreeBlock(runner, &runner);
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
