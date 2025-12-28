#ifndef __STACK_H__
#define __STACK_H__

#include <stddef.h>

typedef struct stack stack_t;

stack_t* StackCreate(size_t capacity, size_t element_size);
void StackDestroy(stack_t* stack);
void StackPush(stack_t* stack, const void* data);
void StackPop(stack_t* stack);
int StackIsEmpty(const stack_t* stack);
size_t StackSize(const stack_t* stack);
size_t StackCapacity(const stack_t* stack);
void StackPeek(const stack_t* stack, void* dest);

#endif /*__STACK_H__*/
