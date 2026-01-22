/*gd -I include src/scheduler.c src/task.c src/pqueue.c src/uid.c src/sorted_list.c src/d_link_list.c 
*/

#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>

#include "task.h"
#include "scheduler.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

/* gd ../c/pointers2/String.c src/uid.c src/scheduler.c src/task.c src/pqueue.c -I include -I ../c/pointers2 src/sorted_list.c src/doubly_linked_list.c test/test_scheduler.c */

typedef enum {TEST_STATUS_SUCCESS, TEST_STATUS_FAILURE} test_status_t;

static void CheckTest(test_status_t result, char* name);
static test_status_t SchedulerCreate_UnitTest(void);
static test_status_t TaskCreate_UnitTest(void);
static test_status_t SchedulerAddTask_UnitTest(void);
static test_status_t SchedulerAddTaskMultiple_Test(void);
static test_status_t SchedulerRun_BasicTest(void);
static test_status_t SchedulerRun_ExtensiveTest(void);

/*
static test_status_t QueueEnqueueMultiple_Test(void);
static test_status_t QueuePeek_UnitTest(void);
static test_status_t QueueDequeue_UnitTest(void);
static test_status_t QueueDequeueMultiple_Test(void);
static test_status_t QueueIsEmpty_Test(void);
static test_status_t QueueSize_Test(void);
static test_status_t QueueAppend_Test(void);
static test_status_t QueueAppendWithEmpty_Test(void);
*/

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

	CheckTest(SchedulerCreate_UnitTest(), "SchedulerCreate_UnitTest");
	CheckTest(TaskCreate_UnitTest(), "TaskCreate_UnitTest");
	CheckTest(SchedulerAddTask_UnitTest(), "SchedulerAddTask_UnitTest");
	CheckTest(SchedulerAddTaskMultiple_Test(), "SchedulerAddTaskMultiple_Test");
	CheckTest(SchedulerRun_BasicTest(), "SchedulerRun_BasicTest");
	CheckTest(SchedulerRun_ExtensiveTest(), "SchedulerRun_ExtensiveTest");
	/*
	CheckTest(QueueEnqueue_UnitTest(), "QueueEnqueue_UnitTest");
	CheckTest(QueueEnqueueMultiple_Test(), "QueueEnqueueMultiple_Test");
	CheckTest(QueuePeek_UnitTest(), "QueuePeek_UnitTest");
	CheckTest(QueueDequeue_UnitTest(), "QueueDequeue_UnitTest");
	CheckTest(QueueDequeueMultiple_Test(), "QueueDequeueMultiple_Test");
	CheckTest(QueueIsEmpty_Test(), "QueueIsEmpty_Test");
	CheckTest(QueueSize_Test(), "QueueSize_Test");
	CheckTest(QueueAppend_Test(), "QueueAppend_Test");
	CheckTest(QueueAppendWithEmpty_Test(), "QueueAppendWithEmpty_Test");
	*/
	
	return 0;
}

task_status DoNothing(void* param)
{
    (void) param;
    
    return DO_NOT_REPEAT;
}

void Cleanup(void* param)
{
    (void)param;
}

static test_status_t TaskCreate_UnitTest(void)
{
	task_t* task = NULL;
    time_t interval = 10;
        
	task = TaskCreate(DoNothing, Cleanup, interval, NULL);
	
	if (NULL == task)
	{
	    return TEST_STATUS_FAILURE;
	}
	
	TaskDestroy(task);
	return TEST_STATUS_SUCCESS;
}

static test_status_t SchedulerCreate_UnitTest(void)
{
	scheduler_t* scheduler = SchedulerCreate();
	
	if (NULL == scheduler)
	{
	    return TEST_STATUS_FAILURE;
	}
	
	SchedulerDestroy(scheduler);
	return TEST_STATUS_SUCCESS;
}

static test_status_t SchedulerAddTask_UnitTest(void)
{
        scheduler_t* scheduler = SchedulerCreate();
        time_t interval = 10;
        
	SchedulerAddTask(scheduler, interval, DoNothing, Cleanup, NULL);
	
	if (SchedulerIsEmpty(scheduler))
	{
	    return TEST_STATUS_FAILURE;
	}
	
	SchedulerDestroy(scheduler);
	return TEST_STATUS_SUCCESS;
}

static test_status_t SchedulerAddTaskMultiple_Test(void)
{
	scheduler_t* scheduler = SchedulerCreate();
	test_status_t res = TEST_STATUS_SUCCESS;
	time_t a = 5, b = 7, c = 9;
	ilrd_uid_t a_uid = {0}, b_uid = {0}, c_uid = {0};
	
	a_uid = SchedulerAddTask(scheduler, a, DoNothing, Cleanup, NULL);
	b_uid = SchedulerAddTask(scheduler, b, DoNothing, Cleanup, NULL);
	c_uid = SchedulerAddTask(scheduler, c, DoNothing, Cleanup, NULL);
	
	if (IsILRDUIDEqual(&a_uid, &bad_uid) || IsILRDUIDEqual(&b_uid, &bad_uid) ||
	    IsILRDUIDEqual(&c_uid, &bad_uid))
	{
	    res = TEST_STATUS_FAILURE;
	}
	
	if (3 != SchedulerCount(scheduler)) { res = TEST_STATUS_FAILURE; }
	SchedulerRemoveTask(scheduler, a_uid);
	
	if (2 != SchedulerCount(scheduler)) { res = TEST_STATUS_FAILURE; }
	SchedulerRemoveTask(scheduler, b_uid);
	
	if (1 != SchedulerCount(scheduler)) { res = TEST_STATUS_FAILURE; }
	
	SchedulerDestroy(scheduler);
	return res;
}

static test_status_t SchedulerRun_BasicTest(void)
{
	scheduler_t* scheduler = SchedulerCreate();
	test_status_t res = TEST_STATUS_SUCCESS;
	time_t a = 1, b = 2, c = 2;
	ilrd_uid_t a_uid = {0}, b_uid = {0}, c_uid = {0};
	
	a_uid = SchedulerAddTask(scheduler, a, DoNothing, Cleanup, NULL);
	b_uid = SchedulerAddTask(scheduler, b, DoNothing, Cleanup, NULL);
	c_uid = SchedulerAddTask(scheduler, c, DoNothing, Cleanup, NULL);
	
	if (IsILRDUIDEqual(&a_uid, &bad_uid) || IsILRDUIDEqual(&b_uid, &bad_uid) ||
	    IsILRDUIDEqual(&c_uid, &bad_uid))
	{
	    res = TEST_STATUS_FAILURE;
	}
	
	if (3 != SchedulerCount(scheduler)) { res = TEST_STATUS_FAILURE; }
	SchedulerRemoveTask(scheduler, a_uid);
	
	if (2 != SchedulerCount(scheduler)) { res = TEST_STATUS_FAILURE; }
	SchedulerRemoveTask(scheduler, b_uid);
	
	if (1 != SchedulerCount(scheduler)) { res = TEST_STATUS_FAILURE; }
	
	SchedulerRun(scheduler);
	
	if (0 != SchedulerCount(scheduler)) { res = TEST_STATUS_FAILURE; }
	
	SchedulerDestroy(scheduler);
	return res;
}

task_status DoMalloc(void* param)
{
        int* my_param = (int*) param;
        
        if (NULL == my_param)
        {
            return FAILURE;
        }
        
        (*my_param)++;
        
        return DO_NOT_REPEAT;
}

void Cleanup_free(void* param)
{
        free(param);
}

static test_status_t SchedulerRun_ExtensiveTest(void)
{
        scheduler_t* scheduler = SchedulerCreate();
	test_status_t res = TEST_STATUS_SUCCESS;
	time_t a = 1;
	int* arr = (int*)malloc(sizeof(int)*2); 
	ilrd_uid_t a_uid = {0};
	
	a_uid = SchedulerAddTask(scheduler, a, DoMalloc, Cleanup_free, arr);
	
	if (IsILRDUIDEqual(&a_uid, &bad_uid))
	{
	    res = TEST_STATUS_FAILURE;
	}
	
	SchedulerRun(scheduler);
	
	if (0 != SchedulerCount(scheduler)) { res = TEST_STATUS_FAILURE; }
	
	SchedulerDestroy(scheduler);
	return res;
}

/*

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
*/


