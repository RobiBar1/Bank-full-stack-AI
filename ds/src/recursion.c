/*
Writer:  Robi
Checker: Michael
Date:    10.02.2026
*/
#include <string.h> 	/* strncmp */
#include <assert.h> 	/* assert  */

#include "recursion.h"  /* our api */

static void SortStackHelp(stack_t* stack, size_t size, int num)
{
    int temp = 0;
    
    assert(NULL != stack);

    if(0 == size)
    {
        StackPush(stack, &num);
        return;
    }

    StackPeek(stack, &temp);
    StackPop(stack);

    if(num > temp)
    {
        SortStackHelp(stack, size - 1, num);
        StackPush(stack, &temp);
        return;
    }
    else
    {
        SortStackHelp(stack, size - 1, temp);
        StackPush(stack, &num);
        return;
    }
}

static void CallStackHelp(stack_t* stack, size_t size)
{
    int top_stack = 0;

    assert(NULL != stack);

    if(1 == size)
    {
        return;
    }
    
    StackPeek(stack, &top_stack);
    StackPop(stack);
    
    SortStackHelp(stack, size - 1, top_stack);
    CallStackHelp(stack, size - 1);
}

stack_t* SortStack(stack_t* stack)
{
    assert(NULL != stack);

    if(StackIsEmpty(stack))
    {
        return stack;
    }
    
    CallStackHelp(stack, StackSize(stack));
    return stack;
}

size_t Strlen(const char* str)
{
	assert (NULL != str);
	
	if ('\0' == *str)
	{
		return 0;
	}
	
	return 1 + Strlen(++str);
}

int Strcmp(const char *s1, const char *s2)
{
	assert (NULL != s1);
	assert (NULL != s2);
	
	if ('\0' == *s1 || *s1 != *s2)
	{
		return *s1 - *s2;
	}
	
	return Strcmp(++s1, ++s2);
}

char* Strcpy(char* dst, const char* src)
{
	assert (NULL != dst);
	assert (NULL != src);
	
	*dst = *src;
	if ('\0' == *src)
	{		
		return dst;
	}
	
	Strcpy(dst + 1, src + 1);
	
	return dst;
}

char* Strcat(char* dst, const char* src)
{
	char* tmp = dst;
	
	assert (NULL != dst);
	assert (NULL != src);
	
	dst = dst + Strlen(dst);
	Strcpy(dst, src);
	
	return tmp;
}

char* Strstr(const char* haystack, const char* needle)
{
	assert (NULL != haystack);
	assert (NULL != needle);
	
	if ('\0' == *haystack)
	{
		return NULL;
	}
	
	if (0 == strncmp(haystack, needle, Strlen(needle)))
	{
		return (char*)haystack;
	}
	
	return Strstr(haystack + 1, needle);
}

int FibonacciRecursion(int element_index)
{
	if (0 == element_index || 1 == element_index)					
	{
		return element_index;
	}
	
	return (FibonacciRecursion(element_index - 1) +
		   FibonacciRecursion(element_index - 2));
}

int FibonacciIterative(int element_index)
{
	int a0 = 0;
	int sum = 0;
	int a1 = 1;
	int i = 2;
	
	if (0 == element_index || 1 == element_index)					
	{
		return element_index;
	}
	
	for (; i <= element_index; ++i)
	{
		sum = a0 + a1;
		a0 = a1;
		a1 = sum;
	}
	
	return sum;
}

sll_node_t* FlipList(sll_node_t* head)
{
	sll_node_t* new_head = NULL;
	
	if (NULL == head->next)
	{
		return head;
	}
	
	new_head = FlipList(head->next);
	head->next->next = head;
	head->next = NULL;
	
	return new_head;
}
