#include <stddef.h> /* size_t */
#include <stdio.h> /* printf */

#include "vector.h" /* vector API */

#include "heap.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

#define APPLY_TEST(func_call) CheckTest(func_call, #func_call)

typedef enum {TEST_STATUS_SUCCESS, TEST_STATUS_FAILURE} test_status_t;

static void CheckTest(test_status_t result, char* name);

static test_status_t HeapCreate_SmokeTest(void);
static test_status_t HeapPush_SmokeTest(void);
static test_status_t HeapPushPop_SingleElement_SmokeTest(void);
static test_status_t HeapPushPeek_SingleElement_Test(void);
static test_status_t HeapPushPeekPop_TwoElements_Simple_Test(void);
static test_status_t HeapPushPeekPop_TwoElementsStructs_Simple_Test(void);
static test_status_t HeapWithSiftUp_TwoElements_Simple_Test(void);
static test_status_t Heap_5elements_Test(void);
static test_status_t HeapIsEmpty_Test(void);
static test_status_t HeapCount_Test(void);
static test_status_t HeapRemove_Test(void);
static test_status_t HeapRemove_InterestingMatch_Test(void);
static test_status_t Heap_DoesResizing_Test(void);

/* gd test_heap.c ../src/heap.c  ../src/vector.c -I ../include/ */

static void CheckTest(test_status_t result, char* name)
{
	if (result == TEST_STATUS_SUCCESS)
	{
		printf("-> %s: ", name);
		printf(GREEN "PASSED\n\n" RESET);
	}
	else 
	{
		printf(RED "-> %s: FAILED\n\n" RESET, name);
	}
}


int main()
{
	printf("\n ---- TESTS ----\n\n");
	
	APPLY_TEST(HeapCreate_SmokeTest());
	APPLY_TEST(HeapPush_SmokeTest());
	APPLY_TEST(HeapPushPop_SingleElement_SmokeTest());
	APPLY_TEST(HeapPushPeek_SingleElement_Test());
	APPLY_TEST(HeapPushPeekPop_TwoElements_Simple_Test());
	APPLY_TEST(HeapPushPeekPop_TwoElementsStructs_Simple_Test());
	APPLY_TEST(HeapWithSiftUp_TwoElements_Simple_Test());
	APPLY_TEST(Heap_5elements_Test());
	APPLY_TEST(HeapIsEmpty_Test());
	APPLY_TEST(HeapCount_Test());
	APPLY_TEST(HeapRemove_Test());
	APPLY_TEST(HeapRemove_InterestingMatch_Test());
	APPLY_TEST(Heap_DoesResizing_Test());
	
	return 0;
}

/* ==== HELPER FUNCTIONS ==== */

int CompareInts(const void* one, const void* two)
{
	return (*(int*)one - *(int*)two);
}

int MatchInts(const void* one, const void* two)
{
	return (*(int*)one == *(int*)two);
}

int MatchIntsDiffIsMaxTwo(const void* one, const void* two)
{
	return ((*(int*)one >= *(int*)two - 2) && (*(int*)one <= *(int*)two + 2));
}

/* ==== TEST FUNCTIONS ==== */


static test_status_t HeapCreate_SmokeTest(void)
{
	heap_t* heap = HeapCreate(10, &CompareInts);
	HeapDestroy(heap);
	
	return TEST_STATUS_SUCCESS;
}

static test_status_t HeapPush_SmokeTest(void)
{
	heap_t* heap = HeapCreate(10, &CompareInts);
	int a = 5;
	
	HeapPush(heap, &a);
	
	HeapDestroy(heap);
	return TEST_STATUS_SUCCESS;
}

static test_status_t HeapPushPop_SingleElement_SmokeTest(void)
{
	heap_t* heap = HeapCreate(10, &CompareInts);
	int a = 5;
	
	HeapPush(heap, &a);
	HeapPop(heap);
	
	HeapDestroy(heap);
	return TEST_STATUS_SUCCESS;
}

static test_status_t HeapPushPeek_SingleElement_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	heap_t* heap = HeapCreate(10, &CompareInts);
	int a = 5;
	int* received = 0;
	
	HeapPush(heap, &a);
	
	received = HeapPeek(heap);
	if (&a != received)
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HeapPop(heap);
	
	HeapDestroy(heap);
	return res;
}

static test_status_t HeapPushPeekPop_TwoElements_Simple_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	heap_t* heap = HeapCreate(10, &CompareInts);
	int a = 5;
	int b = 2;
	int* received = 0;
	
	HeapPush(heap, &a);
	HeapPush(heap, &b);
	
	received = HeapPeek(heap);
	if (&a != received)
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HeapPop(heap);
	
	received = HeapPeek(heap);
	if (&b != received)
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HeapPop(heap);
	
	HeapDestroy(heap);
	return res;
}

typedef struct planet {
	size_t planet_id;
	size_t amount_secret_bases;
} planet_t;

static test_status_t HeapPushPeekPop_TwoElementsStructs_Simple_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	heap_t* heap = HeapCreate(10, &CompareInts); /* TODO: change to other Compare */
	planet_t a = {2,12};
	planet_t b = {1, 16};
	planet_t* received = 0;
	
	HeapPush(heap, &a);
	HeapPush(heap, &b);
	
	received = HeapPeek(heap);
	if (&a != received)
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HeapPop(heap);
	
	received = HeapPeek(heap);
	if (&b != received)
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HeapPop(heap);
	
	HeapDestroy(heap);
	return res;
}

static test_status_t HeapWithSiftUp_TwoElements_Simple_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	heap_t* heap = HeapCreate(10, &CompareInts);
	int a = 5;
	int b = 8;
	int* received = 0;
	
	HeapPush(heap, &a);
	HeapPush(heap, &b);
	
	received = HeapPeek(heap);
	if (&b != received)
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HeapPop(heap);
	
	received = HeapPeek(heap);
	if (&a != received)
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HeapPop(heap);
	
	HeapDestroy(heap);
	return res;
}

static test_status_t Heap_5elements_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	heap_t* heap = HeapCreate(10, &CompareInts);
	int a = 5;
	int b = 8;
	int c = 12;
	int d = 7;
	int e = 2;
	int* received = 0;
	
	HeapPush(heap, &a);
	HeapPush(heap, &b);
	HeapPush(heap, &c);
	
	received = HeapPeek(heap);
	if (&c != received)
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HeapPush(heap, &d);
	HeapPush(heap, &e);
	
	if (&c != HeapPeek(heap))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HeapPop(heap);
	
	if (&b != HeapPeek(heap))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HeapPop(heap);
	
	if (&d != HeapPeek(heap))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HeapPop(heap);
	
	if (&a != HeapPeek(heap))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HeapPop(heap);
	
	if (&e != HeapPeek(heap))
	{
		res = TEST_STATUS_FAILURE;
	}

	
	HeapDestroy(heap);
	return res;
}

static test_status_t HeapIsEmpty_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	heap_t* heap = HeapCreate(10, &CompareInts);
	int a = 5;
	int b = 8;
	int c = 12;
	
	if (1 != HeapIsEmpty(heap))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HeapPush(heap, &a);
	
	if (0 != HeapIsEmpty(heap))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HeapPush(heap, &b);
	HeapPush(heap, &c);
	
	if (0 != HeapIsEmpty(heap))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HeapPop(heap);

	if (0 != HeapIsEmpty(heap))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HeapPop(heap);
	HeapPop(heap);
	
	if (1 != HeapIsEmpty(heap))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HeapDestroy(heap);
	return res;
}

static test_status_t HeapCount_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	heap_t* heap = HeapCreate(10, &CompareInts);
	int a = 5;
	int b = 8;
	int c = 12;
	
	if (0 != HeapCount(heap))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HeapPush(heap, &a);
	
	if (1 != HeapCount(heap))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HeapPush(heap, &b);
	HeapPush(heap, &c);
	
	if (3 != HeapCount(heap))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HeapPop(heap);

	if (2 != HeapCount(heap))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HeapPop(heap);
	HeapPop(heap);
	
	if (0 != HeapCount(heap))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HeapDestroy(heap);
	return res;
}

static test_status_t HeapRemove_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	heap_t* heap = HeapCreate(10, &CompareInts);
	int a = 5;
	int b = 8;
	int c = 12;
	int d = 7;
	int e = 2;
	int* received = 0;
	
	HeapPush(heap, &a);
	HeapPush(heap, &b);
	HeapPush(heap, &c);
	
	received = HeapPeek(heap);
	if (&c != received)
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (a != *(int*)HeapRemove(heap, &a, MatchInts))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (2 != HeapCount(heap))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HeapPush(heap, &d);
	HeapPush(heap, &e);
	
	if (e != *(int*)HeapRemove(heap, &e, MatchInts))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (c != *(int*)HeapRemove(heap, &c, MatchInts))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (&b != HeapPeek(heap))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (2 != HeapCount(heap))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HeapDestroy(heap);
	return res;
}

static test_status_t HeapRemove_InterestingMatch_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	heap_t* heap = HeapCreate(10, &CompareInts);
	int a = 7;
	int b = 8;
	int c = 10;
	int d = 4;
	int e = 6;
	
	HeapPush(heap, &a);
	HeapPush(heap, &b);
	HeapPush(heap, &c);
	HeapPush(heap, &d);
	HeapPush(heap, &e);
	
	if (&c != HeapPeek(heap))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (&c != HeapRemove(heap, &b, MatchIntsDiffIsMaxTwo))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (&b != HeapPeek(heap))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HeapDestroy(heap);
	return res;
}

static test_status_t Heap_DoesResizing_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	heap_t* heap = HeapCreate(1, &CompareInts);
	int a = 5;
	int b = 8;
	int c = 12;
	int d = 7;
	int e = 2;
	int* received = 0;
	
	HeapPush(heap, &a);
	HeapPush(heap, &b);
	HeapPush(heap, &c);
	
	received = HeapPeek(heap);
	if (&c != received)
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (a != *(int*)HeapRemove(heap, &a, MatchInts))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (2 != HeapCount(heap))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HeapPush(heap, &d);
	HeapPush(heap, &e);
	
	if (e != *(int*)HeapRemove(heap, &e, MatchInts))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (c != *(int*)HeapRemove(heap, &c, MatchInts))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (&b != HeapPeek(heap))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (2 != HeapCount(heap))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HeapDestroy(heap);
	return res;
}

