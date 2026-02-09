#include <stdio.h> /* printf */
#include <assert.h> /* assert */

#include "stack.h"

#include "recursion.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

#define APPLY_TEST(func_call) CheckTest(func_call, #func_call)

typedef enum {TEST_STATUS_SUCCESS, TEST_STATUS_FAILURE} test_status_t;

/* gd test_recursion.c ../src/recursion.c ../src/stack.c -I ../include/ */


/* ==== FORWARD DELARATIONS ==== */

static void CheckTest(test_status_t result, char* name);


static test_status_t FibonacciIteratively_Simple_Test(void);
static test_status_t FibonacciIteratively_Simple2_Test(void);
static test_status_t FibonacciIteratively_Second_Test(void);
static test_status_t FibonacciRecursion_Simple_Test(void);
static test_status_t FibonacciRecursion_Simple2_Test(void);
static test_status_t Flip_Basic_Test(void);
static test_status_t Flip_OnlyOne_Test(void);
static test_status_t Strlen_Test(void);
static test_status_t Strcmp_Test(void);
static test_status_t Strcmp_Basic2_Test(void);
static test_status_t StrCpy_Basic_Test(void);
static test_status_t StrCat_Basic_Test(void);
static test_status_t StrStr_Basic_Test(void);
static test_status_t StrStr_Basic2_Test(void);
static test_status_t StackSort_OnlyTwo_Test(void);
static test_status_t StackSort_Three_Test(void);
static test_status_t StackSort_Basic_Test(void);


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

	APPLY_TEST(FibonacciIteratively_Simple_Test());
	APPLY_TEST(FibonacciIteratively_Simple2_Test());
	APPLY_TEST(FibonacciIteratively_Second_Test());
	APPLY_TEST(FibonacciRecursion_Simple_Test());
	APPLY_TEST(FibonacciRecursion_Simple2_Test());
	/*
	APPLY_TEST(Flip_Basic_Test());
	APPLY_TEST(Flip_OnlyOne_Test());
	*/
	APPLY_TEST(Strlen_Test());
	APPLY_TEST(Strcmp_Test());
	APPLY_TEST(Strcmp_Basic2_Test());
	APPLY_TEST(StrCpy_Basic_Test());
	APPLY_TEST(StrCat_Basic_Test());
	APPLY_TEST(StrStr_Basic_Test());
	APPLY_TEST(StrStr_Basic2_Test());
	/*
	APPLY_TEST(StackSort_OnlyTwo_Test());
	APPLY_TEST(StackSort_Three_Test());
	APPLY_TEST(StackSort_Basic_Test());
	*/
	return 0;
	
}

/* ==== HELPER FUNCTIONS ==== */

static int StackIsSorted(stack_t* stack)
{
	int popped_top = 0;
	int curr_top = 0;
	
	if (StackIsEmpty(stack))
	{
		return 1;
	}
	
	StackPeek(stack, &popped_top);
	StackPop(stack);
	
	while(!StackIsEmpty(stack))
	{
		StackPeek(stack, &curr_top);
		StackPop(stack);
		
		if (curr_top < popped_top)
		{
			return 0;
		}
		
		popped_top = curr_top;
	}
	
	return 1;
}


/* ==== TEST FUNCTIONS ==== */


static test_status_t FibonacciIteratively_Simple_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	
	if (5 != FibonacciIteratively(5))
	{
		res = TEST_STATUS_FAILURE;
	}

	return res;
}

static test_status_t FibonacciIteratively_Simple2_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	
	if (21 != FibonacciIteratively(8))
	{
		res = TEST_STATUS_FAILURE;
	}

	return res;
}

static test_status_t FibonacciIteratively_Second_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	
	if (1 != FibonacciIteratively(2))
	{
		res = TEST_STATUS_FAILURE;
	}

	return res;
}

static test_status_t FibonacciRecursion_Simple_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	int x = FibonacciRecursion(5);
	if (5 != x)
	{
		res = TEST_STATUS_FAILURE;
	}

	return res;
}

static test_status_t FibonacciRecursion_Simple2_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	
	if (21 != FibonacciRecursion(8))
	{
		res = TEST_STATUS_FAILURE;
	}

	return res;
}
/*
static test_status_t Flip_Basic_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;
	
	sll_node_t n4 = {NULL, 4};
	sll_node_t n3 = {NULL, 3};
	sll_node_t n2 = {NULL, 2};
	sll_node_t n1 = {NULL, 1};
	sll_node_t* new_head = NULL;
	n2.next = &n3;
	n3.next = &n4;
	n1.next = &n2;
	
	new_head = FlipList(&n1);
	
	if (&n3 != n4.next)
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (&n2 != n3.next)
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (&n1 != n2.next)
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (NULL != n1.next)
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (&n4 != new_head)
	{
		res = TEST_STATUS_FAILURE;
	}

	return res;
}

static test_status_t Flip_OnlyOne_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;

	sll_node_t n1 = {NULL, 1};
	sll_node_t* new_head = NULL;
	
	new_head = FlipList(&n1);
	
	if (NULL != n1.next)
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (&n1 != new_head)
	{
		res = TEST_STATUS_FAILURE;
	}

	return res;
}
*/
static test_status_t Strlen_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;

	char str[] = "hello mars";
	
	size_t count = Strlen(str);
	
	if (10 != count)
	{
		res = TEST_STATUS_FAILURE;
	}

	return res;
}

static test_status_t Strcmp_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;

	char str1[] = "hello mars";
	char str2[] = "hello mars";
	
	int ans = StrCmp(str1, str2);
	
	if (0 != ans)
	{
		res = TEST_STATUS_FAILURE;
	}

	return res;
}

static test_status_t Strcmp_Basic2_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;

	char str1[] = "hello m4rs";
	char str2[] = "hello mars";
	
	int ans = StrCmp(str1, str2);
	
	if (0 <= ans)
	{
		res = TEST_STATUS_FAILURE;
	}

	return res;
}

static test_status_t StrCpy_Basic_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;

	const char src[] = "hello mars";
	char dst[] = "xxxxxxxxxxxxxxxxxx";
	
	char* dst_returned = StrCpy(dst, src);
	
	if (dst_returned != dst)
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (0 != StrCmp(src, dst))
	{
		res = TEST_STATUS_FAILURE;
	}

	return res;
}

static test_status_t StrCat_Basic_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;

	const char src[] = "hello mars";
	char dst[] = "hello neptune, \0xxxxxxxxxxxxxxxxxxxxx";
	char ans[] = "hello neptune, hello mars";
	
	char* dst_returned = StrCat(dst, src);
	
	if (dst_returned != dst)
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (0 != StrCmp(ans, dst))
	{
		res = TEST_STATUS_FAILURE;
	}

	return res;
}

static test_status_t StrStr_Basic_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;

	const char haystack[] = "hellomarsagain";
	char needle[] = "mars";
	char ans_ret[] = "marsagain";
	
	char* haystack_ret = StrStr(haystack, needle);
	
	if (0 != StrCmp(haystack_ret, ans_ret))
	{
		res = TEST_STATUS_FAILURE;
	}

	return res;
}

static test_status_t StrStr_Basic2_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;

	const char haystack[] = "hellom4rsagain";
	char needle[] = "mars";
	
	char* haystack_ret = StrStr(haystack, needle);
	
	if (NULL != haystack_ret)
	{
		res = TEST_STATUS_FAILURE;
	}

	return res;
}
/*
static test_status_t StackSort_OnlyTwo_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;

	stack_t* stack = StackCreate(10, sizeof(int));
	int s1 = 5;
	int s2 = 3;
	
	StackPush(stack, &s1);
	StackPush(stack, &s2);
	
	SortStack(stack);
	
	if (2 != StackSize(stack))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (!StackIsSorted(stack))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	StackDestroy(stack);

	return res;
}

static test_status_t StackSort_Three_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;

	stack_t* stack = StackCreate(10, sizeof(int));
	int s1 = 5;
	int s2 = 3;
	int s3 = 6;
	
	StackPush(stack, &s1);
	StackPush(stack, &s2);
	StackPush(stack, &s3);
	
	SortStack(stack);
	
	if (3 != StackSize(stack))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (!StackIsSorted(stack))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	StackDestroy(stack);

	return res;
}

static test_status_t StackSort_Basic_Test(void)
{
	test_status_t res = TEST_STATUS_SUCCESS;

	stack_t* stack = StackCreate(10, sizeof(int));
	int s1 = 5;
	int s2 = 3;
	int s3 = 6;
	int s4 = 2;
	int s5 = 1;
	int s6 = 4;
	
	StackPush(stack, &s1);
	StackPush(stack, &s2);
	StackPush(stack, &s3);
	StackPush(stack, &s4);
	StackPush(stack, &s5);
	StackPush(stack, &s6);
	
	SortStack(stack);
	
	if (6 != StackSize(stack))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	if (!StackIsSorted(stack))
	{
		res = TEST_STATUS_FAILURE;
	}
	
	StackDestroy(stack);

	return res;
}
*/










