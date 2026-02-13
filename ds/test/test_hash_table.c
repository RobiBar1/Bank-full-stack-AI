#include <stdio.h> /* printf */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */
#include <string.h> /* memcmp */

#include "hash_table.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

#define APPLY_TEST(func_call) CheckTest(func_call, #func_call)

typedef enum {TEST_STATUS_SUCCESS, TEST_STATUS_FAILURE} test_status_t;

/* gd ./test_hash_table.c ../src/hash_table.c ../src/dlist.c -I ../include -lm */

/* ==== FORWARD DELARATIONS ==== */

static void CheckTest(test_status_t result, char* name);

int CompareInts(const void* one, const void* two);

static test_status_t HashTableCreateDestroy_SmokeTest(void);
static test_status_t HashTableInsert_SmokeTest(void);
static test_status_t HashTableRemove_SmokeTest(void);
static test_status_t HashTableCount_Test(void);
static test_status_t HashTableEmpty_Test(void);
static test_status_t HashTableForEach_Test(void);
static test_status_t HashTableFind_Test(void);
static test_status_t HashTableLoadFactor_Test(void);
static test_status_t HashTableSD_Basic_Test(void);

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

	APPLY_TEST(HashTableCreateDestroy_SmokeTest());
	APPLY_TEST(HashTableInsert_SmokeTest());
	APPLY_TEST(HashTableRemove_SmokeTest());
	APPLY_TEST(HashTableCount_Test());
	APPLY_TEST(HashTableEmpty_Test());
	APPLY_TEST(HashTableForEach_Test());
	APPLY_TEST(HashTableFind_Test());
	APPLY_TEST(HashTableLoadFactor_Test());
	APPLY_TEST(HashTableSD_Basic_Test());
	
	return 0;
}

/* ==== HELPER FUNCTIONS ==== */

static size_t HashWithMod10(const void* data)
{
	return *(size_t*)data % 10;
}

static int IsMatchSizet(const void* one, const void* two)
{
	return (*(size_t*)one == *(size_t*)two);
}

/*
void FillHashTableWithArr(hash_table_t* table, int* arr, size_t size)
{
	while(0 < size)
	{
		HashTableInsert(table, arr);
		
		++arr;
		--size;
	}
}
*/

typedef struct planet {
	size_t planet_id;
	size_t amount_moons;
	size_t amount_secret_bases;
	size_t amount_aliens;
} planet_t;

static size_t HashPlanetIdWithMod10(const void* planet)
{
	return (((planet_t*)planet)->planet_id % 10);
}

static int IsMatchPlanets(const void* planet1, const void* planet2)
{
	return (((planet_t*)planet1)->planet_id == ((planet_t*)planet2)->planet_id);
}

static int AddSecretBaseToPlanet(void* planet, void* add_secret_bases)
{
	((planet_t*)planet)->amount_secret_bases += (size_t)*(int*)add_secret_bases;
	
	return 0;
}

static size_t index_add_to_arr = 0;
int AddToArrPlanetAmountSecretBases(void* planet, void* arr)
{
	*(((size_t*)arr) + index_add_to_arr) = ((planet_t*)planet)->amount_secret_bases;
	
	++index_add_to_arr;
	
	return 0;
}

static void PrintArr(const size_t* arr, size_t size)
{
	while(0 < size)
	{
		printf("%ld ", *arr);
		
		++arr;
		--size;
	}
	
	printf("\n");
}


/* ==== TEST FUNCTIONS ==== */

static test_status_t HashTableCreateDestroy_SmokeTest(void)
{
	hash_table_t* table = HashTableCreate(10, HashWithMod10, IsMatchSizet);
	
	HashTableDestroy(table);
	return TEST_STATUS_SUCCESS;
}

static test_status_t HashTableInsert_SmokeTest(void)
{
	hash_table_t* table = HashTableCreate(10, HashWithMod10, IsMatchSizet);
	size_t a = 5;
	
	HashTableInsert(table, &a);
	
	HashTableDestroy(table);
	return TEST_STATUS_SUCCESS;
}

static test_status_t HashTableRemove_SmokeTest(void)
{
	hash_table_t* table = HashTableCreate(10, HashWithMod10, IsMatchSizet);
	size_t a = 5;
	
	HashTableInsert(table, &a);
	HashTableRemove(table, &a);
	
	HashTableDestroy(table);
	return TEST_STATUS_SUCCESS;
}

static test_status_t HashTableCount_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	hash_table_t* table = HashTableCreate(10, HashWithMod10, IsMatchSizet);
	size_t a = 5;
	size_t b = 12;
	size_t c = 22;
	
	if (0 != HashTableCount(table))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HashTableInsert(table, &a);
	HashTableInsert(table, &b);
	
	if (2 != HashTableCount(table))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HashTableInsert(table, &c);
	
	if (3 != HashTableCount(table))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HashTableRemove(table, &a);
	
	if (2 != HashTableCount(table))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HashTableDestroy(table);
	return res;
}

static test_status_t HashTableEmpty_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	hash_table_t* table = HashTableCreate(10, HashWithMod10, IsMatchSizet);
	size_t a = 5;
	size_t b = 12;
	
	if (1 != HashTableIsEmpty(table))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HashTableInsert(table, &a);
	
	if (0 != HashTableIsEmpty(table))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HashTableInsert(table, &b);
	
	if (0 != HashTableIsEmpty(table))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	
	HashTableRemove(table, &a);
	
	if (0 != HashTableIsEmpty(table))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HashTableRemove(table, &b);
	
	if (1 != HashTableIsEmpty(table))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HashTableDestroy(table);
	return res;
}

static test_status_t HashTableForEach_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	hash_table_t* table = HashTableCreate(10, HashPlanetIdWithMod10, IsMatchPlanets);
	
	planet_t neptune = {14, 16, 3, 200};
	planet_t mars = {16, 2, 15, 14};
	
	int add_bases = 2;
	size_t arr_res[2] = {0};
	size_t arr_ans[] = {5,17};
	
	HashTableInsert(table, &neptune);
	HashTableInsert(table, &mars);
	
	HashTableForEach(table, AddSecretBaseToPlanet, &add_bases);
	
	index_add_to_arr = 0;
	HashTableForEach(table, AddToArrPlanetAmountSecretBases, arr_res);
	index_add_to_arr = 0;
	
	printf("You got: ");
	PrintArr(arr_res, 2);
	printf("Should have gotten: ");
	PrintArr(arr_ans, 2);

	if (0 != memcmp(arr_res, arr_ans, sizeof(int)*2))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HashTableDestroy(table);
	return res;
}

static test_status_t HashTableFind_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	hash_table_t* table = HashTableCreate(10, HashPlanetIdWithMod10, IsMatchPlanets);
	
	planet_t neptune = {14, 16, 3, 200};
	planet_t mars = {16, 2, 15, 14};
	planet_t jupiter = {26, 97, 7, 12};
	
	if (NULL != HashTableFind(table, &neptune))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HashTableInsert(table, &neptune);
	HashTableInsert(table, &mars);
	HashTableInsert(table, &jupiter);
	
	if (&mars != HashTableFind(table, &mars))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (&jupiter != HashTableFind(table, &jupiter))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (&mars != HashTableFind(table, &mars))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HashTableRemove(table, &mars);
	
	if (NULL != HashTableFind(table, &mars))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (&jupiter != HashTableFind(table, &jupiter))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HashTableDestroy(table);
	return res;
}

static test_status_t HashTableLoadFactor_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	hash_table_t* table = HashTableCreate(10, HashPlanetIdWithMod10, IsMatchPlanets);
	
	planet_t neptune = {14, 16, 3, 200};
	planet_t mars = {16, 2, 15, 14};
	planet_t jupiter = {26, 97, 7, 12};
	planet_t venus = {34, 0, 1, 9};
	
	HashTableInsert(table, &neptune);
	HashTableInsert(table, &mars);
	HashTableInsert(table, &jupiter);
	HashTableInsert(table, &venus);
	
	if (0.4 != HashTableLoadFactor(table))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HashTableRemove(table, &mars);
	HashTableRemove(table, &neptune);
	
	if (0.2 != HashTableLoadFactor(table))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HashTableDestroy(table);
	return res;
}

static test_status_t HashTableSD_Basic_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	hash_table_t* table = HashTableCreate(10, HashPlanetIdWithMod10, IsMatchPlanets);
	
	planet_t neptune = {14, 16, 3, 200};
	planet_t mars = {16, 2, 15, 14};
	planet_t jupiter = {26, 97, 7, 12};
	planet_t venus = {34, 0, 1, 9};
	
	double sd = 0;
	
	HashTableInsert(table, &neptune);
	HashTableInsert(table, &mars);
	HashTableInsert(table, &jupiter);
	HashTableInsert(table, &venus);
	
	sd = HashTableSD(table);
	
	if (sd != HashTableSD(table))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	HashTableDestroy(table);
	return res;
}






















