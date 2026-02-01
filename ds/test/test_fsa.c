#include <stdio.h> /* printf */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */

#include "fsa.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

#define APPLY_TEST(func_call) CheckTest(func_call, #func_call)

#define WORD_BYTES (sizeof(size_t))

typedef enum {TEST_STATUS_SUCCESS, TEST_STATUS_FAILURE} test_status_t;

/* gd test_fsa.c ../src/fsa.c  -I ../include/ */

/* ==== FORWARD DELARATIONS ==== */

static void CheckTest(test_status_t result, char* name);


static test_status_t FSASuggestSize_Test(void);
static test_status_t FSAInit_BlackBoxTest(void);
static test_status_t FSACountFreeChunks_EmptyCase_Test(void);
static test_status_t FSACountFreeChunks_OnlyAllocCase_Test(void);
static test_status_t FSAFree_OnlyFromEndCase_Test(void);
static test_status_t FSAFree_NotFilled_Test(void);
static test_status_t FSAFree_Test(void);


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

	APPLY_TEST(FSASuggestSize_Test());
	APPLY_TEST(FSAInit_BlackBoxTest());
	APPLY_TEST(FSACountFreeChunks_EmptyCase_Test());
	APPLY_TEST(FSACountFreeChunks_OnlyAllocCase_Test());
	APPLY_TEST(FSAFree_OnlyFromEndCase_Test());
	APPLY_TEST(FSAFree_NotFilled_Test());
	APPLY_TEST(FSAFree_Test());
	
	return 0;
}

/* ==== HELPER FUNCTIONS ==== */

/* ==== TEST FUNCTIONS ==== */

static test_status_t FSASuggestSize_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	
	printf("Test for 64-bit\n");
	
	if ((5*WORD_BYTES + WORD_BYTES) != FSASuggestSize(WORD_BYTES, 5))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if ((5*(2*WORD_BYTES) + WORD_BYTES) != FSASuggestSize(WORD_BYTES + 2, 5))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if ((5*(2*WORD_BYTES) + WORD_BYTES) != FSASuggestSize(WORD_BYTES + 4, 5))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if ((5*WORD_BYTES + WORD_BYTES) != FSASuggestSize(3, 5))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if ((5*(3*WORD_BYTES) + WORD_BYTES) != FSASuggestSize(WORD_BYTES*2 + 4, 5))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	
	return res;
}

static test_status_t FSAInit_BlackBoxTest(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	
	size_t chunk_size = 2*sizeof(size_t);
	size_t num_chunks = 4;
	size_t total_size = chunk_size*num_chunks + sizeof(size_t);
	
	size_t* buffer = (size_t*)malloc(total_size);
	fsa_t memory_pool = FSAInit(chunk_size, num_chunks, (void*)buffer);
	
	if (buffer != (size_t*)memory_pool)
	{
		res = TEST_STATUS_FAILURE;
	}
	
	free(buffer);
	
	return res;
}

static test_status_t FSACountFreeChunks_EmptyCase_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	
	size_t chunk_size = 2*sizeof(size_t);
	size_t num_chunks = 4;
	size_t total_size = chunk_size*num_chunks + sizeof(size_t);
	
	size_t* buffer = (size_t*)malloc(total_size);
	fsa_t memory_pool = FSAInit(chunk_size, num_chunks, (void*)buffer);
	
	if (num_chunks != FSACountFreeChunks(memory_pool))
	{
		res = TEST_STATUS_FAILURE;
	}
	free(buffer);
	
	
	chunk_size = 7*sizeof(size_t);
	num_chunks = 2;
	total_size = chunk_size*num_chunks + sizeof(size_t);
	buffer = malloc(total_size);
	memory_pool = FSAInit(chunk_size, num_chunks, (void*)buffer);
	
	if (num_chunks != FSACountFreeChunks(memory_pool))
	{
		res = TEST_STATUS_FAILURE;
	}
	free(buffer);
	
	
	chunk_size = 1*sizeof(size_t);
	num_chunks = 1;
	total_size = chunk_size*num_chunks + sizeof(size_t);
	buffer = malloc(total_size);
	memory_pool = FSAInit(chunk_size, num_chunks, (void*)buffer);
	
	if (num_chunks != FSACountFreeChunks(memory_pool))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	
	free(buffer);
	return res;
}

static test_status_t FSACountFreeChunks_OnlyAllocCase_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	
	void* ret_address = NULL;
	size_t chunk_size = 2*sizeof(size_t);
	size_t num_chunks = 4;
	size_t total_size = chunk_size*num_chunks + sizeof(size_t);
	
	size_t* buffer = (size_t*)malloc(total_size);
	fsa_t memory_pool = FSAInit(chunk_size, num_chunks, (void*)buffer);
	
	if (num_chunks != FSACountFreeChunks(memory_pool))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	FSAAlloc(memory_pool);

	if ((num_chunks-1) != FSACountFreeChunks(memory_pool))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	FSAAlloc(memory_pool);
	
	if ((num_chunks-2) != FSACountFreeChunks(memory_pool))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	FSAAlloc(memory_pool);
	
	if ((num_chunks-3) != FSACountFreeChunks(memory_pool))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	FSAAlloc(memory_pool);
	
	if (0 != FSACountFreeChunks(memory_pool))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	ret_address = FSAAlloc(memory_pool);
	
	if (NULL != ret_address)
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (0 != FSACountFreeChunks(memory_pool))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	free(buffer);
	return res;
}

static test_status_t FSAFree_OnlyFromEndCase_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	
	void* ret_address1 = NULL;
	void* ret_address2 = NULL;
	void* ret_address3 = NULL;
	void* ret_address4 = NULL;
	size_t chunk_size = 2*sizeof(size_t);
	size_t num_chunks = 4;
	size_t total_size = chunk_size*num_chunks + sizeof(size_t);
	
	size_t* buffer = (size_t*)malloc(total_size);
	fsa_t memory_pool = FSAInit(chunk_size, num_chunks, (void*)buffer);
	
	ret_address1 = FSAAlloc(memory_pool);
	ret_address2 = FSAAlloc(memory_pool);
	
	if (2 != FSACountFreeChunks(memory_pool))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	FSAFree(ret_address2, memory_pool); ret_address2 = NULL;
	
	if (3 != FSACountFreeChunks(memory_pool))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	FSAFree(ret_address1, memory_pool); ret_address1 = NULL;
	
	if (4 != FSACountFreeChunks(memory_pool))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	ret_address1 = FSAAlloc(memory_pool);
	ret_address2 = FSAAlloc(memory_pool);
	ret_address3 = FSAAlloc(memory_pool);
	
	FSAFree(ret_address3, memory_pool); ret_address3 = NULL;
	
	if (2 != FSACountFreeChunks(memory_pool))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	ret_address3 = FSAAlloc(memory_pool);
	ret_address4 = FSAAlloc(memory_pool);
	
	if (0 != FSACountFreeChunks(memory_pool))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	FSAFree(ret_address4, memory_pool); ret_address4 = NULL;
	
	if (1 != FSACountFreeChunks(memory_pool))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	ret_address4 = FSAAlloc(memory_pool);
	
	if (0 != FSACountFreeChunks(memory_pool))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	free(buffer);
	return res;
}

static test_status_t FSAFree_NotFilled_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	
	void* ret_address0 = NULL;
	void* ret_address1 = NULL;
	void* ret_address2 = NULL;
	void* ret_address3 = NULL;
	
	size_t chunk_size = 2*sizeof(size_t);
	size_t num_chunks = 6;
	size_t total_size = chunk_size*num_chunks + sizeof(size_t);
	
	size_t* buffer = (size_t*)malloc(total_size);
	fsa_t memory_pool = FSAInit(chunk_size, num_chunks, (void*)buffer);
	
	ret_address0 = FSAAlloc(memory_pool);
	ret_address1 = FSAAlloc(memory_pool);
	ret_address2 = FSAAlloc(memory_pool);
	ret_address3 = FSAAlloc(memory_pool); /* X X X X _ _ */
	
	if (2 != FSACountFreeChunks(memory_pool))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	FSAFree(ret_address1, memory_pool); ret_address1 = NULL; /* X _ X X _ _ */
	FSAFree(ret_address3, memory_pool); ret_address3 = NULL; /* X _ X _ _ _ */
	
	ret_address3 = FSAAlloc(memory_pool); /* X _ X X _ _ */
	
	if (3 != FSACountFreeChunks(memory_pool))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	FSAFree(ret_address0, memory_pool); ret_address0 = NULL; /* _ _ X X _ _ */
	FSAFree(ret_address2, memory_pool); ret_address2 = NULL; /* _ _ _ X _ _ */
	FSAFree(ret_address3, memory_pool); ret_address3 = NULL; /* _ _ _ _ _ _ */
	
	if (6 != FSACountFreeChunks(memory_pool))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	free(buffer);
	return res;
}

static test_status_t FSAFree_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	
	void* ret_address0 = NULL;
	void* ret_address1 = NULL;
	void* ret_address2 = NULL;
	void* ret_address3 = NULL;
	void* ret_address4 = NULL;
	void* ret_address5 = NULL;
	
	size_t chunk_size = 2*sizeof(size_t);
	size_t num_chunks = 6;
	size_t total_size = chunk_size*num_chunks + sizeof(size_t);
	
	size_t* buffer = (size_t*)malloc(total_size);
	fsa_t memory_pool = FSAInit(chunk_size, num_chunks, (void*)buffer);
	
	ret_address0 = FSAAlloc(memory_pool);
	ret_address1 = FSAAlloc(memory_pool);
	ret_address2 = FSAAlloc(memory_pool);
	ret_address3 = FSAAlloc(memory_pool); /* X X X X _ _ */
	
	if (2 != FSACountFreeChunks(memory_pool))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	FSAFree(ret_address1, memory_pool); ret_address1 = NULL; /* X _ X X _ _ */
	FSAFree(ret_address3, memory_pool); ret_address3 = NULL; /* X _ X _ _ _ */
	
	ret_address3 = FSAAlloc(memory_pool); /* X _ X X _ _ */
	
	if (3 != FSACountFreeChunks(memory_pool))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	ret_address1 = FSAAlloc(memory_pool); /* X X X X _ _ */
	ret_address4 = FSAAlloc(memory_pool); /* X X X X X _ */
	ret_address5 = FSAAlloc(memory_pool); /* X X X X X X */
	
	if (0 != FSACountFreeChunks(memory_pool))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	FSAFree(ret_address0, memory_pool); ret_address0 = NULL; /* _ X X X X X */
	FSAFree(ret_address2, memory_pool); ret_address2 = NULL; /* _ X _ X X X */
	
	if (2 != FSACountFreeChunks(memory_pool))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	ret_address2 = FSAAlloc(memory_pool); /* _ X X X X X */
	FSAFree(ret_address3, memory_pool); ret_address3 = NULL; /* _ X X _ X X */
	
	if (2 != FSACountFreeChunks(memory_pool))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	FSAFree(ret_address5, memory_pool); ret_address5 = NULL; /* _ X X _ X _ */
	FSAFree(ret_address4, memory_pool); ret_address4 = NULL; /* _ X X _ _ _ */
	
	if (4 != FSACountFreeChunks(memory_pool))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	free(buffer);
	return res;
}








