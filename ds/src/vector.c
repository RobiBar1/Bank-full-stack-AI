/*
writer:  Robi
Checker: Hadar
Date:    31.12.2025
*/

#include <stdlib.h> /* free */
#include <string.h> /* memcpy */
#include <assert.h> /* assert */

#include "vector.h" /* our api */

#define SUCCESS (0)
#define NOT_SUCCESS (1)
#define GROWTH_FACTOR (2) 
#define MIN_(x,y) ((x) > (y) ? (y) : (x))

struct vector
{
	size_t curr_size_of_elemnts;
	size_t capacity;
	size_t element_size_in_bytes;
	char* arr;
};

static int VectorResize(vector_t* vec, size_t new_capacity)
{
	char* tmp = NULL;
	
	assert(NULL != vec);
	
	tmp = (char*)realloc(vec->arr, new_capacity * vec->element_size_in_bytes);
	if (NULL != tmp)
	{
		vec->arr = tmp;
		vec->capacity = new_capacity;
		
		return SUCCESS; 
	}
	
	return NOT_SUCCESS;
}

vector_t* VectorCreate(size_t capacity, size_t element_size)
{
	vector_t* vec = NULL;
	
	vec = (vector_t*)malloc(sizeof(vector_t));
	if (NULL == vec)
	{
		return NULL;
	}
	
	vec->arr = (char*)malloc(capacity * element_size);
	if (NULL == vec->arr)
	{
		free(vec);
		return NULL;
	}
	
	vec->curr_size_of_elemnts = 0;
	vec->capacity = capacity;
	vec->element_size_in_bytes = element_size;
	
	return vec;
}

int VectorPushBack(vector_t* vec, const void* data)
{
	
	assert(NULL != vec);
	assert(NULL != data);
	
	if (vec->curr_size_of_elemnts == vec->capacity)
	{
		if (SUCCESS != VectorResize(vec, vec->capacity * GROWTH_FACTOR))
		{
			return NOT_SUCCESS;
		}
	}
	
	memcpy(vec->arr + vec->curr_size_of_elemnts * vec->element_size_in_bytes, data, vec->element_size_in_bytes);
	++(vec->curr_size_of_elemnts);
	
	return SUCCESS;
}

void VectorPopBack(vector_t* vec)
{
	assert(NULL != vec);
	assert(0 != vec->curr_size_of_elemnts);
	
	--(vec->curr_size_of_elemnts);
}

void* VectorGetAccess(const vector_t* vec, size_t index)
{
	assert(vec);
	assert(index < vec->curr_size_of_elemnts);
	
	return (void*)(vec->arr + index * vec->element_size_in_bytes);
}

void VectorDestroy(vector_t* vec)
{
	free(vec->arr);
	free(vec);
}

size_t VectorGetSize(const vector_t* vec)
{
	return vec->curr_size_of_elemnts;
}

size_t VectorGetCapacity(const vector_t* vec)
{
	return vec->capacity;
}

int VectorReserve(vector_t* vec, size_t new_capacity)
{
	assert(NULL != vec);
	
	if (vec->capacity > new_capacity)
	{
		return SUCCESS;
	}
	
	return VectorResize(vec, new_capacity);
}

int VectorShrink(vector_t* vec)
{
	size_t new_capacity = 0;
	
	assert(NULL != vec);
	
	new_capacity = MIN_(vec->curr_size_of_elemnts * GROWTH_FACTOR + 1, vec->capacity);
	
	return VectorResize(vec, new_capacity);
}
