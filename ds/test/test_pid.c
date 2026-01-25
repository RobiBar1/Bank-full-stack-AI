#include <stdio.h>
#include "pid.h"

#define GREEN "\033[0;32m"
#define RED   "\033[0;31m"
#define RESET "\033[0m"

static void test_IsEqual(void);
static void test_Create(void);

int main() 
{
    test_Create();
    test_IsEqual();
    return 0;
}

static void test_IsEqual(void)
{
	ilrd_uid_t tmp = ILRDUIDCreate();
    ilrd_uid_t tmp1;
	printf("--------------------- Start test_IsEqual------------------------\n");
	sleep(1);
	tmp1 = ILRDUIDCreate();
	if (0 == IsILRDUIDEqual(&tmp, &tmp))
	{
		printf("IsILRDUIDEqual FAIL for equals\n");
		return;
	}
	if (0 == IsILRDUIDEqual(&tmp, &tmp))
	{
		printf("IsILRDUIDEqual FAIL for Non-equals\n");
		return;
	}
	
	printf("IsILRDUIDEqual PASS all tests\n");
	printf("--------------------- End test_IsEqual------------------------\n");
}

static void test_Create(void)
{
	ilrd_uid_t tmp = ILRDUIDCreate();
    ilrd_uid_t tmp1;
	printf("--------------------- Start test_Create------------------------\n");
    
    printf("Counter: %lu\n", tmp.counter);
    printf("PID: %d\n", tmp.pid);
    printf("Time: %ld\n", tmp.time);
    printf("IP: %s\n", tmp.ip);
    sleep(1);
    tmp1 = ILRDUIDCreate();
    
    printf("Counter: %lu\n", tmp1.counter);
    printf("PID: %d\n", tmp1.pid);
    printf("Time: %ld\n", tmp1.time);
    printf("IP: %s\n", tmp1.ip);
    printf("-----------------------------------------------\n");
    printf("Counter: %lu\n", tmp.counter);
    printf("PID: %d\n", tmp.pid);
    printf("Time: %ld\n", tmp.time);
    printf("IP: %s\n", tmp.ip);
    
	printf("--------------------- End test_Create------------------------\n");
}
