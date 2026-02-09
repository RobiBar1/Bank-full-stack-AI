#include "recursion.h"

static SortStack1(stack_t* s, int cur_num)
{
	int tmp = 0;
	
	assert (NULL != s);
	
	if (!StackIsEmpty(s))
	{
		tmp = StackPeek(s, &tmp);
	}
	
	
}

static SortStack2(stack_t* stack, size_t size)
{
	assert (NULL != stack);
}

stack_t* SortStack(stack_t* s)
{
	int tmp = 0;
	
	assert (NULL != stack);
	
	if (!StackIsEmpty(s))
	{
		tmp = StackPeek(s, &tmp);
		SortStack1(s, StackSize(s), tmp);
	}
	
	return s;
}
