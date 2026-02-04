
#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

/* gd src/bst.c test/test_bst.c -I include/ */
/*
- print real root data:
p *(int*)bst->end->children[LEFT]->data

*/

#define GREEN "\033[0;32m"
#define RED   "\033[0;31m"
#define RESET "\033[0m"

#define TEST(name, real, expected) \
    do { \
        ++tests_run; \
        printf("%s: ", name); \
        if((size_t)(real) == (size_t)(expected)) { \
            ++tests_passed; \
            printf(GREEN "PASS" RESET "\n"); \
        } else { \
            printf(RED "FAIL" RESET "\n"); \
            printf(RED "      Expected: %ld, Got: %ld" RESET "\n", (size_t)(expected), (size_t)(real)); \
        } \
    } while(0)

static size_t tests_run = 0;
static size_t tests_passed = 0;

static void PrintStart(char* name_function)
{
	printf("================================================================\n");
	printf("------------------- Start Test: %s ------------------------\n", name_function);
}
static void PrintEnd(char* name_function)
{
	printf("================================================================\n");
	printf("------------------- End Test: %s --------------------------\n", name_function);
}

static int CmpFunc(const void* left, const void* right)
{
	return *(int*)left - *(int*)right;
}

static void TestBSTSmokeTest(void)
{
    bst_t* bst = BSTCreate(CmpFunc);
    
    BSTDestroy(bst);
    
    TEST("TestBSTSmokeTest", 1, 1);
}

static void TestBSTCreate(void)
{
	bst_t* bst = BSTCreate(CmpFunc);
	int is_good = 1;
	if (NULL == bst)
	{
		is_good = 0;
	}
    TEST("TestBSTCreate", is_good, 1);
    BSTDestroy(bst);
}


static void TestBSTInsert(void)
{
    bst_t* bst = BSTCreate(CmpFunc);
    int arr[] = {10,50,30,0,15,75,100};
    int sorted_arr[] = {0, 10, 15, 30, 50, 75, 100};
    int arr_size = sizeof(arr) / sizeof(int);
    int i = 0;
    int is_good = 1;
    bst_iter_t iter = NULL;

    for (i = 0; i < arr_size; ++i)
    {
        BSTInsert(bst, &arr[i]);
    }

    iter = BSTBegin(bst);
    for (i = 0; i < arr_size - 1; ++i)
    {
      if (sorted_arr[i] != *(int*)BSTGetData(iter))
      {
          is_good = 0;
      }
      iter = BSTGetNext(iter);
    }

    TEST("TestBSTInsert", is_good, 1);
    BSTDestroy(bst);
}

static void TestBSTRemove(void)
{
    bst_t* bst = BSTCreate(CmpFunc);
    int arr[] = {1, 18, 9, 6, 12, 15};
    int b[] = {1, 6, 9, 12, 15, 18};
    size_t size = sizeof(arr) / sizeof(int);
    size_t i = 0;
    bst_iter_t iter = NULL;
    size_t is_good = 1;
    
    for(; i < size; ++i)
    {
        BSTInsert(bst, &arr[i]);
    }
    
    iter = BSTBegin(bst);
    
    for(i = 0; i < size - 1; ++i)
    {
        iter = BSTRemove(iter);
        
        if(b[i + 1] != *(int*)BSTGetData(iter))
        {
            is_good = 0; 
        }
    }
    
    TEST("TestBSTRemove", is_good, 1);
    
    BSTDestroy(bst);
}

static void TestBSTRemoveMultiple(void)
{
    bst_t* bst = BSTCreate(CmpFunc);
    int arr[] = {9, 18, 1, 6, 12, 15};
    int b[] = {9, 12, 15, 18};
    size_t size = sizeof(arr) / sizeof(int);
    size_t i = 0;
    bst_iter_t iter = NULL;
    size_t is_good = 1;
    
    for(; i < size; ++i)
    {
        BSTInsert(bst, &arr[i]);
    }
    
    iter = BSTFind(bst, &arr[0]);
    
    for(i = 0; i < 3; ++i) /* remove 9, 12, 15 */
    {
        iter = BSTRemove(iter);
        
        if(b[i + 1] != *(int*)BSTGetData(iter))
        {
            is_good = 0; 
        }
    }
    
    /* After tree is: 18, 1, 6 */
    iter = BSTBegin(bst); /* 1 */
    
    if(arr[2] != *(int*)BSTGetData(iter))
    {
        is_good = 0; 
    }
    
    iter = BSTRemove(iter);
        
    if(arr[3] != *(int*)BSTGetData(iter))
    {
        is_good = 0; 
    }
    
    TEST("TestBSTRemoveMultiple", is_good, 1);
    
    BSTDestroy(bst);
}

static void TestBSTCount(void)
{
	bst_t* bst = BSTCreate(CmpFunc);
	int arr[] = {10, 50, 30, 0, 15, 75, 100};
	int arr_size = sizeof(arr) / sizeof(int);
	int i = 0;
	
	TEST("TestBSTCount Empty", BSTCount(bst), 0);
	
	for (i = 0; i < arr_size; ++i)
	{
		BSTInsert(bst, &arr[i]);
	}
	
	TEST("TestBSTCount with elements", BSTCount(bst), arr_size);
	BSTDestroy(bst);
}

static void TestBSTGetPrev(void)
{
	bst_t* bst = BSTCreate(CmpFunc);
	int arr[] = {10, 50, 30, 0, 15, 75, 100};
	/*0, 10, 15, 30, 50, 75, 100*/
	int arr_size = sizeof(arr) / sizeof(int);
	int i = 0;
	bst_iter_t iter = NULL;
	
	for (i = 0; i < arr_size; ++i)
	{
		BSTInsert(bst, &arr[i]);
	}
	
	iter = BSTBegin(bst);
	iter = BSTGetNext(iter);
	iter = BSTGetNext(iter);
	iter = BSTGetPrev(iter);
	
	TEST("TestBSTGetPrev", *(int*)BSTGetData(iter), 10);
	
	#ifndef NDEBUG
	PrintStart("Debug-Mode_Check-only");
	printf(RED "IF YOU WONT GET ASSERT HERE YOU DID WRONG IMPLEMENT OF PREV" RESET "\n");
	iter = BSTBegin(bst);
	++tests_run;
	printf("current data: %d\n", *(int*)BSTGetData(iter));
	iter = BSTGetPrev(iter);
	printf("current data: %lu\n", *(size_t*)BSTGetData(iter));
	printf(GREEN "NOW YOU NEED GET ASSERT, IF U GET ASSERT LOOK ON IT AS 1 MORE SUCCESS TEST" RESET "\n");
	printf("\nSUMMARY: %lu/%lu tests passed.\n", tests_passed, tests_run);
	iter = BSTGetPrev(iter); /* should throw assert if work fine */
	printf(RED "fail in GetPrev - case: try to go prev from first logical element" RESET "\n");
	#endif
	
	BSTDestroy(bst);
}

static void TestBSTIsEmpty(void)
{
	bst_t* bst = BSTCreate(CmpFunc);
	int arr[] = {10, 50, 30, 0, 15, 75, 100};
	int arr_size = sizeof(arr) / sizeof(int);
	int i = 0;
	
	TEST("TestBSTIsEmpty No Elements", BSTIsEmpty(bst), 1);
	
	for (i = 0; i < arr_size; ++i)
	{
		BSTInsert(bst, &arr[i]);
	}
	
	TEST("TestBSTCount With Elements", BSTIsEmpty(bst), 0);
	BSTDestroy(bst);
}

static int ActionFunc(void* num, void* param)
{
    (void)param;
    *(int*)num = (*(int*)num * *(int*)num);
    return 0;
}

static int ActionFunc2(void* num, void* param)
{
    *(int*)num = (*(int*)num - *(int*)param);
    
    return 0;
}

static void TestBSTForEach(void)
{
    bst_t* bst = BSTCreate(CmpFunc);
    int arr[] = {9, 18, 1, 6, 12, 15};
    int b[] = {81, 144, 225, 324};
    int c[] = {80, 143, 224, 323};
    size_t size = sizeof(arr) / sizeof(int);
    size_t i = 0;
    bst_iter_t iter = NULL;
    size_t is_good = 1;
    
    for(; i < size; ++i)
    {
        BSTInsert(bst, &arr[i]);
    }
    
    iter = BSTFind(bst, &arr[0]);
    
    BSTForEach(iter, BSTEnd(bst), ActionFunc, NULL);
    
    for(i = 0; i < 3; ++i) /* check 81, 144, 225 */
    {
        if(b[i] != *(int*)BSTGetData(iter))
        {
            is_good = 0; 
        }
        
        iter = BSTGetNext(iter);
    }
    
    iter = BSTFind(bst, &b[0]);
    
    BSTForEach(iter, BSTEnd(bst), ActionFunc2, &arr[2]);
    
    for(i = 0; i < 3; ++i) /* check 80, 143, 224 */
    {
        if(c[i] != *(int*)BSTGetData(iter))
        {
            is_good = 0; 
        }
        
        iter = BSTGetNext(iter);
    }
    
    TEST("TestBSTForEach", is_good, 1);
    
    BSTDestroy(bst);
}



void TestBSTFind(void)
{
    bst_t* bst = BSTCreate(CmpFunc);
    size_t x = 1;
    int arr[] = {1, 18, 9, 6, 12, 15};
    size_t size = sizeof(arr) / sizeof(int);
    size_t i = 0;
    bst_iter_t iter = NULL;
    
    PrintStart("BSTFind");
    for(; i < size; ++i)
    {
        BSTInsert(bst, &arr[i]);
    }
    
    iter = BSTFind(bst, &x);
    
    TEST("FindTest - Case: first element exist in tree",
    (*(int*)(BSTGetData(iter))), x);
    	
    x = 6;
    iter = BSTFind(bst, &x);
    TEST("FindTest - Case: element exist in tree",(*(int*)(BSTGetData(iter))),
    	 x);
    	 
   	x = 18;
    iter = BSTFind(bst, &x);
    TEST("FindTest - Case: last element exist in tree"
    ,(*(int*)(BSTGetData(iter))), x);
    
    x = 7;
    iter = BSTFind(bst, &x);
    TEST("FindTest - Case: element not exist in tree"
    ,((iter)), BSTEnd(bst));
    
    BSTDestroy(bst);
    PrintEnd("BSTFind");
}


int main() 
{
    TestBSTSmokeTest();
    TestBSTCreate();
    TestBSTInsert();
    TestBSTRemove();
    TestBSTRemoveMultiple();
    TestBSTCount();
    TestBSTIsEmpty();
    TestBSTForEach();
    TestBSTFind();
    TestBSTGetPrev();
    
    printf("\nSUMMARY: %lu/%lu tests passed.\n", tests_passed, tests_run);
    return 0;
}

