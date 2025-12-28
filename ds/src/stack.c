#include <stdio.h>  /* printf */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */
#include <string.h> /* memcpy */

#include "stack.h"  /* our api */

#define INIT0 0

struct stack
{
	size_t max_capacity;
	size_t cur_size;
	size_t elemnt_size;
	void* arr;
};

stack_t* StackCreate(size_t capacity, size_t elemnt_size)
{
	stack_t* stack = (stack_t*)malloc
	(elemnt_size * capacity + sizeof(stack_t));
	
	if (NULL == stack)
	{
		return NULL;
	}
	
	stack->max_capacity = capacity;
	stack->cur_size = INIT0;
	stack->arr = stack + 1;
	stack->elemnt_size = elemnt_size;
	
	return stack;
}

void StackDestroy(stack_t* stack)
{
	assert(NULL != stack);
	
	free(stack);
}

void StackPush(stack_t* stack, const void* data)
{	
	assert(NULL != stack);
	assert(NULL != data);
	assert(stack->max_capacity > stack->cur_size);
	
	memcpy(stack->arr, data, stack->elemnt_size);
	++(stack->cur_size);
	
	stack->arr =
	(unsigned char*)stack->arr +
	stack->elemnt_size;
	 

}

int StackIsEmpty(const stack_t* stack)
{
	assert(NULL != stack);
	
	return (0 == stack->cur_size);
}

void StackPop(stack_t* stack)
{
	assert(NULL != stack);
	assert(!StackIsEmpty(stack));
	
	--(stack->cur_size);
	stack->arr = (unsigned char*)stack->arr - 
	stack->elemnt_size;
}

size_t StackSize(const stack_t* stack)
{
	assert(NULL != stack);
	
	return stack->cur_size;
}

size_t StackCapacity(const stack_t* stack)
{
	assert(NULL != stack);
	
	return stack->max_capacity;
} 

void StackPeek(const stack_t* stack, void* dest)
{
	assert(NULL != stack);
	assert(NULL != dest);
	assert(!StackIsEmpty(stack));
	
	memcpy(dest,((void*)
	((unsigned char*)(stack->arr) - stack->elemnt_size)), stack->elemnt_size);
}


