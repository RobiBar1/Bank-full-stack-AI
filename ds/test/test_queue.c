#include <stdio.h>
#include <assert.h>

#include "queue.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

typedef enum {TEST_STATUS_SUCCESS, TEST_STATUS_FAILURE} test_status_t;

static void CheckTest(test_status_t result, char* name);
static test_status_t QueueCreate_UnitTest(void);
static test_status_t QueueEnqueue_UnitTest(void);
static test_status_t QueueEnqueueMultiple_Test(void);
static test_status_t QueuePeek_UnitTest(void);
static test_status_t QueueDequeue_UnitTest(void);
static test_status_t QueueDequeueMultiple_Test(void);
static test_status_t QueueIsEmpty_Test(void);
static test_status_t QueueSize_Test(void);
static test_status_t QueueAppend_Test(void);
static test_status_t QueueAppendWithEmpty_Test(void);

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

	CheckTest(QueueCreate_UnitTest(), "QueueCreate_UnitTest");
	CheckTest(QueueEnqueue_UnitTest(), "QueueEnqueue_UnitTest");
	CheckTest(QueueEnqueueMultiple_Test(), "QueueEnqueueMultiple_Test");
	CheckTest(QueuePeek_UnitTest(), "QueuePeek_UnitTest");
	CheckTest(QueueDequeue_UnitTest(), "QueueDequeue_UnitTest");
	CheckTest(QueueDequeueMultiple_Test(), "QueueDequeueMultiple_Test");
	CheckTest(QueueIsEmpty_Test(), "QueueIsEmpty_Test");
	CheckTest(QueueSize_Test(), "QueueSize_Test");
	CheckTest(QueueAppend_Test(), "QueueAppend_Test");
	CheckTest(QueueAppendWithEmpty_Test(), "QueueAppendWithEmpty_Test");
	
	return 0;
}

static test_status_t QueueCreate_UnitTest(void)
{
	queue_t* queue = QueueCreate();
	QueueDestroy(queue);
	return TEST_STATUS_SUCCESS;
}

static test_status_t QueueEnqueue_UnitTest(void)
{
	queue_t* queue = QueueCreate();
	int a = 5;
	QueueEnqueue(queue, (void*)&a);
	QueueDestroy(queue);
	return TEST_STATUS_SUCCESS;
}

static test_status_t QueueEnqueueMultiple_Test(void)
{
	queue_t* queue = QueueCreate();
	test_status_t res = TEST_STATUS_SUCCESS;
	int a = 5, b = 7, c = 9;
	
	QueueEnqueue(queue, (void*)&a);
	QueueEnqueue(queue, (void*)&b);
	QueueEnqueue(queue, (void*)&c);
	
	if (a != *(int*)QueuePeek(queue)) { res = TEST_STATUS_FAILURE; }
	QueueDequeue(queue);
	
	if (b != *(int*)QueuePeek(queue)) { res = TEST_STATUS_FAILURE; }
	QueueDequeue(queue);
	
	if (c != *(int*)QueuePeek(queue)) { res = TEST_STATUS_FAILURE; }
	
	QueueDestroy(queue);
	return res;
}

static test_status_t QueuePeek_UnitTest(void)
{
	queue_t* queue = QueueCreate();
	int a = 5;
	QueueEnqueue(queue, (void*)&a);
	
	if (a != *(int*)QueuePeek(queue))
	{
		QueueDestroy(queue);
		return TEST_STATUS_FAILURE;
	}
	
	QueueDestroy(queue);
	return TEST_STATUS_SUCCESS;
}

static test_status_t QueueDequeue_UnitTest(void)
{
	queue_t* queue = QueueCreate();
	int a = 5;
	QueueEnqueue(queue, (void*)&a);
	QueueDequeue(queue);
	QueueDestroy(queue);
	return TEST_STATUS_SUCCESS;
}

static test_status_t QueueDequeueMultiple_Test(void)
{
	queue_t* queue = QueueCreate();
	test_status_t res = TEST_STATUS_SUCCESS;
	int a = 5, b = 7, c = 9, d = 12;
	
	QueueEnqueue(queue, (void*)&a);
	QueueEnqueue(queue, (void*)&b);
	QueueEnqueue(queue, (void*)&c);
	QueueEnqueue(queue, (void*)&d);
	
	QueueDequeue(queue);
	
	if (b != *(int*)QueuePeek(queue)) { res = TEST_STATUS_FAILURE; }
	QueueDequeue(queue);
	
	if (c != *(int*)QueuePeek(queue)) { res = TEST_STATUS_FAILURE; }
	QueueDequeue(queue);
	
	if (d != *(int*)QueuePeek(queue)) { res = TEST_STATUS_FAILURE; }
	
	QueueDestroy(queue);
	return res;
}

static test_status_t QueueIsEmpty_Test(void)
{
	queue_t* queue = QueueCreate();
	test_status_t res = TEST_STATUS_SUCCESS;
	int a = 5, b = 7;
	
	if (1 != QueueIsEmpty(queue)) { res = TEST_STATUS_FAILURE; }
	
	QueueEnqueue(queue, (void*)&a);
	if (0 != QueueIsEmpty(queue)) { res = TEST_STATUS_FAILURE; }
	
	QueueEnqueue(queue, (void*)&b);
	QueueDequeue(queue);
	if (0 != QueueIsEmpty(queue)) { res = TEST_STATUS_FAILURE; }
	
	QueueDequeue(queue);
	if (1 != QueueIsEmpty(queue)) { res = TEST_STATUS_FAILURE; }
	
	QueueDestroy(queue);
	return res;
}

static void FillQueueWithIntArr(queue_t* queue, const int arr[], size_t arr_length)
{
	size_t index = 0;
	while (index < arr_length)
	{
		QueueEnqueue(queue, (void*)&arr[index]); 
		++index;
	}
}

static int IsMatchIntArrayWithQueue(queue_t* queue, const int arr[], size_t arr_length)
{
	if (QueueSize(queue) != arr_length) { return 0; }
	
	while (arr_length > 0)
	{
		if (*arr != *(int*)QueuePeek(queue)) { return 0; }
		QueueDequeue(queue);
		++arr;
		--arr_length;
	}
	return 1;
}

static test_status_t QueueSize_Test(void)
{
	queue_t* queue = QueueCreate();
	test_status_t res = TEST_STATUS_SUCCESS;
	int arr[] = {2, 4, 14, 8, 5};
	size_t arr_length = 5;
	
	if (0 != QueueSize(queue)) { res = TEST_STATUS_FAILURE; }
	
	FillQueueWithIntArr(queue, arr, arr_length);
	if (arr_length != QueueSize(queue)) { res = TEST_STATUS_FAILURE; }
	
	QueueDequeue(queue);
	if ((arr_length - 1) != QueueSize(queue)) { res = TEST_STATUS_FAILURE; }
	
	QueueDestroy(queue);
	return res;
}

static test_status_t QueueAppend_Test(void)
{
	queue_t* dest = QueueCreate();
	queue_t* src = QueueCreate();
	test_status_t res = TEST_STATUS_SUCCESS;
	int arr1[] = {1, 2, 3}, arr2[] = {4, 5, 6}, arr_appended[] = {1, 2, 3, 4, 5, 6};
	int a = 7, b = 12;
	
	FillQueueWithIntArr(dest, arr1, 3);
	FillQueueWithIntArr(src, arr2, 3);
	
	QueueAppend(dest, src);
	
	if (1 != QueueIsEmpty(src)) { res = TEST_STATUS_FAILURE; }
	if (6 != QueueSize(dest)) { res = TEST_STATUS_FAILURE; }
	if (1 != IsMatchIntArrayWithQueue(dest, arr_appended, 6)) { res = TEST_STATUS_FAILURE; }
	
	QueueEnqueue(dest, (void*)&a); 
	QueueEnqueue(src, (void*)&b); 
	
	if (1 != QueueSize(dest)) { res = TEST_STATUS_FAILURE; }
	if (1 != QueueSize(src)) { res = TEST_STATUS_FAILURE; }
	
	QueueDestroy(dest);
	QueueDestroy(src);
	return res;
}

static test_status_t QueueAppendWithEmpty_Test(void)
{
	queue_t* dest = QueueCreate();
	queue_t* src = QueueCreate();
	test_status_t res = TEST_STATUS_SUCCESS;
	int arr1[] = {1, 2, 3};
	int a = 4, b = 12;
	
	FillQueueWithIntArr(dest, arr1, 3);
	QueueAppend(dest, src);
	
	if (1 != QueueIsEmpty(src)) { res = TEST_STATUS_FAILURE; }
	if (3 != QueueSize(dest)) { res = TEST_STATUS_FAILURE; }
	if (1 != IsMatchIntArrayWithQueue(dest, arr1, 3)) { res = TEST_STATUS_FAILURE; }
	
	QueueEnqueue(dest, (void*)&a); 
	QueueEnqueue(src, (void*)&b); 
	
	if (1 != QueueSize(dest)) { res = TEST_STATUS_FAILURE; }
	if (1 != QueueSize(src)) { res = TEST_STATUS_FAILURE; }
	
	QueueDestroy(dest);
	QueueDestroy(src);
	return res;
}

