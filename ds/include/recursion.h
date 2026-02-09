#ifndef __RECURSION_H__
#define __RECURSION_H__

#include <stddef.h> /* size_t */

#include "stack.h"
#include "sorted_list.h"

typedef struct SllNode
{
    struct SllNode* next;
    int data;
} sll_node_t;

int FibonacciIteratively(int element_index);

int FibonacciRecursion(int element_index);

sll_node_t* FlipList(sll_node_t* head);

/* Smallest value at top */
stack_t* SortStack(stack_t* stack);

size_t Strlen(const char* str);

int StrCmp(const char *s1, const char *s2);

char* StrCpy(char* dst, const char* src);

char* StrCat(char* dst, const char* src);

char* StrStr(const char* haystack, const char* needle);

#endif /*__RECURSION_H__*/
