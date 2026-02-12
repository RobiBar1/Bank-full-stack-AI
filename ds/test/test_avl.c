#include <stddef.h> /* size_t */
#include <stdio.h> /* printf */
#include <vector.h> /* vector API */

#include "avl.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

#define APPLY_TEST(func_call) CheckTest(func_call, #func_call)

typedef enum {TEST_STATUS_SUCCESS, TEST_STATUS_FAILURE} test_status_t;

static void CheckTest(test_status_t result, char* name);

static test_status_t AVLCreate_SmokeTest(void);
static test_status_t AVLInsert_SmokeTest(void);
static test_status_t AVLInsert_Multiple_SmokeTest(void);
static test_status_t AVLRemove_SmokeTest(void);
static test_status_t AVLIsEmpty_Test(void);
static test_status_t AVLForEachIn_Test(void);
static test_status_t AVLCount_Basic_Test(void);
static test_status_t AVLFind_Test(void);
static test_status_t AVLInsert_Balanced_Preorder_Test(void);
static test_status_t AVLInsert_Balanced_Preorder2_Test(void);
static test_status_t AVLInsert_Balanced_Preorder3_Test(void);
static test_status_t AVLInsert_Balanced_Postorder_Test(void);
static test_status_t AVLRemoveTwoChildren_Balanced_Preorder_Test(void);
static test_status_t AVLRemoveHasOneChild_Balanced_Preorder_Test(void);
static test_status_t AVLRemoveLeaf_Balanced_Preorder_Test(void);
static test_status_t AVLHeight_Test(void);
static test_status_t AVLMultiFind_Test(void);
static test_status_t AVLMultiRemove_Basic_Test(void);
static test_status_t AVLMultiRemove_Test(void);

/* gd test_avl.c ../src/avl.c  ../src/vector.c -I ../include/ */

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
	
	APPLY_TEST(AVLCreate_SmokeTest());
	APPLY_TEST(AVLInsert_SmokeTest());
	APPLY_TEST(AVLInsert_Multiple_SmokeTest());
	APPLY_TEST(AVLRemove_SmokeTest());
	APPLY_TEST(AVLIsEmpty_Test());
	APPLY_TEST(AVLForEachIn_Test());
	APPLY_TEST(AVLCount_Basic_Test());
	APPLY_TEST(AVLFind_Test());
	APPLY_TEST(AVLInsert_Balanced_Preorder_Test());
	APPLY_TEST(AVLInsert_Balanced_Preorder2_Test());
	APPLY_TEST(AVLInsert_Balanced_Preorder3_Test());
	APPLY_TEST(AVLInsert_Balanced_Postorder_Test());
	APPLY_TEST(AVLRemoveTwoChildren_Balanced_Preorder_Test());
	APPLY_TEST(AVLRemoveHasOneChild_Balanced_Preorder_Test());
	APPLY_TEST(AVLRemoveLeaf_Balanced_Preorder_Test());
	APPLY_TEST(AVLHeight_Test());
	APPLY_TEST(AVLMultiFind_Test());
	APPLY_TEST(AVLMultiRemove_Basic_Test());
	APPLY_TEST(AVLMultiRemove_Test());
	
	return 0;
}

/* ==== HELPER FUNCTIONS ==== */

int CompareInts(const void* one, const void* two)
{
	return (*(int*)one - *(int*)two);
}

int MatchIntIsAtMost2AwayFromParam(const void* data, const void* param)
{
	int ans =  (*(int*)data - *(int*)param);
	
	return (ans <= 2 && ans >= -2);
}

static size_t index_add_to_arr = 0;
int AddToArr(const void* data, void* arr)
{
	*(((int*)arr) + index_add_to_arr) = *(int*)data;
	
	++index_add_to_arr;
	
	return 0;
}

void FillAVLWithArr(avl_t* avl, int* arr, size_t size)
{
	while(0 < size)
	{
		AVLInsert(avl, arr);
		
		++arr;
		--size;
	}
}

void PrintArr(const int* arr, size_t size)
{
	while(0 < size)
	{
		printf("%d ", *arr);
		
		++arr;
		--size;
	}
	
	printf("\n");
}

int ArraysAreEqual(const int* arr1, const int* arr2, size_t size)
{
	while (0 < size)
	{
		if (*arr1 != *arr2)
		{
			return 0;
		}
		
		--size;
		++arr1;
		++arr2;
	}
	
	return 1;
}

int ArrayAndVectorMatch(const int* arr, size_t size, const vector_t* vector)
{
	size_t i = 0;
	
	while (i < size)
	{
		if (*(int*)VectorGetAccess(vector, i) != arr[i])
		{
			return 0;
		}
		
		++i;
	}
	
	return 1;
}

/* ==== TEST FUNCTIONS ==== */


static test_status_t AVLCreate_SmokeTest(void)
{
	avl_t* avl = AVLCreate(&CompareInts);
	AVLDestroy(avl);
	
	return TEST_STATUS_SUCCESS;
}

static test_status_t AVLInsert_SmokeTest(void)
{
	avl_t* avl = AVLCreate(&CompareInts);
	int a = 8;
	
	AVLInsert(avl, &a);
	
	AVLDestroy(avl);
	return TEST_STATUS_SUCCESS;
}

static test_status_t AVLInsert_Multiple_SmokeTest(void)
{
	avl_t* avl = AVLCreate(&CompareInts);
	int a = 8;
	int b = 4;
	int c = 7;
	int d = 5;
	int e = 10;
	
	AVLInsert(avl, &a);
	AVLInsert(avl, &b);
	AVLInsert(avl, &c);
	AVLInsert(avl, &d);
	AVLInsert(avl, &e);
	
	AVLDestroy(avl);
	return TEST_STATUS_SUCCESS;
}

static test_status_t AVLRemove_SmokeTest(void)
{
	avl_t* avl = AVLCreate(&CompareInts);
	int a = 8;
	int b = 4;
	int c = 7;
	int d = 5;
	
	AVLInsert(avl, &a);
	AVLInsert(avl, &b);
	AVLInsert(avl, &c);
	AVLInsert(avl, &d);
	
	AVLRemove(avl, &c);
	
	AVLDestroy(avl);
	return TEST_STATUS_SUCCESS;
}

static test_status_t AVLIsEmpty_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	avl_t* avl = AVLCreate(&CompareInts);
	int a = 8;
	int b = 4;
	
	if (1 != AVLIsEmpty(avl))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	AVLInsert(avl, &a);
	
	if (0 != AVLIsEmpty(avl))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	AVLInsert(avl, &b);
	
	if (0 != AVLIsEmpty(avl))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	AVLRemove(avl, &a);
	
	if (0 != AVLIsEmpty(avl))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	AVLRemove(avl, &b);
	
	if (1 != AVLIsEmpty(avl))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	AVLDestroy(avl);
	return res;
}

static test_status_t AVLForEachIn_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	avl_t* avl = AVLCreate(&CompareInts);
	size_t size = 7;
	int arr_fill_avl[7] = {5,7,9,3,4,1,8};
	int arr_get_res[7] = {0};
	int arr_ans[] = {1,3,4,5,7,8,9};
	
	FillAVLWithArr(avl, arr_fill_avl, size);
	
	index_add_to_arr = 0;
	AVLForEach(avl, AddToArr, arr_get_res, IN);
	index_add_to_arr = 0;
	
	PrintArr(arr_get_res, size);
	
	if (1 != ArraysAreEqual(arr_get_res, arr_ans, size))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	AVLDestroy(avl);
	return res;
}

static test_status_t AVLCount_Basic_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	avl_t* avl = AVLCreate(&CompareInts);
	int a = 8;
	int b = 4;
	int c = 7;
	int d = 5;
	
	if (0 != AVLCount(avl))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	AVLInsert(avl, &a);
	AVLInsert(avl, &b);
	
	if (2 != AVLCount(avl))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	AVLInsert(avl, &c);
	AVLInsert(avl, &d);
	
	if (4 != AVLCount(avl))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	AVLRemove(avl, &c);
	
	if (3 != AVLCount(avl))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	AVLDestroy(avl);
	return res;
}

static test_status_t AVLFind_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	avl_t* avl = AVLCreate(&CompareInts);
	int a = 8;
	int b = 4;
	int c = 7;
	int d = 5;
	
	AVLInsert(avl, &a);
	AVLInsert(avl, &b);
	
	if (b != *(int*)AVLFind(avl, &b))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (NULL != AVLFind(avl, &c))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	AVLInsert(avl, &c);
	AVLInsert(avl, &d);
	
	if (c != *(int*)AVLFind(avl, &c))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	AVLRemove(avl, &b);
	
	if (NULL != AVLFind(avl, &b))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	AVLDestroy(avl);
	return res;
}

static test_status_t AVLInsert_Balanced_Preorder_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	avl_t* avl = AVLCreate(&CompareInts);
	size_t size = 4;
	int arr_fill_avl[] = {8,4,7,5};
	int arr_get_res[4] = {0};
	int arr_ans[] = {7,4,5,8};
	
	FillAVLWithArr(avl, arr_fill_avl, size);
	
	index_add_to_arr = 0;
	AVLForEach(avl, AddToArr, arr_get_res, PRE);
	index_add_to_arr = 0;
	
	PrintArr(arr_get_res, size);
	
	if (1 != ArraysAreEqual(arr_get_res, arr_ans, size))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	AVLDestroy(avl);
	return res;
}

static test_status_t AVLInsert_Balanced_Preorder2_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	avl_t* avl = AVLCreate(&CompareInts);
	size_t size = 3;
	int arr_fill_avl[] = {8,4,7};
	int arr_get_res[3] = {0};
	int arr_ans[] = {7,4,8};
	
	FillAVLWithArr(avl, arr_fill_avl, size);
	
	index_add_to_arr = 0;
	AVLForEach(avl, AddToArr, arr_get_res, PRE);
	index_add_to_arr = 0;
	
	PrintArr(arr_get_res, size);
	
	if (1 != ArraysAreEqual(arr_get_res, arr_ans, size))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	AVLDestroy(avl);
	return res;
}

static test_status_t AVLInsert_Balanced_Preorder3_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	avl_t* avl = AVLCreate(&CompareInts);
	size_t size = 12;
	int arr_fill_avl[] = {10,15,18,20,25,5,3,12,14,11,1,6};
	int arr_get_res[12] = {0};
	int arr_ans[] = {10,3,1,5,6,15,12,11,14,20,18,25};
	
	FillAVLWithArr(avl, arr_fill_avl, size);
	
	index_add_to_arr = 0;
	AVLForEach(avl, AddToArr, arr_get_res, PRE);
	index_add_to_arr = 0;
	
	PrintArr(arr_get_res, size);
	
	if (1 != ArraysAreEqual(arr_get_res, arr_ans, size))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	AVLDestroy(avl);
	return res;
}

static test_status_t AVLInsert_Balanced_Postorder_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	avl_t* avl = AVLCreate(&CompareInts);
	size_t size = 12;
	int arr_fill_avl[] = {10,15,18,20,25,5,3,12,14,11,1,6};
	int arr_get_res[12] = {0};
	int arr_ans[] = {1,6,5,3,11,14,12,18,25,20,15,10};
	
	FillAVLWithArr(avl, arr_fill_avl, size);
	
	index_add_to_arr = 0;
	AVLForEach(avl, AddToArr, arr_get_res, POST);
	index_add_to_arr = 0;
	
	PrintArr(arr_get_res, size);
	
	if (1 != ArraysAreEqual(arr_get_res, arr_ans, size))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	AVLDestroy(avl);
	return res;
}

static test_status_t AVLRemoveTwoChildren_Balanced_Preorder_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	avl_t* avl = AVLCreate(&CompareInts);
	int a = 8;
	int b = 4;
	int c = 7;
	int d = 5;
	int e = 12;
	int f = 6;
	int g = 3;
	
	size_t size = 6;
	int arr_get_res[6] = {0};
	int arr_ans[] = {5,4,3,8,6,12};
	
	AVLInsert(avl, &a);
	AVLInsert(avl, &b);
	AVLInsert(avl, &c);
	AVLInsert(avl, &d);
	AVLInsert(avl, &e);
	AVLInsert(avl, &f);
	AVLInsert(avl, &g);
	
	AVLRemove(avl, &c);
	
	index_add_to_arr = 0;
	AVLForEach(avl, AddToArr, arr_get_res, PRE);
	index_add_to_arr = 0;
	
	printf("[Note: If you did 'remove' differently, \
then update 'arr_ans' to the preorder solution you're looking for]\n");
	PrintArr(arr_get_res, size);
	
	if (1 != ArraysAreEqual(arr_get_res, arr_ans, size))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	AVLDestroy(avl);
	return res;
}

static test_status_t AVLRemoveHasOneChild_Balanced_Preorder_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	avl_t* avl = AVLCreate(&CompareInts);
	int a = 8;
	int b = 4;
	int c = 7;
	int d = 5;
	int e = 12;
	int f = 6;
	int g = 3;
	
	size_t size = 6;
	int arr_get_res[6] = {0};
	int arr_ans[] = {5,4,3,7,6,12};
	
	AVLInsert(avl, &a);
	AVLInsert(avl, &b);
	AVLInsert(avl, &c);
	AVLInsert(avl, &d);
	AVLInsert(avl, &e);
	AVLInsert(avl, &f);
	AVLInsert(avl, &g);
	
	AVLRemove(avl, &a);
	
	index_add_to_arr = 0;
	AVLForEach(avl, AddToArr, arr_get_res, PRE);
	index_add_to_arr = 0;
	
	printf("[Note: If you did 'remove' differently, \
then update 'arr_ans' to the preorder solution you're looking for]\n");
	
	PrintArr(arr_get_res, size);
	
	if (1 != ArraysAreEqual(arr_get_res, arr_ans, size))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	AVLDestroy(avl);
	return res;
}

static test_status_t AVLRemoveLeaf_Balanced_Preorder_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	avl_t* avl = AVLCreate(&CompareInts);
	int a = 8;
	int b = 4;
	int c = 7;
	int d = 5;
	int e = 12;
	int f = 6;
	int g = 3;
	
	size_t size = 6;
	int arr_get_res[6] = {0};
	int arr_ans[] = {5,4,3,7,6,8};
	
	AVLInsert(avl, &a);
	AVLInsert(avl, &b);
	AVLInsert(avl, &c);
	AVLInsert(avl, &d);
	AVLInsert(avl, &e);
	AVLInsert(avl, &f);
	AVLInsert(avl, &g);
	
	AVLRemove(avl, &e);
	
	index_add_to_arr = 0;
	AVLForEach(avl, AddToArr, arr_get_res, PRE);
	index_add_to_arr = 0;
	
	printf("[Note: If you did 'remove' differently, \
then update 'arr_ans' to the preorder solution you're looking for]\n");
	
	PrintArr(arr_get_res, size);
	
	if (1 != ArraysAreEqual(arr_get_res, arr_ans, size))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	AVLDestroy(avl);
	return res;
}

static test_status_t AVLHeight_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	avl_t* avl = AVLCreate(&CompareInts);
	int a = 8;
	int b = 4;
	int c = 7;
	int d = 5;
	int e = 12;
	int f = 6;
	int g = 3;
	
	AVLInsert(avl, &a);
	AVLInsert(avl, &b);
	AVLInsert(avl, &c);
	AVLInsert(avl, &d);
	
	if (2 != AVLHeight(avl))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	AVLInsert(avl, &e);
	AVLInsert(avl, &f);
	
	if (2 != AVLHeight(avl))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	AVLInsert(avl, &g);
	
	if (3 != AVLHeight(avl))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	AVLRemove(avl, &e);
	
	if (2 != AVLHeight(avl))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	AVLDestroy(avl);
	return res;
}

static test_status_t AVLMultiFind_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	avl_t* avl = AVLCreate(&CompareInts);
	size_t size = 12;
	int arr_fill_avl[] = {10,15,18,20,25,5,3,12,14,11,1,6};
	vector_t* vector = VectorCreate(size, sizeof(int));
	int arr_ans[] = {3,5,6};
	
	int param = 5;
	
	FillAVLWithArr(avl, arr_fill_avl, size);
	
	AVLMultiFind(avl, MatchIntIsAtMost2AwayFromParam, &param, vector);
	
	if (3 != VectorGetSize(vector))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (!ArrayAndVectorMatch(arr_ans, 3, vector))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	VectorDestroy(vector);
	AVLDestroy(avl);
	return res;
}

static test_status_t AVLMultiRemove_Basic_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	avl_t* avl = AVLCreate(&CompareInts);
	int a = 8;
	int b = 1;
	int c = 16;
	int param = 7;
	
	AVLInsert(avl, &a);
	AVLInsert(avl, &b);
	AVLInsert(avl, &c);
	
	if (NULL == AVLFind(avl, &a))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	MultiRemove(avl, MatchIntIsAtMost2AwayFromParam, &param);
	
	if (NULL == AVLFind(avl, &b))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (NULL == AVLFind(avl, &c))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (NULL != AVLFind(avl, &a))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	AVLDestroy(avl);
	return res;
}

static test_status_t AVLMultiRemove_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	avl_t* avl = AVLCreate(&CompareInts);
	int a = 8;
	int b = 1;
	int c = 16;
	int d = 5;
	int e = 2;
	int f = 4;
	int g = 3;
	int param = 5;
	
	AVLInsert(avl, &a);
	AVLInsert(avl, &b);
	AVLInsert(avl, &c);
	AVLInsert(avl, &d);
	AVLInsert(avl, &e);
	AVLInsert(avl, &f);
	AVLInsert(avl, &g);
	
	MultiRemove(avl, MatchIntIsAtMost2AwayFromParam, &param);
	
	if (NULL == AVLFind(avl, &b))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (NULL != AVLFind(avl, &d))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (NULL != AVLFind(avl, &f))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (NULL != AVLFind(avl, &g))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (NULL != AVLFind(avl, &g))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (4 != AVLCount(avl))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	AVLDestroy(avl);
	return res;
}













