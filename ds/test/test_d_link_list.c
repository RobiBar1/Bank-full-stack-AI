#include <stdio.h> /* printf */
#include <assert.h> /* assert */

#include "d_link_list.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

typedef enum {TEST_STATUS_SUCCESS, TEST_STATUS_FAILURE} test_status_t;

/* gd test_d_link_list.c ../src/d_link_list.c -I ../include/ */

/* ==== FORWARD DELARATIONS ==== */

static void CheckTest(test_status_t result, char* name);

static dlist_iter_t AdvanceIterBy(dlist_iter_t iter, size_t num);
static void FillListWithIntArr(dlist_t* dlist, const int arr[], size_t arr_length);
static int IsMatchIntArrayWithDList(dlist_t* dlist, const int arr[], size_t arr_length);

static test_status_t DListCreate_SmokeTest(void);
static test_status_t DListInsert_SimpleTest(void);
static test_status_t DListRemove_Test(void);
static test_status_t DListRemove_MakeEmpty_Test(void);
static test_status_t DListPushPopFront_Test(void);
static test_status_t DListPushPopBack_Test(void);
static test_status_t DListFind_Test(void);
static test_status_t DListCount_Test(void);
static test_status_t DListSplice_Test(void);
static test_status_t DListSplice_WholeList_Test(void);
static test_status_t DListSplice_IntoStart_Test(void);
static test_status_t DListSplice_IntoEnd_Test(void);
static test_status_t DListSplice_JustTwo_Test(void);
static test_status_t DListSplice_JustOne_Test(void);
static test_status_t DListSplice_FromAndToSame_Test(void);
static test_status_t DListMultiFind_Test(void);


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

	CheckTest(DListCreate_SmokeTest(), "DListCreate_SmokeTest");
	CheckTest(DListInsert_SimpleTest(), "DListInsert_SimpleTest");
	CheckTest(DListRemove_Test(), "DListRemove_Test");
	CheckTest(DListRemove_MakeEmpty_Test(), "DListRemove_MakeEmpty_Test");
	CheckTest(DListPushPopFront_Test(), "DListPushPopFront_Test");
	CheckTest(DListPushPopBack_Test(), "DListPushPopBack_Test");
	CheckTest(DListFind_Test(), "DListFind_Test");
	CheckTest(DListCount_Test(), "DListCount_Test");
	CheckTest(DListSplice_Test(), "DListSplice_Test");
	CheckTest(DListSplice_WholeList_Test(), "DListSplice_WholeList_Test");
	CheckTest(DListSplice_IntoStart_Test(), "DListSplice_IntoStart_Test");
	CheckTest(DListSplice_IntoEnd_Test(), "DListSplice_IntoEnd_Test");
	CheckTest(DListSplice_JustTwo_Test(), "DListSplice_JustTwo_Test");
	CheckTest(DListSplice_JustOne_Test(), "DListSplice_JustOne_Test");
	CheckTest(DListSplice_FromAndToSame_Test(), "DListSplice_FromAndToSame_Test");
	CheckTest(DListMultiFind_Test(), "DListMultiFind_Test");
	
	return 0;
}

/* ==== HELPER FUNCTIONS ==== */


static dlist_iter_t AdvanceIterBy(dlist_iter_t iter, size_t num)
{
	while (0 != num)
	{
		iter = DListGetNext(iter);
		--num;
	}
	
	return iter;
}

static int IntsAreEqual(const void* num1, const void* num2)
{
	return (*(int*)num1 == *(int*)num2);
}

static int NumIsMultOfTheOther(const void* num1, const void* num2)
{
	if (0 == *(int*)num1 || 0 == *(int*)num2)
	{
		return 1;
	}
	if ((0 == (*(int*)num1 % *(int*)num2)) || (0 == (*(int*)num2 % *(int*)num1)))
	{
		return 1;
	}
	
	return 0;
}

static void FillListWithIntArr(dlist_t* dlist, const int arr[], size_t arr_length)
{
	dlist_iter_t iter = DListBegin(dlist);
	size_t index = 0;
	
	while (0 < arr_length)
	{
		iter = DListInsert(iter, (void*)&arr[index]); 
		iter = DListGetNext(iter);
		
		++arr;
		--arr_length;
	}
}

static int IsMatchIntArrayWithDList(dlist_t* dlist, const int arr[], size_t arr_length)
{
	dlist_iter_t iter = DListBegin(dlist);
	
	assert(DListCount(dlist) == arr_length);
	
	while (0 < arr_length)
	{
		if (*arr != *(int*)DListGetData(iter))
		{
			return 0;
		}
		
		iter = DListGetNext(iter);
		++arr;
		--arr_length;
	}
	
	return 1;
}

/* ==== TEST FUNCTIONS ==== */

static test_status_t DListCreate_SmokeTest(void)
{
	dlist_t* dlist = DListCreate();
	DListDestroy(dlist);

	return TEST_STATUS_SUCCESS;
}

static test_status_t DListInsert_SimpleTest(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	dlist_t* dlist = DListCreate();
	
	int a = 5;
	
	dlist_iter_t iter_start = DListBegin(dlist);
	dlist_iter_t iter_entered = DListInsert(iter_start, (void*)&a);
	
	if (a != *(int*)DListGetData(iter_entered))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	DListDestroy(dlist);

	return res;
}

static test_status_t DListRemove_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	dlist_t* dlist = DListCreate();
	dlist_iter_t iter = NULL;
	
	int a = 5;
	int b = 7;
	int c = 3;
	
	iter = DListBegin(dlist);
	iter = DListInsert(iter, (void*)&a);
	
	iter = DListGetNext(iter);
	iter = DListInsert(iter, (void*)&b);
	
	if (b != *(int*)DListGetData(iter))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	iter = DListRemove(iter);
	
	if (DListEnd(dlist) != iter)
	{
		res = TEST_STATUS_FAILURE;
	}
	
	DListInsert(iter, (void*)&c);
	iter = DListBegin(dlist);
	
	if (a != *(int*)DListGetData(iter))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	iter = DListGetNext(iter);
	
	if (c != *(int*)DListGetData(iter))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	iter = DListGetNext(iter);
	
	if (DListEnd(dlist) != iter)
	{
		res = TEST_STATUS_FAILURE;
	}
	
	DListDestroy(dlist);

	return res;
}

static test_status_t DListRemove_MakeEmpty_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	dlist_t* dlist = DListCreate();
	dlist_iter_t iter = NULL;
	
	int a = 5;
	int b = 7;
	int c = 3;
	
	iter = DListBegin(dlist);
	iter = DListInsert(iter, (void*)&a);
	
	iter = DListGetNext(iter);
	iter = DListInsert(iter, (void*)&b);
	
	if (b != *(int*)DListGetData(iter))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if(0 != DListIsEmpty(dlist))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	iter = DListRemove(iter);
	iter = DListBegin(dlist);
	iter = DListRemove(iter);
	
	if(1 != DListIsEmpty(dlist))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	iter = DListInsert(iter, (void*)&c);
	iter = DListBegin(dlist);
	
	if (c != *(int*)DListGetData(iter))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	iter = DListGetNext(iter);
	
	if (DListEnd(dlist) != iter)
	{
		res = TEST_STATUS_FAILURE;
	}
	
	DListDestroy(dlist);

	return res;
}

static test_status_t DListPushPopFront_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	dlist_t* dlist = DListCreate();
	dlist_iter_t iter = NULL;
	
	int a = 5;
	int b = 7;
	int c = 9;
	int c_ret = 0;
	
	DListPushFront(dlist, (void*)&a);
	DListPushFront(dlist, (void*)&b);
	DListPushFront(dlist, (void*)&c);
	iter = DListBegin(dlist);
	
	if (c != *(int*)DListGetData(iter))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	c_ret = *(int*)DListPopFront(dlist);
	
	if (c != c_ret)
	{
		res = TEST_STATUS_FAILURE;
	}
	
	iter = DListBegin(dlist);
	
	if (b != *(int*)DListGetData(iter))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	DListDestroy(dlist);

	return res;
}

static test_status_t DListPushPopBack_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	dlist_t* dlist = DListCreate();
	dlist_iter_t iter = NULL;
	
	int a = 5;
	int b = 7;
	int c = 9;
	int c_ret = 0;
	
	DListPushBack(dlist, (void*)&a);
	DListPushBack(dlist, (void*)&b);
	DListPushBack(dlist, (void*)&c);
	iter = DListGetNext(DListGetNext(DListBegin(dlist)));
	
	if (c != *(int*)DListGetData(iter))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	c_ret = *(int*)DListPopBack(dlist);
	
	if (c != c_ret)
	{
		res = TEST_STATUS_FAILURE;
	}
	
	iter = DListBegin(dlist);
	
	if (a != *(int*)DListGetData(iter))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	DListDestroy(dlist);

	return res;
}

static test_status_t DListFind_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	dlist_t* dlist = DListCreate();
	dlist_iter_t iter = NULL;
	dlist_iter_t from = NULL;
	dlist_iter_t to = NULL;
	
	int a = 5;
	int b = 7;
	int c = 9;
	int d = 12;
	int e = 22;
	
	DListPushBack(dlist, (void*)&a);
	DListPushBack(dlist, (void*)&b);
	DListPushBack(dlist, (void*)&c);
	DListPushBack(dlist, (void*)&d);
	DListPushBack(dlist, (void*)&e);
	
	from = DListBegin(dlist);
	to = DListEnd(dlist);
	
	iter = DListFind(from, to, (void*)&c, &IntsAreEqual);
	
	if (c != *(int*)DListGetData(iter))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	to = DListGetNext(DListGetNext(DListGetNext(from)));
	
	iter = DListFind(from, to, (void*)&e, &IntsAreEqual);
	
	if (d != *(int*)DListGetData(iter))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	DListDestroy(dlist);

	return res;
}

static test_status_t DListCount_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	dlist_t* dlist = DListCreate();
	
	int a = 5;
	int b = 7;
	int c = 9;
	
	if (0 != DListCount(dlist))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if(1 != DListIsEmpty(dlist))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	DListPushBack(dlist, (void*)&a);
	
	if (1 != DListCount(dlist))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if(0 != DListIsEmpty(dlist))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	DListPushBack(dlist, (void*)&b);
	DListPushBack(dlist, (void*)&c);
	
	if (3 != DListCount(dlist))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	DListPopBack(dlist);
	
	if (2 != DListCount(dlist))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	DListDestroy(dlist);

	return res;
}

static test_status_t DListSplice_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	dlist_t* dlist1 = DListCreate();
	dlist_t* dlist2 = DListCreate();
	dlist_iter_t from_1 = NULL;
	dlist_iter_t to_1 = NULL;
	dlist_iter_t where_2 = NULL;
	
	int arr1[] = {1,2,3,4,5,6,7,8,9};
	int arr2[] = {11,22,33,44,55,66,77};
	int ans1[] = {1,2,6,7,8,9};
	int ans2[] = {11,22,33,44,3,4,5,55,66,77};
	
	FillListWithIntArr(dlist1, arr1, 9);
	FillListWithIntArr(dlist2, arr2, 7);
	
	from_1 = AdvanceIterBy(DListBegin(dlist1), 2); /* at '3'*/
	to_1 = AdvanceIterBy(DListBegin(dlist1), 5); /* at '6'*/
	where_2 = AdvanceIterBy(DListBegin(dlist2), 4); /* at '55'*/
	
	DListSplice(from_1, to_1, where_2);
	
	if (6 != DListCount(dlist1))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (10 != DListCount(dlist2))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (1 != IsMatchIntArrayWithDList(dlist1, ans1, 6))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (1 != IsMatchIntArrayWithDList(dlist2, ans2, 10))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	DListDestroy(dlist1);
	DListDestroy(dlist2);

	return res;
}

static test_status_t DListSplice_WholeList_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	dlist_t* dlist1 = DListCreate();
	dlist_t* dlist2 = DListCreate();
	dlist_iter_t from_1 = NULL;
	dlist_iter_t to_1 = NULL;
	dlist_iter_t where_2 = NULL;
	
	int arr1[] = {1,2,3,4,5,6,7,8,9};
	int arr2[] = {11,22,33,44,55,66,77};
	int ans2[] = {11,22,33,44,1,2,3,4,5,6,7,8,9,55,66,77};
	
	FillListWithIntArr(dlist1, arr1, 9);
	FillListWithIntArr(dlist2, arr2, 7);
	
	from_1 = DListBegin(dlist1);
	to_1 = DListEnd(dlist1);
	where_2 = AdvanceIterBy(DListBegin(dlist2), 4); /* at '55'*/
	
	DListSplice(from_1, to_1, where_2);
	
	if (0 != DListCount(dlist1))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (16 != DListCount(dlist2))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (1 != IsMatchIntArrayWithDList(dlist2, ans2, 16))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	DListDestroy(dlist1);
	DListDestroy(dlist2);

	return res;
}

static test_status_t DListSplice_IntoStart_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	dlist_t* dlist1 = DListCreate();
	dlist_t* dlist2 = DListCreate();
	dlist_iter_t from_1 = NULL;
	dlist_iter_t to_1 = NULL;
	dlist_iter_t where_2 = NULL;
	
	int arr1[] = {1,2,3,4,5,6,7,8,9};
	int arr2[] = {11,22,33,44,55,66,77};
	int ans1[] = {1,2,6,7,8,9};
	int ans2[] = {3,4,5,11,22,33,44,55,66,77};
	
	FillListWithIntArr(dlist1, arr1, 9);
	FillListWithIntArr(dlist2, arr2, 7);
	
	from_1 = AdvanceIterBy(DListBegin(dlist1), 2); /* at '3'*/
	to_1 = AdvanceIterBy(DListBegin(dlist1), 5); /* at '6'*/
	where_2 = DListBegin(dlist2);
	
	DListSplice(from_1, to_1, where_2);
	
	if (6 != DListCount(dlist1))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (10 != DListCount(dlist2))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (1 != IsMatchIntArrayWithDList(dlist1, ans1, 6))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (1 != IsMatchIntArrayWithDList(dlist2, ans2, 10))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	DListDestroy(dlist1);
	DListDestroy(dlist2);

	return res;
}

static test_status_t DListSplice_IntoEnd_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	dlist_t* dlist1 = DListCreate();
	dlist_t* dlist2 = DListCreate();
	dlist_iter_t from_1 = NULL;
	dlist_iter_t to_1 = NULL;
	dlist_iter_t where_2 = NULL;
	
	int arr1[] = {1,2,3,4,5,6,7,8,9};
	int arr2[] = {11,22,33,44,55,66,77};
	int ans1[] = {1,2,6,7,8,9};
	int ans2[] = {11,22,33,44,55,66,77,3,4,5};
	
	FillListWithIntArr(dlist1, arr1, 9);
	FillListWithIntArr(dlist2, arr2, 7);
	
	from_1 = AdvanceIterBy(DListBegin(dlist1), 2); /* at '3'*/
	to_1 = AdvanceIterBy(DListBegin(dlist1), 5); /* at '6'*/
	where_2 = DListEnd(dlist2);
	
	DListSplice(from_1, to_1, where_2);
	
	if (6 != DListCount(dlist1))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (10 != DListCount(dlist2))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (1 != IsMatchIntArrayWithDList(dlist1, ans1, 6))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (1 != IsMatchIntArrayWithDList(dlist2, ans2, 10))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	DListDestroy(dlist1);
	DListDestroy(dlist2);

	return res;
}

static test_status_t DListSplice_JustTwo_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	dlist_t* dlist1 = DListCreate();
	dlist_t* dlist2 = DListCreate();
	dlist_iter_t from_1 = NULL;
	dlist_iter_t to_1 = NULL;
	dlist_iter_t where_2 = NULL;
	
	int arr1[] = {1,2,3,4,5,6,7,8,9};
	int arr2[] = {11,22,33,44,55,66,77};
	int ans1[] = {1,2,3,6,7,8,9};
	int ans2[] = {11,22,33,44,4,5,55,66,77};
	
	FillListWithIntArr(dlist1, arr1, 9);
	FillListWithIntArr(dlist2, arr2, 7);
	
	from_1 = AdvanceIterBy(DListBegin(dlist1), 3); /* at '4'*/
	to_1 = AdvanceIterBy(DListBegin(dlist1), 5); /* at '6'*/
	where_2 = AdvanceIterBy(DListBegin(dlist2), 4); /* at '55'*/
	
	DListSplice(from_1, to_1, where_2);
	
	if (7 != DListCount(dlist1))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (9 != DListCount(dlist2))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (1 != IsMatchIntArrayWithDList(dlist1, ans1, 7))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (1 != IsMatchIntArrayWithDList(dlist2, ans2, 9))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	DListDestroy(dlist1);
	DListDestroy(dlist2);

	return res;
}

static test_status_t DListSplice_JustOne_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	dlist_t* dlist1 = DListCreate();
	dlist_t* dlist2 = DListCreate();
	dlist_iter_t from_1 = NULL;
	dlist_iter_t to_1 = NULL;
	dlist_iter_t where_2 = NULL;
	
	int arr1[] = {1,2,3,4,5,6,7,8,9};
	int arr2[] = {11,22,33,44,55,66,77};
	int ans1[] = {1,2,3,4,6,7,8,9};
	int ans2[] = {11,22,33,44,5,55,66,77};
	
	FillListWithIntArr(dlist1, arr1, 9);
	FillListWithIntArr(dlist2, arr2, 7);
	
	from_1 = AdvanceIterBy(DListBegin(dlist1), 4); /* at '5'*/
	to_1 = AdvanceIterBy(DListBegin(dlist1), 5); /* at '6'*/
	where_2 = AdvanceIterBy(DListBegin(dlist2), 4); /* at '55'*/
	
	DListSplice(from_1, to_1, where_2);
	
	if (8 != DListCount(dlist1))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (8 != DListCount(dlist2))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (1 != IsMatchIntArrayWithDList(dlist1, ans1, 8))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (1 != IsMatchIntArrayWithDList(dlist2, ans2, 8))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	DListDestroy(dlist1);
	DListDestroy(dlist2);

	return res;
}

static test_status_t DListSplice_FromAndToSame_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	dlist_t* dlist1 = DListCreate();
	dlist_t* dlist2 = DListCreate();
	dlist_iter_t from_1 = NULL;
	dlist_iter_t to_1 = NULL;
	dlist_iter_t where_2 = NULL;
	
	int arr1[] = {1,2,3,4,5,6,7,8,9};
	int arr2[] = {11,22,33,44,55,66,77};
	int ans1[] = {1,2,3,4,5,6,7,8,9};
	int ans2[] = {11,22,33,44,55,66,77};
	
	FillListWithIntArr(dlist1, arr1, 9);
	FillListWithIntArr(dlist2, arr2, 7);
	
	from_1 = AdvanceIterBy(DListBegin(dlist1), 5); /* at '6'*/
	to_1 = AdvanceIterBy(DListBegin(dlist1), 5); /* at '6'*/
	where_2 = AdvanceIterBy(DListBegin(dlist2), 4); /* at '55'*/
	
	DListSplice(from_1, to_1, where_2);
	
	if (9 != DListCount(dlist1))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (7 != DListCount(dlist2))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (1 != IsMatchIntArrayWithDList(dlist1, ans1, 9))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (1 != IsMatchIntArrayWithDList(dlist2, ans2, 7))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	DListDestroy(dlist1);
	DListDestroy(dlist2);

	return res;
}

static test_status_t DListMultiFind_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	dlist_t* dlist = DListCreate();
	dlist_t* dlist_out = DListCreate();
	dlist_iter_t from = NULL;
	dlist_iter_t to = NULL;
	int divisible_by = 3;
	
	int arr[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	int out_ans[] = {0,1,3,6,9,12,15};
	
	FillListWithIntArr(dlist, arr, 16);
	
	from = DListBegin(dlist);
	to = DListEnd(dlist);
	
	DListMultiFind(	from, to,
					(void*)&divisible_by, &NumIsMultOfTheOther, dlist_out);
	
	
	if (7 != DListCount(dlist_out))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (1 != IsMatchIntArrayWithDList(dlist_out, out_ans, 7))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	DListDestroy(dlist);
	DListDestroy(dlist_out);

	return res;
}

