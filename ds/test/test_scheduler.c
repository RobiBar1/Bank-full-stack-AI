#include <stdio.h> /* printf */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */

#include "uid.h"
#include "scheduler.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

#define APPLY_TEST(func_call) CheckTest(func_call, #func_call)
#define UNUSED(x) ((void)(x))

typedef enum {TEST_STATUS_SUCCESS, TEST_STATUS_FAILURE, TEST_INNER_FAILURE} test_status_t;

/* gd test_scheduler.c ../src/scheduler.c ../src/pqueue.c  ../src/sorted_list.c ../src/dlist.c ../src/task.c ../src/uid.c -I ../include/ */

/* ==== FORWARD DELARATIONS ==== */

static void CheckTest(test_status_t result, char* name);

static test_status_t SchedulerCreate_SmokeTest(void);
static test_status_t SchedulerAddTask_SmokeTest(void);
static test_status_t SchedulerIsEmptyCount_BasicTest(void);
static test_status_t SchedulerRun_BasicTest(void);
static test_status_t SchedulerRun_CheckRepeatBasicTest(void);
static test_status_t SchedulerRun_CheckCleanupBasicTest(void);
static test_status_t SchedulerRun_CheckFailureBasicTest(void);
static test_status_t SchedulerRemoveTask_CheckRemoveSelfTest(void);
static test_status_t SchedulerRemoveTask_CheckRemoveOtherTest(void);

/* ==== TEST FUNCTION ==== */

static void CheckTest(test_status_t result, char* name)
{
	if (result == TEST_STATUS_SUCCESS)
	{
		printf("-> %s: ", name);
		printf(GREEN "PASSED\n\n" RESET);
	}
	else if (result == TEST_INNER_FAILURE)
	{
		printf("-> %s: ", name);
		printf(GREEN "ALLOC FAILURE\n\n" RESET);
	}
	else 
	{
		printf(RED "-> %s: FAILED\n\n" RESET, name);
	}
}


int main()
{

	printf("\n ---- TESTS ----\n\n");

	APPLY_TEST(SchedulerCreate_SmokeTest());
	APPLY_TEST(SchedulerAddTask_SmokeTest());
	APPLY_TEST(SchedulerIsEmptyCount_BasicTest());
	APPLY_TEST(SchedulerRun_BasicTest());
	APPLY_TEST(SchedulerRun_CheckRepeatBasicTest());
	APPLY_TEST(SchedulerRun_CheckCleanupBasicTest());
	APPLY_TEST(SchedulerRun_CheckFailureBasicTest());
	APPLY_TEST(SchedulerRemoveTask_CheckRemoveSelfTest());
	APPLY_TEST(SchedulerRemoveTask_CheckRemoveOtherTest());
	
	return 0;
}

/* ==== HELPER FUNCTIONS ==== */

static size_t mars_count = 0;
static size_t jupiter_count = 0;
static size_t venus_count = 0;
static size_t neptune_count = 0;
static size_t mercury_count = 0;

task_status CallSchedulerStop(void* scheduler)
{
	SchedulerStop((scheduler_t*)scheduler);
	
	return DO_NOT_REPEAT;
}

void NothingFunction(void* param)
{
	UNUSED(param);
}

task_status PrintMars(void* param)
{
	UNUSED(param);

	printf("mars %lu\n", mars_count);
	
	++mars_count;
	
	return DO_NOT_REPEAT;
}

void PrintMars_Cleanup(void* param)
{
	UNUSED(param);

	printf("mars cleanup\n");
}

task_status PrintJupiter(void* param)
{
	UNUSED(param);

	printf("jupiter %lu\n", jupiter_count);
	
	++jupiter_count;
	
	if (jupiter_count > 2)
	{
		return DO_NOT_REPEAT;
	}
	
	return REPEAT;	
}

void PrintJupiter_Cleanup(void* param)
{
	UNUSED(param);

	printf("jupiter cleanup\n");
}

task_status PrintVenus(void* param)
{
	UNUSED(param);

	printf("venus %lu\n", venus_count);
	
	if (0 == venus_count)
	{
		*(int**)param = (int*)malloc(sizeof(int)*10);
	}
	
	++venus_count;
	
	if (venus_count > 2)
	{
		return DO_NOT_REPEAT;
	}
	
	return REPEAT;	
}

void PrintVenus_Cleanup(void* param)
{
	free(*(int**)param);

	printf("venus cleanup\n");
}

typedef struct neptune_struct
{
	int* arr;
	scheduler_t* scheduler;
} neptune_struct_t;

task_status PrintNeptune(void* param)
{
	UNUSED(param);

	printf("neptune %lu\n", neptune_count);
	
	if (0 == neptune_count)
	{
		((neptune_struct_t*)param)->arr = (int*)malloc(sizeof(int)*10);
	}
	
	if (1 == neptune_count)
	{
		SchedulerStop(((neptune_struct_t*)param)->scheduler);
		
		neptune_count = 20;
		return FAILURE;
	}
	
	++neptune_count;
	
	if (neptune_count > 5)
	{
		return DO_NOT_REPEAT;
	}
	
	return REPEAT;	
}

void PrintNeptune_Cleanup(void* param)
{
	free(((neptune_struct_t*)param)->arr);
	
	printf("neptune cleanup\n");
}

typedef struct mercury_struct
{
	int* arr;
	scheduler_t* scheduler;
	ilrd_uid_t uid;
} mercury_struct_t;

task_status PrintMercury(void* param)
{
	UNUSED(param);

	printf("mercury %lu\n", mercury_count);
	
	if (0 == mercury_count)
	{
		((mercury_struct_t*)param)->arr = (int*)malloc(sizeof(int)*10);
	}
	
	if (2 == mercury_count)
	{
		SchedulerRemoveTask(((mercury_struct_t*)param)->scheduler,
							((mercury_struct_t*)param)->uid);
							
		++mercury_count;					
		return REPEAT;
	}
	
	++mercury_count;
	
	if (mercury_count > 3)
	{
		return DO_NOT_REPEAT;
	}
	
	return REPEAT;
}

void PrintMercury_Cleanup(void* param)
{
	free(((mercury_struct_t*)param)->arr);
	
	printf("mercury cleanup\n");
}

/* ==== TEST FUNCTIONS ==== */

static test_status_t SchedulerCreate_SmokeTest(void)
{
	scheduler_t* scheduler = SchedulerCreate();
	SchedulerDestroy(scheduler);

	return TEST_STATUS_SUCCESS;
}

static test_status_t SchedulerAddTask_SmokeTest(void)
{
	scheduler_t* scheduler = SchedulerCreate();
	
	ilrd_uid_t uid_returned = SchedulerAddTask(scheduler, 1, &PrintMars, &PrintMars_Cleanup, NULL);
	
	if (IsILRDUIDEqual(&bad_uid, &uid_returned))
	{
		printf(RED "[Recieved a bad uid]\n" RESET);
		SchedulerDestroy(scheduler);
		return TEST_INNER_FAILURE;
	}
	
	SchedulerDestroy(scheduler);
	return TEST_STATUS_SUCCESS;
}

static test_status_t SchedulerIsEmptyCount_BasicTest(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	scheduler_t* scheduler = SchedulerCreate();
	
	ilrd_uid_t uid_returned1 = bad_uid;
	ilrd_uid_t uid_returned2 = bad_uid;
	
	if (1 != SchedulerIsEmpty(scheduler))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (0 != SchedulerCount(scheduler))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	uid_returned1 = SchedulerAddTask(scheduler, 1, &PrintMars, &PrintMars_Cleanup, NULL);
	
	if (IsILRDUIDEqual(&bad_uid, &uid_returned1))
	{
		printf(RED "[Recieved a bad uid1]\n" RESET);
		SchedulerDestroy(scheduler);
		return TEST_INNER_FAILURE;
	}
	
	if (0 != SchedulerIsEmpty(scheduler))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (1 != SchedulerCount(scheduler))
	{
		res = TEST_STATUS_FAILURE;
	}

	uid_returned2 = SchedulerAddTask(scheduler, 2, &PrintMars, &PrintMars_Cleanup, NULL);
	
	if (IsILRDUIDEqual(&bad_uid, &uid_returned2))
	{
		printf(RED "[Recieved a bad uid2]\n" RESET);
		SchedulerDestroy(scheduler);
		return TEST_INNER_FAILURE;
	}
	
	if (0 != SchedulerIsEmpty(scheduler))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (2 != SchedulerCount(scheduler))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	SchedulerDestroy(scheduler);
	return res;
}

static test_status_t SchedulerRun_BasicTest(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	scheduler_t* scheduler = SchedulerCreate();
	
	SchedulerAddTask(scheduler, 1, &PrintMars, &PrintMars_Cleanup, NULL);
	SchedulerAddTask(scheduler, 2, &PrintMars, &PrintMars_Cleanup, NULL);
	
	SchedulerRun(scheduler);
	
	if (2 != mars_count)
	{
		res = TEST_STATUS_FAILURE;
	}	
	
	mars_count = 0;
	
	SchedulerDestroy(scheduler);
	return res;
}

static test_status_t SchedulerRun_CheckRepeatBasicTest(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	scheduler_t* scheduler = SchedulerCreate();
	
	
	SchedulerAddTask(scheduler, 2, &PrintJupiter, &PrintJupiter_Cleanup, NULL);
	SchedulerAddTask(scheduler, 3, &PrintMars, &PrintMars_Cleanup, NULL);
	
	SchedulerRun(scheduler);
	
	if (1 != mars_count)
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (3 != jupiter_count)
	{
		res = TEST_STATUS_FAILURE;
	}
	
	mars_count = 0;
	jupiter_count = 0;
	
	SchedulerDestroy(scheduler);
	return res;
}

static test_status_t SchedulerRun_CheckCleanupBasicTest(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	scheduler_t* scheduler = SchedulerCreate();
	int* venus_malloc = NULL;
	
	SchedulerAddTask(scheduler, 2, &PrintVenus, &PrintVenus_Cleanup, &venus_malloc);
	SchedulerAddTask(scheduler, 3, &PrintMars, &PrintMars_Cleanup, NULL);
	
	SchedulerRun(scheduler);
	
	if (1 != mars_count)
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (3 != venus_count)
	{
		res = TEST_STATUS_FAILURE;
	}
	
	mars_count = 0;
	venus_count = 0;
	
	SchedulerDestroy(scheduler);
	return res;
}

static test_status_t SchedulerRun_CheckFailureBasicTest(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	scheduler_t* scheduler = SchedulerCreate();
	neptune_struct_t* neptune = {0};
    
    neptune = (neptune_struct_t*)malloc(sizeof(neptune_struct_t));
    
    neptune->scheduler = scheduler;
	
	SchedulerAddTask(scheduler, 2, &PrintNeptune, &PrintNeptune_Cleanup, neptune);
	SchedulerAddTask(scheduler, 3, &PrintMars, &PrintMars_Cleanup, NULL);
	
	SchedulerRun(scheduler);
	
	if (1 != mars_count)
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (20 != neptune_count)
	{
		res = TEST_STATUS_FAILURE;
	}

	mars_count = 0;
	neptune_count = 0;
	
	SchedulerDestroy(scheduler);
	free(neptune);
	return res;
}

static test_status_t SchedulerRemoveTask_CheckRemoveSelfTest(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	scheduler_t* scheduler = SchedulerCreate();
	mercury_struct_t* mercury = {0};
	ilrd_uid_t mercury_uid = {0};
    
    mercury = (mercury_struct_t*)malloc(sizeof(mercury_struct_t));
    
    mercury->scheduler = scheduler;
	
	mercury_uid = SchedulerAddTask(scheduler, 1, &PrintMercury, &PrintMercury_Cleanup, mercury);
	SchedulerAddTask(scheduler, 6, &PrintMars, &PrintMars_Cleanup, NULL);
	
	mercury->uid = mercury_uid;
	
	SchedulerRun(scheduler);
	
	if (1 != mars_count)
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (3 != mercury_count)
	{
		res = TEST_STATUS_FAILURE;
	}

	mars_count = 0;
	mercury_count = 0;
	
	
	SchedulerDestroy(scheduler);
	free(mercury);
	return res;
}

static test_status_t SchedulerRemoveTask_CheckRemoveOtherTest(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	scheduler_t* scheduler = SchedulerCreate();
	mercury_struct_t* mercury = {0};
	int* venus_malloc = NULL;
	ilrd_uid_t venus_uid = {0};
    
    mercury = (mercury_struct_t*)malloc(sizeof(mercury_struct_t));
    
    mercury->scheduler = scheduler;
	
	SchedulerAddTask(scheduler, 1, &PrintMercury, &PrintMercury_Cleanup, mercury);
	venus_uid = SchedulerAddTask(scheduler, 2, &PrintVenus, &PrintVenus_Cleanup, &venus_malloc);
	
	mercury->uid = venus_uid;
	
	SchedulerRun(scheduler);
	
	if (1 != venus_count)
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (4 != mercury_count)
	{
		res = TEST_STATUS_FAILURE;
	}

	venus_count = 0;
	mercury_count = 0;
	
	
	SchedulerDestroy(scheduler);
	free(mercury);
	return res;
}
