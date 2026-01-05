#include <stdio.h> /* printf */
#include <assert.h> /* assert */

#include "singly_linked_list.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

typedef enum {TEST_STATUS_SUCCESS, TEST_STATUS_FAILURE} test_status_t;

/* gd test_singly_linked_list.c ../src/singly_linked_list.c -I ../include/ */

static void CheckTest(test_status_t result, char* name);

static slist_iter_t AdvanceIterBy(slist_iter_t iter, size_t num);
static void FillListWithIntArr(slist_t* slist, const int arr[], size_t arr_length);
static int IsMatchIntArrayWithSList(slist_t* slist, const int arr[], size_t arr_length);

static test_status_t SListCreate_UnitTest(void);
static test_status_t SListBeginEnd_BasicTest(void);
static test_status_t SListInsert_UnitTest(void);
static test_status_t SListInsert_UnitTest(void);
static test_status_t SListInsertMultiple_Test(void);
static test_status_t SListRemove_UnitTest(void);
static test_status_t SListRemoveMultiple_Test(void);
static test_status_t SListIsEmpty_Test(void);
static test_status_t SListSetData_Test(void);
static test_status_t SListFind_Test(void);
static test_status_t SListCount_Test(void);
static test_status_t SListForEach_Test(void);
static test_status_t SListIsIterEqual_Test(void);
static test_status_t SListRemoveLast_Test(void);
static test_status_t SListAppend_Test(void);
static test_status_t SListAppendWithEmpty_Test(void);

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

	CheckTest(SListCreate_UnitTest(), "SListCreate_UnitTest");
	CheckTest(SListBeginEnd_BasicTest(), "SListBeginEnd_BasicTest");
	CheckTest(SListInsert_UnitTest(), "SListInsert_UnitTest");
	CheckTest(SListInsertMultiple_Test(), "SListInsertMultiple_Test");
	CheckTest(SListRemove_UnitTest(), "SListRemove_UnitTest");
	CheckTest(SListRemoveMultiple_Test(), "SListRemoveMultiple_Test");
	CheckTest(SListIsEmpty_Test(), "SListIsEmpty_Test");
	CheckTest(SListSetData_Test(), "SListSetData_Test");
	CheckTest(SListFind_Test(), "SListFind_Test");
	CheckTest(SListCount_Test(), "SListCount_Test");
	CheckTest(SListForEach_Test(), "SListForEach_Test");
	CheckTest(SListIsIterEqual_Test(), "SListIsIterEqual_Test");
	CheckTest(SListRemoveLast_Test(), "SListRemoveLast_Test");
	CheckTest(SListAppend_Test(), "SListAppend_Test");
	CheckTest(SListAppendWithEmpty_Test(), "SListAppendWithEmpty_Test");
	
	return 0;
}

/* == HELPER FUNCTIONS == */

static slist_iter_t AdvanceIterBy(slist_iter_t iter, size_t num)
{
	while (0 != num)
	{
		iter = SListNext(iter);
		--num;
	}
	
	return iter;
}

static void FillListWithIntArr(slist_t* slist, const int arr[], size_t arr_length)
{
	slist_iter_t iter = SListBegin(slist);
	size_t index = 0;
	
	while (index < arr_length)
	{
		iter = SListInsert(slist, iter, (void*)&arr[index]); 
		iter = SListNext(iter);
		++index;
	}
}

static int IsMatchIntArrayWithSList(slist_t* slist, const int arr[], size_t arr_length)
{
	slist_iter_t iter = SListBegin(slist);
	
	assert(SListCount(slist) == arr_length);
	
	while (0 < arr_length)
	{
		if (*arr != *(int*)SListGetData(iter))
		{
			return 0;
		}
		
		iter = SListNext(iter);
		++arr;
		--arr_length;
	}
	
	return 1;
}

/* == TEST FUNCTIONS == */

static test_status_t SListCreate_UnitTest(void)
{
	slist_t* slist = SListCreate();
	SListDestroy(slist);

	return TEST_STATUS_SUCCESS;
}

static test_status_t SListBeginEnd_BasicTest(void)
{
	slist_t* slist = SListCreate();
	test_status_t res = TEST_STATUS_SUCCESS;
	
	if (SListBegin(slist) != SListEnd(slist) )
	{
		res = TEST_STATUS_FAILURE;
	}
	
	SListDestroy(slist);

	return res;
}

static test_status_t SListInsert_UnitTest(void)
{
	slist_t* slist = SListCreate();
	slist_iter_t start = SListBegin(slist);
	
	int a = 5;
	
	SListInsert(slist, start, &a);
	
	SListDestroy(slist);

	return TEST_STATUS_SUCCESS;
}

static test_status_t SListInsertMultiple_Test(void)
{
	slist_t* slist = SListCreate();
	test_status_t res = TEST_STATUS_SUCCESS;
	slist_iter_t iter = SListBegin(slist);
	
	int a = 5;
	int b = 7;
	int c = 9;
	
	SListInsert(slist, iter, (void*)&a); /* 5 */
	SListInsert(slist, iter, (void*)&b); /* 7 5 */
	iter = SListNext(iter);
	SListInsert(slist, iter, (void*)&c); /* 7 9 5 */
	
	iter = SListBegin(slist);
	
	if (b != *(int*)SListGetData(iter))
	{
		res = TEST_STATUS_FAILURE;
	}
	iter = SListNext(iter);
	
	if (c != *(int*)SListGetData(iter))
	{
		res = TEST_STATUS_FAILURE;
	}
	iter = SListNext(iter);
	
	if (a != *(int*)SListGetData(iter))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	SListDestroy(slist);

	return res;
}

static test_status_t SListRemove_UnitTest(void)
{
	slist_t* slist = SListCreate();
	slist_iter_t start = SListBegin(slist);
	
	int a = 5;
	
	SListInsert(slist, start, &a);
	SListRemove(start);
	
	SListDestroy(slist);

	return TEST_STATUS_SUCCESS;
}

static test_status_t SListRemoveMultiple_Test(void)
{
	slist_t* slist = SListCreate();
	test_status_t res = TEST_STATUS_SUCCESS;
	slist_iter_t iter = SListBegin(slist);
	
	int a = 5;
	int b = 7;
	int c = 9;
	int d = 12;
	
	SListInsert(slist, iter, (void*)&a); /* 5 */
	iter = SListNext(iter);
	SListInsert(slist, iter, (void*)&b); /* 5 7 */
	iter = SListNext(iter);
	SListInsert(slist, iter, (void*)&c); /* 5 7 9 */
	iter = SListNext(iter);
	SListInsert(slist, iter, (void*)&d); /* 5 7 9 12 */
	
	SListRemove(SListBegin(slist)); /* 7 9 12 */
	
	iter = SListBegin(slist);
	
	if (b != *(int*)SListGetData(iter))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	iter = SListNext(iter);
	iter = SListNext(iter);
	
	SListRemove(iter); /* 7 9 */
	
	iter = SListBegin(slist);
	
	if (b != *(int*)SListGetData(iter))
	{
		res = TEST_STATUS_FAILURE;
	}
	iter = SListNext(iter);
	
	if (c != *(int*)SListGetData(iter))
	{
		res = TEST_STATUS_FAILURE;
	}
	iter = SListNext(iter);
	
	if (iter != SListEnd(slist))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	SListDestroy(slist);

	return res;
}

static test_status_t SListRemoveLast_Test(void)
{
	slist_t* slist = SListCreate();
	test_status_t res = TEST_STATUS_SUCCESS;
	slist_iter_t iter = SListBegin(slist);
	
	int a = 5;
	int b = 7;
	int c = 9;
	int d = 12;
	int d_new = 14;
	
	SListInsert(slist, iter, (void*)&a); /* 5 */
	iter = SListNext(iter);
	SListInsert(slist, iter, (void*)&b); /* 5 7 */
	iter = SListNext(iter);
	SListInsert(slist, iter, (void*)&c); /* 5 7 9 */
	iter = SListNext(iter);
	SListInsert(slist, iter, (void*)&d); /* 5 7 9 12 */
	
	SListRemove(iter); /* 5 7 9 */
	iter = SListBegin(slist);
	iter = SListNext(SListNext(iter));
	
	if (c != *(int*)SListGetData(iter))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	SListInsert(slist, iter, (void*)&d_new); /* 5 7 9 14 */
	
	if (d_new != *(int*)SListGetData(iter))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	SListRemove(iter); /* 5 7 9 */
	iter = SListBegin(slist);
	iter = SListNext(SListNext(iter));
	SListRemove(iter); /* 5 7 */
	
	if (1 != SListIsIterEqual(iter, SListEnd(slist)))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	iter = SListBegin(slist);
	
	if (a != *(int*)SListGetData(iter))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	iter = SListNext(iter);
	if (b != *(int*)SListGetData(iter))
	{
		res = TEST_STATUS_FAILURE;
	}
	SListRemove(iter); /* 5 */
	
	if (1 != SListIsIterEqual(iter, SListEnd(slist)))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	SListDestroy(slist);

	return res;
}

static test_status_t SListIsEmpty_Test(void)
{
	slist_t* slist = SListCreate();
	test_status_t res = TEST_STATUS_SUCCESS;
	slist_iter_t iter = SListBegin(slist);
	
	int a = 5;
	int b = 7;
	int c = 9;
	int d = 12;
	
	if (1 != SListIsEmpty(slist))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	SListInsert(slist, iter, (void*)&a); /* 5 */
	iter = SListNext(iter);
	
	if (0 != SListIsEmpty(slist))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	SListInsert(slist, iter, (void*)&b); /* 5 7 */
	iter = SListNext(iter);
	SListInsert(slist, iter, (void*)&c); /* 5 7 9 */
	iter = SListNext(iter);
	SListInsert(slist, iter, (void*)&d); /* 5 7 9 12 */
	
	SListRemove(SListBegin(slist)); /* 7 9 12 */
	
	SListRemove(SListBegin(slist)); /* 7 9 */
	
	SListRemove(SListBegin(slist)); /* 9 */
	
	if (0 != SListIsEmpty(slist))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	SListRemove(SListBegin(slist)); /* */
	
	if (1 != SListIsEmpty(slist))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	SListDestroy(slist);

	return res;
}

static test_status_t SListSetData_Test(void)
{
	slist_t* slist = SListCreate();
	test_status_t res = TEST_STATUS_SUCCESS;
	slist_iter_t iter = SListBegin(slist);
	
	int a = 5;
	int b = 7;
	int a_new = 9;
	int b_new = 12;
	
	iter = SListInsert(slist, iter, (void*)&a); /* 5 */
	iter = SListInsert(slist, iter, (void*)&b); /* 7 5 */
	iter = SListNext(iter);
	
	SListSetData(iter, &a_new); /* 7 9 */

	if (a_new != *(int*)SListGetData(iter))
	{
		res = TEST_STATUS_FAILURE;
	}
	if (b != *(int*)SListGetData(SListBegin(slist)))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	iter = SListBegin(slist);
	SListSetData(iter, &b_new); /* 12 9 */
	
	if (b_new != *(int*)SListGetData(iter))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	SListDestroy(slist);

	return res;
}


static int IntsAreEqual(const void* num1, void* num2)
{
	return (*(int*)num1 == *(int*)num2);
}

static test_status_t SListFind_Test(void)
{
	slist_t* slist = SListCreate();
	test_status_t res = TEST_STATUS_SUCCESS;
	slist_iter_t iter = SListBegin(slist);
	slist_iter_t from = NULL;
	slist_iter_t to = NULL;
	slist_iter_t ans = NULL;
	
	int arr[] = {2, 4, 14, 8, 5, 4 , 9, 5, 12, 5, 6, 7, 3, 6};
	size_t arr_length = 14;
	int search_for = 5;
	
	FillListWithIntArr(slist, arr, arr_length);
	
	from = AdvanceIterBy(SListBegin(slist), 2);
	to = AdvanceIterBy(SListBegin(slist), 8);
	ans = SListFind(from, to, (void*)&search_for, &IntsAreEqual);
	
	if (5 != *(int*)SListGetData(ans))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	iter = SListNext(ans);
	
	if (4 != *(int*)SListGetData(iter))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	SListDestroy(slist);

	return res;
}

static test_status_t SListCount_Test(void)
{
	slist_t* slist = SListCreate();
	test_status_t res = TEST_STATUS_SUCCESS;
	
	int arr[] = {2, 4, 14, 8, 5, 4 , 9, 5, 12, 5, 6, 7, 3, 6};
	size_t arr_length = 14;
	
	if (0 != SListCount(slist))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	FillListWithIntArr(slist, arr, arr_length);
	
	if (arr_length != SListCount(slist))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	SListRemove(SListBegin(slist));
	SListRemove(SListBegin(slist));
	
	if (arr_length-2 != SListCount(slist))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	SListDestroy(slist);

	return res;
}



static int AddNumToNum(void* data, void* param)
{
	if (NULL == data || NULL == param)
	{
		return 1;
	}
	*(int*)data = *(int*)data + *(int*)param;
	
	return 0;
}

static test_status_t SListForEach_Test(void)
{
	slist_t* slist = SListCreate();
	test_status_t res = TEST_STATUS_SUCCESS;
	slist_iter_t from = NULL;
	slist_iter_t to = NULL;
	slist_iter_t checker = NULL;
	
	int arr[] = {2, 4, 14, 8, 5, 4 , 9, 5, 12, 5, 6, 7, 3, 6};
	size_t arr_length = 14;
	int add_amount = 2;
	
	FillListWithIntArr(slist, arr, arr_length);
	
	from = AdvanceIterBy(SListBegin(slist), 2);
	to = AdvanceIterBy(SListBegin(slist), 8);
	
	SListForEach(from, to, &AddNumToNum, &add_amount);
	
	checker = AdvanceIterBy(SListBegin(slist), 1);
	if (4 != *(int*)SListGetData(checker))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	checker = AdvanceIterBy(SListBegin(slist), 2);
	if (16 != *(int*)SListGetData(checker))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	checker = AdvanceIterBy(SListBegin(slist), 3);
	if (10 != *(int*)SListGetData(checker))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	checker = AdvanceIterBy(SListBegin(slist), 7);
	if (7 != *(int*)SListGetData(checker))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	checker = AdvanceIterBy(SListBegin(slist), 8);
	if (12 != *(int*)SListGetData(checker))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	SListDestroy(slist);

	return res;
}

static test_status_t SListIsIterEqual_Test(void)
{
	slist_t* slist1 = SListCreate();
	slist_t* slist2 = SListCreate();
	test_status_t res = TEST_STATUS_SUCCESS;
	slist_iter_t checker1 = NULL;
	slist_iter_t checker2 = NULL;
	
	int arr[] = {2, 4, 14, 8, 5, 4 , 9, 5, 12, 5, 6, 7, 3, 6};
	size_t arr_length = 14;
	
	FillListWithIntArr(slist1, arr, arr_length);
	FillListWithIntArr(slist2, arr, arr_length/2);
	
	checker1 = AdvanceIterBy(SListBegin(slist1), 3);
	checker2 = AdvanceIterBy(SListBegin(slist2), 3);
	if (0 != SListIsIterEqual(checker1, checker2))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	checker2 = AdvanceIterBy(SListBegin(slist1), 3);
	if (1 != SListIsIterEqual(checker1, checker2))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	checker2 = AdvanceIterBy(SListBegin(slist1), 4);
	if (0 != SListIsIterEqual(checker1, checker2))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	SListDestroy(slist1);
	SListDestroy(slist2);

	return res;
}

static test_status_t SListAppend_Test(void)
{
	slist_t* dest = SListCreate();
	slist_t* src = SListCreate();
	test_status_t res = TEST_STATUS_SUCCESS;
	
	int arr1[] = {1,2,3};
	size_t arr_length1 = 3;
	
	int arr2[] = {4,5,6};
	size_t arr_length2 = 3;
	
	int arr_appended[] = {1,2,3,4,5,6};
	
	int a = 7;
	int b = 12;
	
	int dest_after_insert[] = {1,2,3,4,5,6,7};
	int src_after_insert[] = {12};
	
	FillListWithIntArr(dest, arr1, arr_length1);
	FillListWithIntArr(src, arr2, arr_length2);
	
	SListAppend(dest, src);
	
	if (1 != SListIsEmpty(src))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (6 != SListCount(dest))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (1 != IsMatchIntArrayWithSList(dest, arr_appended, 6))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	
	SListInsert(dest, SListEnd(dest), (void*)&a); 
	SListInsert(src, SListEnd(src), (void*)&b); 
	
	if (7 != SListCount(dest))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (1 != SListCount(src))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (1 != IsMatchIntArrayWithSList(dest, dest_after_insert, 7))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (1 != IsMatchIntArrayWithSList(dest, dest_after_insert, 7))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (1 != IsMatchIntArrayWithSList(src, src_after_insert, 1))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	SListDestroy(dest);
	SListDestroy(src);

	return res;
}

static test_status_t SListAppendWithEmpty_Test(void)
{
	slist_t* dest = SListCreate();
	slist_t* src = SListCreate();
	test_status_t res = TEST_STATUS_SUCCESS;
	
	int arr1[] = {1,2,3};
	size_t arr_length1 = 3;
	
	int a = 4;
	int b = 12;
	
	int arr1_after[] = {1,2,3,4};
	int arr2_after[] = {12};
	
	FillListWithIntArr(dest, arr1, arr_length1);
	
	SListAppend(dest, src);
	
	if (1 != SListIsEmpty(src))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (3 != SListCount(dest))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (1 != IsMatchIntArrayWithSList(dest, arr1, 3))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	
	SListInsert(dest, SListEnd(dest), (void*)&a); 
	SListInsert(src, SListEnd(src), (void*)&b); 
	
	if (4 != SListCount(dest))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (1 != SListCount(src))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (1 != IsMatchIntArrayWithSList(dest, arr1_after, 4))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (1 != IsMatchIntArrayWithSList(src, arr2_after, 1))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	SListDestroy(dest);
	SListDestroy(src);

	return res;
}







