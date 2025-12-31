/*
writer:  Robi
Checker: Hadar
Date:    31.12.2025
*/

#include <stdlib.h> /* free */
#include <string.h> /* memcpy */
#include <assert.h> /* assert */

#include "vector.h" /* our api */

#define INIT0 0
#define SUCCESS 0
#define NOT_SUCCESS -1
#define GROWTH_FACTOR 2 
#define MIN_(x,y) ((x) > (y) ? (y) : (x))

struct vector
{
	size_t curr_size_of_elemnts;
	size_t capacity;
	size_t element_size_in_bytes;
	char* arr;
};

vector_t* VectorCreate(size_t capacity, size_t element_size)
{
	vector_t* vec = (vector_t*)malloc(sizeof(vector_t));
	if (NULL == vec)
	{
		return NULL;
	}
	
	vec->arr = (char*)malloc(capacity * element_size);
	if (NULL == vec->arr)
	{
		return NULL;
	}
	
	vec->curr_size_of_elemnts = INIT0;
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
		if (0 != VectorReserve(vec, vec->capacity * GROWTH_FACTOR))
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
	
	if (0 == vec->curr_size_of_elemnts)
	{
		return;
	}
	
	--(vec->curr_size_of_elemnts);
}

void* VectorGetAccess(const vector_t* vec, size_t index)
{
	assert(vec);
	
	return (void*)(vec->arr + index * vec->element_size_in_bytes);
}

void VectorDestroy(vector_t* vec)
{
	free(vec->arr); vec->arr = NULL;
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
	
	vec->arr = (char*)realloc(vec->arr,vec->element_size_in_bytes * new_capacity);
	if (NULL == vec->arr)
	{
		return NOT_SUCCESS;
	}
	
	vec->capacity = new_capacity;
	return SUCCESS;
}

int VectorShrink(vector_t* vec)
{
	size_t new_capacity = INIT0;
	
	assert(NULL != vec);
	
	new_capacity = MIN_(vec->curr_size_of_elemnts * GROWTH_FACTOR + 1, vec->capacity);
	vec->arr = (char*)realloc(vec->arr, new_capacity);
	
	if (NULL == vec->arr)
	{
		return NOT_SUCCESS;
	}
	
	vec->capacity = new_capacity;
	return SUCCESS;	
}
