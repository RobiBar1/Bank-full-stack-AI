#include <stdio.h>  /* printf */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */
#include <string.h> /* memcpy */

#include "stack.h"  /* our api */

int main()
{
	stack_t* stack = StackCreate(8, sizeof(int));
	int x = 5;

	StackPush(stack, &x);
	printf("cur_size: %lu\n", StackSize(stack));
	x = 0;
	StackPeek(stack, &x);
	printf("x after peek: %d\n", x);
	x=7;
	StackPush(stack, &x);
	x=9;
	StackPush(stack, &x);
	x = 3;
	StackPeek(stack, &x);
	printf("x after peek: %d\n", x);
	StackPop(stack);
	StackPeek(stack, &x);
	printf("x after peek: %d\n", x);
	
	StackDestroy(stack); 
	stack = NULL;
	
	return 0;
}
