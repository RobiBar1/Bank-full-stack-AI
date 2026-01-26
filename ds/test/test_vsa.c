#include <stdio.h> /* printf */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */

#include "vsa.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

#define APPLY_TEST(func_call) CheckTest(func_call, #func_call)

#define WORD_BYTES (sizeof(size_t))

typedef enum {TEST_STATUS_SUCCESS, TEST_STATUS_FAILURE} test_status_t;

/* gd test_vsa.c ../src/vsa.c  -I ../include/ */

/* ==== FORWARD DELARATIONS ==== */

static void CheckTest(test_status_t result, char* name);

static test_status_t VSAInit_SmokeTest(void);
static test_status_t VSALargestFreeChunk_EmptyCase_Test(void);
static test_status_t VSAAlloc_WhiteBox_BasicTest(void);
static test_status_t VSAAllocFree_WhiteBox_Test(void);
static test_status_t VSAAllocFree_WhiteBox_Test2(void);

/* ==== TEST FUNCTION ==== */

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

	APPLY_TEST(VSAInit_SmokeTest());
	APPLY_TEST(VSALargestFreeChunk_EmptyCase_Test());
	APPLY_TEST(VSAAlloc_WhiteBox_BasicTest());
	APPLY_TEST(VSAAllocFree_WhiteBox_Test());
	APPLY_TEST(VSAAllocFree_WhiteBox_Test2());
	
	return 0;
}

/* ==== HELPER FUNCTIONS ==== */

/* ==== TEST FUNCTIONS ==== */

static test_status_t VSAInit_SmokeTest(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	
	size_t num_bytes = WORD_BYTES*4;
	
	size_t* buffer = (size_t*)malloc(num_bytes);
	vsa_t vsa = VSAInit((void*)buffer, num_bytes);
	
	if (buffer != (size_t*)vsa)
	{
		res = TEST_STATUS_FAILURE;
	}
	
	free(buffer);
	
	return res;
}

static test_status_t VSALargestFreeChunk_EmptyCase_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	
	#ifndef NDEBUG
	size_t num_bytes = WORD_BYTES*4;
	#else
	size_t num_bytes = WORD_BYTES*3;
	#endif
	
	size_t* buffer = (size_t*)malloc(num_bytes);
	vsa_t vsa = VSAInit((void*)buffer, num_bytes);
	
	if (buffer != (size_t*)vsa)
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (WORD_BYTES != VSALargestFreeChunk(vsa))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	free(buffer);
	
	return res;
}

static test_status_t VSAAlloc_WhiteBox_BasicTest(void)
{
	/* a bit white box testing here */
	
	test_status_t res = TEST_STATUS_SUCCESS;
	#ifndef NDEBUG
	size_t num_bytes = WORD_BYTES*2 + WORD_BYTES*12 + WORD_BYTES;
	#else
	size_t num_bytes = WORD_BYTES*1 + WORD_BYTES*12 + WORD_BYTES;
	#endif
	
	size_t* buffer = (size_t*)malloc(num_bytes);
	vsa_t vsa = VSAInit((void*)buffer, num_bytes);
	VSAAlloc(vsa, 4*WORD_BYTES);

	#ifndef NDEBUG
	if (6*WORD_BYTES != VSALargestFreeChunk(vsa))
	{
		res = TEST_STATUS_FAILURE;
	}
	#else
	if (7*WORD_BYTES != VSALargestFreeChunk(vsa))
	{
		res = TEST_STATUS_FAILURE;
	}
	#endif
	
	free(buffer);
	
	return res;
}

static test_status_t VSAAllocFree_WhiteBox_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	
	#ifndef NDEBUG
	
	size_t num_bytes = WORD_BYTES*2 + WORD_BYTES*5 + WORD_BYTES;
	
	size_t* buffer = (size_t*)malloc(num_bytes);
	vsa_t vsa = VSAInit((void*)buffer, num_bytes);
	void* ptr1 = NULL;
	void* ptr2 = NULL;
	
	ptr1 = VSAAlloc(vsa, 2*WORD_BYTES);
	
	if (1*WORD_BYTES != VSALargestFreeChunk(vsa))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	ptr2 = VSAAlloc(vsa, 1*WORD_BYTES);
	
	if (0 != VSALargestFreeChunk(vsa))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	VSAFree(ptr1); ptr1 = NULL;
	
	ptr1 = VSAAlloc(vsa, 1*WORD_BYTES);
	
	if (0 != VSALargestFreeChunk(vsa))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	VSAFree(ptr1); ptr1 = NULL;
	
	if (2*WORD_BYTES != VSALargestFreeChunk(vsa))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	VSAFree(ptr2); ptr2 = NULL;
	
	if (5*WORD_BYTES != VSALargestFreeChunk(vsa))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	free(buffer);
	
	#endif
	
	printf("[Do in debug mode]\n");
	
	return res;
}

static test_status_t VSAAllocFree_WhiteBox_Test2(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	
	#ifndef NDEBUG
	
	size_t num_bytes = WORD_BYTES*2 + WORD_BYTES*20 + WORD_BYTES;
	
	size_t* buffer = (size_t*)malloc(num_bytes);
	vsa_t vsa = VSAInit((void*)buffer, num_bytes);
	void* ptr1 = NULL;
	void* ptr2 = NULL;
	
	ptr1 = VSAAlloc(vsa, 15*WORD_BYTES); /* 120 M [X] 17 M [] $ */
	
	if (3*WORD_BYTES != VSALargestFreeChunk(vsa))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	ptr2 = VSAAlloc(vsa, 2*WORD_BYTES); /* 120 M [X] 16 M [X] $ */
	
	if (0 != VSALargestFreeChunk(vsa))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	VSAFree(ptr1); ptr1 = NULL; /* 121 M [] 16 M [X] $ */
	VSAFree(ptr2); ptr2 = NULL; /* 121 M [] 17 M [] $ */
	
	ptr1 = VSAAlloc(vsa, 12*WORD_BYTES); /* 96 M [X] 9 M [] 17 M [] $ */
	ptr2 = VSAAlloc(vsa, 3*WORD_BYTES);  /* 96 M [X] 24 M [X]  9 M [] $ */
	
	if (WORD_BYTES != VSALargestFreeChunk(vsa))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	VSAAlloc(vsa, 1*WORD_BYTES);
	
	if (0 != VSALargestFreeChunk(vsa))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	free(buffer);
	
	#endif
	
	printf("[Do in debug mode]\n");
	
	return res;
}








