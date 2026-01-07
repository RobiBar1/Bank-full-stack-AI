#include <stdio.h>
#include <string.h>

#include "circular_buffer.h"

#define COLOR_RED		"\033[31m"
#define COLOR_GREEN		"\033[32m"
#define COLOR_YELLOW	"\033[33m"
#define COLOR_BLUE		"\033[34m"
#define COLOR_RESET		"\033[0m"
#define PASS (1)
#define FAIL (0)
#define UNUSED(x) ((void)(x))
#define RUN_TEST(suite, test_fn)							\
do															\
{															\
	test_result_t result = {0};								\
	(suite).total++;										\
															\
	if (test_fn(&result) == PASS)							\
	{														\
		(suite).passed++;									\
	}														\
	else													\
	{														\
		printf(COLOR_RED "[FAIL] %s: %s\n" COLOR_RESET,		\
				#test_fn, result.message);					\
	}														\
} while (0)


typedef struct
{
	size_t total;
	size_t passed;
} test_suite_t;

typedef struct
{
	char message[256];
} test_result_t;

typedef struct
{
	char buffer[256];
} fake_buffer_t;


static void PrintTestSummary(test_suite_t* suite_ptr)
{
	if (suite_ptr->passed == suite_ptr->total)
	{
		printf(COLOR_GREEN "[PASS] ");
	}
	else
	{
		printf(COLOR_RED "[FAIL] ");
	}
	printf("%lu/%lu passed\n" COLOR_RESET, suite_ptr->passed, suite_ptr->total);
}

static int TestCbuffIsEmpty(cbuff_t* cbuff, test_result_t* result)
{
	if (!CbuffIsEmpty(cbuff))
	{
		sprintf(result->message,
				COLOR_RED "buffer is not empty!\n" COLOR_RESET);
		return FAIL;
	}

	return PASS;
}

static int TestCbuffSize(cbuff_t* cbuff, size_t expected_size, test_result_t* result)
{
	size_t res_size = CbuffBufSize(cbuff);

	if (res_size != expected_size)
	{
		sprintf(result->message,
				COLOR_RED "Buffer is of wrong size! Got %lu, expected %lu" COLOR_RESET,
				res_size, expected_size);
		return FAIL;
	}

	return PASS;
}

static int TestCbuffFreeSpace(cbuff_t* cbuff, size_t expected_free_space, test_result_t* result)
{
	size_t res_free_space = CbuffGetFreeSpace(cbuff);

	if (res_free_space != expected_free_space)
	{
		sprintf(result->message,
				COLOR_RED "Free space is of wrong size! Got %lu, expected %lu" COLOR_RESET,
				res_free_space, expected_free_space);
		return FAIL;
	}

	return PASS;
}

static int CheckCbuffWrite(
	cbuff_t* cbuff,
	ssize_t res,
	ssize_t expected_res,
	size_t expected_free_space,
	test_result_t* result)
{
	if (res != expected_res)
	{
		sprintf(result->message,
				COLOR_RED "Wrong amount of bytes written! Got %ld, expected %ld" COLOR_RESET,
				res, expected_res);
		return FAIL;
	}
	if (FAIL == TestCbuffFreeSpace(cbuff, expected_free_space, result))
	{
		return FAIL;
	}

	return PASS;
}

static int CheckCbuffRead(
	cbuff_t* cbuff,
	ssize_t res,
	char* data,
	ssize_t expected_res,
	char* expected_data,
	size_t expected_free_space,
	test_result_t* result)
{
	if (res != expected_res)
	{
		sprintf(result->message,
				COLOR_RED "Wrong amount of bytes read! Got %ld, expected %ld" COLOR_RESET,
				res, expected_res);
		return FAIL;
	}
	if (FAIL == TestCbuffFreeSpace(cbuff, expected_free_space, result))
	{
		return FAIL;
	}
	if (strncmp(data, expected_data, expected_res) != 0)
	{
		sprintf(result->message,
				COLOR_RED "Wrong data read! Got \"%s\", expected \"%s\"" COLOR_RESET,
				data, expected_data);
		return FAIL;
	}

	return PASS;
}

static int TestCbuffCreation(test_result_t* result)
{
	size_t capacity = 4;
	cbuff_t* cbuff = CbuffCreate(capacity);
	size_t expected_free_space  = 4;

	if (FAIL == TestCbuffIsEmpty(cbuff, result))
	{
		return FAIL;
	}
	if (FAIL == TestCbuffSize(cbuff, capacity, result))
	{
		return FAIL;
	}
	if (FAIL == TestCbuffFreeSpace(cbuff, expected_free_space, result))
	{
		return FAIL;
	}

	CbuffDestroy(cbuff);

	return PASS;
}

static void RunTestCbuffCreation()
{
	test_suite_t suite = {0};

	printf(COLOR_BLUE "Run tests for Circular Buffer creation\n" COLOR_RESET);

	RUN_TEST(suite, TestCbuffCreation);

	PrintTestSummary(&suite);
	printf(COLOR_BLUE "Finished test\n" COLOR_RESET);
}

static int TestCbuffWriteNothing(test_result_t* result)
{
	size_t capacity = 4;
	cbuff_t* cbuff = CbuffCreate(capacity);
	char* data = "abc";
	ssize_t res = 0;
	ssize_t expected_res = 0;
	size_t expected_free_space = 4;
	int status = PASS;

	res = CbuffWrite(cbuff, data, 0);
	status = CheckCbuffWrite(cbuff, res, expected_res, expected_free_space, result);

	CbuffDestroy(cbuff);

	return status;
}

static int TestCbuffWriteLessThanCapacity(test_result_t* result)
{
	size_t capacity = 4;
	cbuff_t* cbuff = CbuffCreate(capacity);
	char* data = "abc";
	ssize_t res = 0;
	ssize_t expected_res = 2;
	size_t expected_free_space = 2;
	int status = PASS;

	res = CbuffWrite(cbuff, data, expected_res);
	status = CheckCbuffWrite(cbuff, res, expected_res, expected_free_space, result);

	CbuffDestroy(cbuff);

	return status;
}

static int TestCbuffWriteLessThanCapacityTwice(test_result_t* result)
{
	size_t capacity = 4;
	cbuff_t* cbuff = CbuffCreate(capacity);
	char* data = "abc";
	ssize_t res = 0;
	ssize_t expected_res = 2;
	size_t expected_free_space = 0;
	int status = PASS;

	res = CbuffWrite(cbuff, data, expected_res);
	res = CbuffWrite(cbuff, data, expected_res);
	status = CheckCbuffWrite(cbuff, res, expected_res, expected_free_space, result);

	CbuffDestroy(cbuff);

	return status;
}

static int TestCbuffWriteFullCapacity(test_result_t* result)
{
	size_t capacity = 4;
	cbuff_t* cbuff = CbuffCreate(capacity);
	char* data = "abc";
	ssize_t res = 0;
	ssize_t expected_res = 4;
	size_t expected_free_space = 0;
	int status = PASS;

	res = CbuffWrite(cbuff, data, expected_res);
	status = CheckCbuffWrite(cbuff, res, expected_res, expected_free_space, result);

	CbuffDestroy(cbuff);

	return status;
}

static int TestCbuffWriteMoreThanCapacity(test_result_t* result)
{
	size_t capacity = 4;
	cbuff_t* cbuff = CbuffCreate(capacity);
	char* data = "abc";
	ssize_t res = 0;
	ssize_t expected_res = 4;
	size_t expected_free_space = 0;
	int status = PASS;

	res = CbuffWrite(cbuff, data, expected_res + 1);
	status = CheckCbuffWrite(cbuff, res, expected_res, expected_free_space, result);

	CbuffDestroy(cbuff);

	return status;
}

static int TestCbuffWriteInvalidBuffer(test_result_t* result)
{
	fake_buffer_t buffer = {0};
	char* data = "abc";
	ssize_t res = 0;
	ssize_t expected = -1;

	res = CbuffWrite((cbuff_t*)&buffer, data, 4);
	if (res != expected)
	{
		sprintf(result->message,
				COLOR_RED "Wrong amount of bytes written! Got %ld, expected %ld" COLOR_RESET,
				res, expected);
		return FAIL;
	}

	return PASS;
}

static void RunTestCbuffWrite()
{
	test_suite_t suite = {0};

	printf(COLOR_BLUE "Run tests for Write\n" COLOR_RESET);

	RUN_TEST(suite, TestCbuffWriteNothing);
	RUN_TEST(suite, TestCbuffWriteLessThanCapacity);
	RUN_TEST(suite, TestCbuffWriteLessThanCapacityTwice);
	RUN_TEST(suite, TestCbuffWriteFullCapacity);
	RUN_TEST(suite, TestCbuffWriteMoreThanCapacity);
	RUN_TEST(suite, TestCbuffWriteInvalidBuffer);

	PrintTestSummary(&suite);
	printf(COLOR_BLUE "Finished tests\n" COLOR_RESET);
}

static int TestCbuffReadEmptyBuffer(test_result_t* result)
{
	size_t capacity = 4;
	cbuff_t* cbuff = CbuffCreate(capacity);
	char data[4] = "abc";
	char buffer[4] = {'\0'};
	ssize_t res = 0;
	ssize_t expected_res = 0;
	size_t expected_free_space = 4;
	int status = PASS;

	res = CbuffRead(cbuff, buffer, capacity);
	status = CheckCbuffRead(cbuff, res, buffer, expected_res, data, expected_free_space, result);

	CbuffDestroy(cbuff);

	return status;
}

static int TestCbuffReadLessThanBufferSize(test_result_t* result)
{
	size_t capacity = 4;
	cbuff_t* cbuff = CbuffCreate(capacity);
	char* data = "abc";
	char buffer[4] = {'\0'};
	ssize_t res = 0;
	ssize_t expected_res = 2;
	size_t expected_free_space = 2;
	int status = PASS;

	CbuffWrite(cbuff, data, capacity);
	res = CbuffRead(cbuff, buffer, expected_res);
	status = CheckCbuffRead(cbuff, res, buffer, expected_res, data, expected_free_space, result);

	CbuffDestroy(cbuff);

	return status;
}

static int TestCbuffReadLessThanBufferSizeTwice(test_result_t* result)
{
	size_t capacity = 4;
	cbuff_t* cbuff = CbuffCreate(capacity);
	char* data = "abc";
	char buffer[4] = {'\0'};
	ssize_t res = 0;
	ssize_t expected_res = 2;
	size_t expected_free_space = 4;
	int status = PASS;

	CbuffWrite(cbuff, data, capacity);
	res = CbuffRead(cbuff, buffer, expected_res);
	res = CbuffRead(cbuff, buffer + expected_res, expected_res);
	status = CheckCbuffRead(cbuff, res, buffer, expected_res, data, expected_free_space, result);

	CbuffDestroy(cbuff);

	return status;
}

static int TestCbuffReadFullBuffer(test_result_t* result)
{
	size_t capacity = 4;
	cbuff_t* cbuff = CbuffCreate(capacity);
	char* data = "abc";
	char buffer[4] = {'\0'};
	ssize_t res = 0;
	ssize_t expected_res = 4;
	size_t expected_free_space = 4;
	int status = PASS;

	CbuffWrite(cbuff, data, capacity);
	res = CbuffRead(cbuff, buffer, expected_res);
	status = CheckCbuffRead(cbuff, res, buffer, expected_res, data, expected_free_space, result);

	CbuffDestroy(cbuff);

	return status;
}

static int TestCbuffReadMoreThanBufferSize(test_result_t* result)
{
	size_t capacity = 4;
	cbuff_t* cbuff = CbuffCreate(capacity);
	char* data = "abc";
	char buffer[4] = {'\0'};
	ssize_t res = 0;
	ssize_t expected_res = 2;
	size_t expected_free_space = 4;
	int status = PASS;

	CbuffWrite(cbuff, data, expected_res);
	res = CbuffRead(cbuff, buffer, capacity);
	status = CheckCbuffRead(cbuff, res, buffer, expected_res, data, expected_free_space, result);

	CbuffDestroy(cbuff);

	return status;
}

static int TestCbuffReadInvalidBuffer(test_result_t* result)
{
	fake_buffer_t buffer = {'\0'};
	char dest_buffer[4] = {'\0'};
	ssize_t res = 0;
	ssize_t expected = -1;

	res = CbuffRead((cbuff_t*)&buffer, dest_buffer, 4);
	if (res != expected)
	{
		sprintf(result->message,
				COLOR_RED "Wrong amount of bytes written! Got %ld, expected %ld" COLOR_RESET,
				res, expected);
		return FAIL;
	}

	return PASS;
}

static void RunTestCbuffRead()
{
	test_suite_t suite = {0};

	printf(COLOR_BLUE "Run tests for Read\n" COLOR_RESET);

	RUN_TEST(suite, TestCbuffReadEmptyBuffer);
	RUN_TEST(suite, TestCbuffReadLessThanBufferSize);
	RUN_TEST(suite, TestCbuffReadLessThanBufferSizeTwice);
	RUN_TEST(suite, TestCbuffReadFullBuffer);
	RUN_TEST(suite, TestCbuffReadMoreThanBufferSize);
	RUN_TEST(suite, TestCbuffReadInvalidBuffer);

	PrintTestSummary(&suite);
	printf(COLOR_BLUE "Finished tests\n" COLOR_RESET);
}

static int TestCbuffWrapAroundByte(test_result_t* result)
{
	size_t capacity = 4;
	cbuff_t* cbuff = CbuffCreate(capacity);
	char* data = "abc";
	char buffer[4] = {'\0'};
	ssize_t res = 0;
	ssize_t expected_res = 1;
	size_t expected_free_space = 3;
	int status = PASS;

	CbuffWrite(cbuff, data, capacity);
	CbuffRead(cbuff, buffer, capacity);
	res = CbuffWrite(cbuff, data, expected_res);
	status = CheckCbuffWrite(cbuff, res, expected_res, expected_free_space, result);

	expected_free_space = 4;
	res = CbuffRead(cbuff, buffer, expected_res);
	buffer[expected_res] = '\0';
	status = CheckCbuffRead(cbuff, res, buffer, expected_res, data, expected_free_space, result);

	CbuffDestroy(cbuff);

	return status;
}

static int TestCbuffWrapAroundCapacity(test_result_t* result)
{
	size_t capacity = 4;
	cbuff_t* cbuff = CbuffCreate(capacity);
	char* data = "abc";
	char buffer[4] = {'\0'};
	ssize_t res = 0;
	ssize_t expected_res = capacity;
	size_t expected_free_space = 0;
	int status = PASS;

	CbuffWrite(cbuff, data, capacity);
	CbuffRead(cbuff, buffer, capacity);
	res = CbuffWrite(cbuff, data, capacity);
	status = CheckCbuffWrite(cbuff, res, expected_res, expected_free_space, result);

	expected_free_space = 4;
	res = CbuffRead(cbuff, buffer, expected_res);
	buffer[expected_res] = '\0';
	status = CheckCbuffRead(cbuff, res, buffer, expected_res, data, expected_free_space, result);

	CbuffDestroy(cbuff);

	return status;
}

static void RunTestCbuffWriteAndReadWrapAround()
{
	test_suite_t suite = {0};

	printf(COLOR_BLUE "Run tests for wrap around\n" COLOR_RESET);

	RUN_TEST(suite, TestCbuffWrapAroundByte);
	RUN_TEST(suite, TestCbuffWrapAroundCapacity);

	PrintTestSummary(&suite);
	printf(COLOR_BLUE "Finished tests\n" COLOR_RESET);
}

static int TestCbuffFlushEmptyBuffer(test_result_t* result)
{
	size_t capacity = 4;
	cbuff_t* cbuff = CbuffCreate(capacity);
	int status = PASS;

	CbuffFlush(cbuff);
	status = TestCbuffIsEmpty(cbuff, result);

	CbuffDestroy(cbuff);

	return status;
}

static int TestCbuffFlushHalfFull(test_result_t* result)
{
	size_t capacity = 4;
	cbuff_t* cbuff = CbuffCreate(capacity);
	char* data = "abc";
	int status = PASS;

	CbuffWrite(cbuff, data, 2);
	CbuffFlush(cbuff);
	status = TestCbuffIsEmpty(cbuff, result);

	CbuffDestroy(cbuff);

	return status;
}

static int TestCbuffFlushFullBuffer(test_result_t* result)
{
	size_t capacity = 4;
	cbuff_t* cbuff = CbuffCreate(capacity);
	char* data = "abc";
	int status = PASS;

	CbuffWrite(cbuff, data, 4);
	CbuffFlush(cbuff);
	status = TestCbuffIsEmpty(cbuff, result);

	CbuffDestroy(cbuff);

	return status;
}

static void RunTestCbuffFlush()
{
	test_suite_t suite = {0};

	printf(COLOR_BLUE "Run tests for wrap around\n" COLOR_RESET);

	RUN_TEST(suite, TestCbuffFlushEmptyBuffer);
	RUN_TEST(suite, TestCbuffFlushHalfFull);
	RUN_TEST(suite, TestCbuffFlushFullBuffer);

	PrintTestSummary(&suite);
	printf(COLOR_BLUE "Finished tests\n" COLOR_RESET);
}

static void RunTests()
{
	RunTestCbuffCreation();
	RunTestCbuffWrite();
	RunTestCbuffRead();
	RunTestCbuffWriteAndReadWrapAround();
	RunTestCbuffFlush();
}

int main()
{
	RunTests();

	return 0;
}

