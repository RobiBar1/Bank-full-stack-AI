#include <stdio.h>

#include "sorted_list.h"

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


static int ComparePointers(const void* one, const void* other)
{
	if (one > other)
	{
		return 1;
	}

	if (other > one)
	{
		return -1;
	}

	return 0;
}

static void PrintTestSummary(test_suite_t* suite_ptr)
{
	if (suite_ptr->passed == suite_ptr->total)
	{
		printf(COLOR_GREEN "PASS: ");
	}
	else
	{
		printf(COLOR_RED "FAIL: ");
	}
	printf("%lu/%lu passed\n" COLOR_RESET, suite_ptr->passed, suite_ptr->total);
}

static int CheckListIsEmpty(sorted_list_t* list, test_result_t* result)
{
	int status = PASS;

	if (!SortedLIsEmpty(list))
	{
		sprintf(result->message, "List is not empty!");
		status = FAIL;
	}

	return status;
}

static int CheckListSize(sorted_list_t* list, size_t expected_size, test_result_t* result)
{
	size_t size = SortedLCount(list);

	if (size != expected_size)
	{
		sprintf(result->message,
				COLOR_RED "List is of Wrong Size! Got %lu, expected %lu" COLOR_RESET,
				size, expected_size);
		return FAIL;
	}

	return PASS;
}

static int CheckValues(void* res, void* expected, test_result_t* result)
{
	if (res != expected)
	{
		sprintf(result->message,
				COLOR_RED "Iter value is different! Got %p, expected %p",
				res, expected);
		return FAIL;
	}

	return PASS;
}

static int CheckListValues(
	sorted_list_t* list,
	void* expected_list[],
	test_result_t* result)
{
	sorted_list_iter_t iter = SortedLBegin(list);
	sorted_list_iter_t iter_end = SortedLEnd(list);
	size_t i = 0;

	for (i = 0; !SortedLIsIterEqual(iter, iter_end); iter = SortedLNext(iter), ++i)
	{
		if (SortedLGetData(iter) != expected_list[i])
		{
			sprintf(result->message,
					COLOR_RED "Values in list are different at index %lu! Got %p, expected %p" COLOR_RESET,
					i, SortedLGetData(iter), expected_list[i]);
			return FAIL;
		}
	}

	return PASS;
}

static int CheckList(
	sorted_list_t* list,
	void* expected_list[],
	size_t expected_size,
	test_result_t* result)
{
	if (FAIL == CheckListSize(list, expected_size, result))
	{
		return FAIL;
	}
	if (FAIL == CheckListValues(list, expected_list, result))
	{
		return FAIL;
	}

	return PASS;
}

static int CheckIters(sorted_list_iter_t res, sorted_list_iter_t expected, test_result_t* result)
{
	if (!SortedLIsIterEqual(res, expected))
	{
		sprintf(result->message,
				COLOR_RED "Iters are not equal!" COLOR_RESET);
		return FAIL;
	}

	return PASS;
}

static int TestSortedListCreation(test_result_t* result)
{
	sorted_list_t* list = SortedLCreate(ComparePointers);

	if (FAIL == CheckListIsEmpty(list, result))
	{
		SortedLDestroy(list);
		return FAIL;
	}

	SortedLDestroy(list);

	return PASS;
}

static void RunTestSortedListCreation()
{
	test_suite_t suite = {0};

	printf(COLOR_BLUE "Run tests for SortedList creation\n" COLOR_RESET);

	RUN_TEST(suite, TestSortedListCreation);

	PrintTestSummary(&suite);
	printf(COLOR_BLUE "Finished tests\n");
}

static int TestSortedListInsertToEnd(test_result_t* result)
{
	sorted_list_t* list = SortedLCreate(ComparePointers);
	size_t i = 0;
	size_t expected_size = 4;
	void* expected_list[] = {(void*)0, (void*)1, (void*)2, (void*)3};
	int status = PASS;

	for (i = 0; i < expected_size; ++i)
	{
		SortedLInsert(list, (void*)i);
	}

	status = CheckList(list, expected_list, expected_size, result);

	SortedLDestroy(list);

	return status;
}

static int TestSortedListInsertToStart(test_result_t* result)
{
	sorted_list_t* list = SortedLCreate(ComparePointers);
	size_t i = 0;
	size_t expected_size = 4;
	void* expected_list[] = {(void*)0, (void*)1, (void*)2, (void*)3};
	int status = PASS;

	for (i = 0; i < expected_size; ++i)
	{
		SortedLInsert(list, (void*)(expected_size - i - 1));
	}

	status = CheckList(list, expected_list, expected_size, result);

	SortedLDestroy(list);

	return status;
}

static int TestSortedListInsertToMiddle(test_result_t* result)
{
	sorted_list_t* list = SortedLCreate(ComparePointers);
	size_t expected_size = 4;
	void* expected_list[] = {(void*)0, (void*)1, (void*)2, (void*)3};
	int status = PASS;

	SortedLInsert(list, (void*)0);
	SortedLInsert(list, (void*)2);
	SortedLInsert(list, (void*)1);
	SortedLInsert(list, (void*)3);

	status = CheckList(list, expected_list, expected_size, result);

	SortedLDestroy(list);

	return status;
}

static void RunTestSortedListInsert()
{
	test_suite_t suite = {0};

	printf(COLOR_BLUE "Run tests for SortedListInsert\n" COLOR_RESET);

	RUN_TEST(suite, TestSortedListInsertToEnd);
	RUN_TEST(suite, TestSortedListInsertToStart);
	RUN_TEST(suite, TestSortedListInsertToMiddle);

	PrintTestSummary(&suite);
	printf(COLOR_BLUE "Finished tests\n");
}

static int TestSortedListRemoveFromStart(test_result_t* result)
{
	sorted_list_t* list = SortedLCreate(ComparePointers);
	size_t expected_size = 3;
	void* expected_list[] = {(void*)1, (void*)2, (void*)3};
	int status = PASS;

	SortedLInsert(list, (void*)0);
	SortedLInsert(list, (void*)1);
	SortedLInsert(list, (void*)2);
	SortedLInsert(list, (void*)3);
	SortedLRemove(SortedLBegin(list));

	status = CheckList(list, expected_list, expected_size, result);

	SortedLDestroy(list);

	return status;
}

static int TestSortedListRemoveFromEnd(test_result_t* result)
{
	sorted_list_t* list = SortedLCreate(ComparePointers);
	size_t expected_size = 3;
	void* expected_list[] = {(void*)0, (void*)1, (void*)2};
	int status = PASS;

	SortedLInsert(list, (void*)0);
	SortedLInsert(list, (void*)1);
	SortedLInsert(list, (void*)2);
	SortedLInsert(list, (void*)3);
	SortedLRemove(SortedLPrev(SortedLEnd(list)));

	status = CheckList(list, expected_list, expected_size, result);

	SortedLDestroy(list);

	return status;
}

static int TestSortedListRemoveFromMiddle(test_result_t* result)
{
	sorted_list_t* list = SortedLCreate(ComparePointers);
	size_t expected_size = 3;
	void* expected_list[] = {(void*)0, (void*)2, (void*)3};
	int status = PASS;

	SortedLInsert(list, (void*)0);
	SortedLInsert(list, (void*)1);
	SortedLInsert(list, (void*)2);
	SortedLInsert(list, (void*)3);
	SortedLRemove(SortedLNext(SortedLBegin(list)));

	status = CheckList(list, expected_list, expected_size, result);

	SortedLDestroy(list);

	return status;
}

static void RunTestSortedListRemove()
{
	test_suite_t suite = {0};

	printf(COLOR_BLUE "Run tests for SortedListRemove\n" COLOR_RESET);

	RUN_TEST(suite, TestSortedListRemoveFromStart);
	RUN_TEST(suite, TestSortedListRemoveFromEnd);
	RUN_TEST(suite, TestSortedListRemoveFromMiddle);

	PrintTestSummary(&suite);
	printf(COLOR_BLUE "Finished tests\n");
}

static int TestSortedListPopFront(test_result_t* result)
{
	sorted_list_t* list = SortedLCreate(ComparePointers);
	void* expected_list[] = {(void*)0, (void*)1, (void*)2, (void*)3};
	size_t i = 0;
	int status = PASS;

	SortedLInsert(list, (void*)0);
	SortedLInsert(list, (void*)1);
	SortedLInsert(list, (void*)2);
	SortedLInsert(list, (void*)3);

	while (!SortedLIsEmpty(list) && PASS == status)
	{
		status = CheckValues(SortedLPopFront(list), expected_list[i++], result);
	}

	SortedLDestroy(list);

	return status;
}

static int TestSortedListPopBack(test_result_t* result)
{
	sorted_list_t* list = SortedLCreate(ComparePointers);
	void* expected_list[] = {(void*)0, (void*)1, (void*)2, (void*)3};
	size_t i = 3;
	int status = PASS;

	SortedLInsert(list, (void*)0);
	SortedLInsert(list, (void*)1);
	SortedLInsert(list, (void*)2);
	SortedLInsert(list, (void*)3);

	while (!SortedLIsEmpty(list) && PASS == status)
	{
		status = CheckValues(SortedLPopBack(list), expected_list[i--], result);
	}

	SortedLDestroy(list);

	return status;
}

static void RunTestSortedListPops()
{
	test_suite_t suite = {0};

	printf(COLOR_BLUE "Run tests for SortedListPops\n" COLOR_RESET);

	RUN_TEST(suite, TestSortedListPopFront);
	RUN_TEST(suite, TestSortedListPopBack);

	PrintTestSummary(&suite);
	printf(COLOR_BLUE "Finished tests\n");
}

static int TestSortedListFindBegin(test_result_t* result)
{
	sorted_list_t* list = SortedLCreate(ComparePointers);
	size_t i = 0;
	sorted_list_iter_t iter = {0};
	void* data = (void*)0;
	int status = PASS;

	for (i = 0; i < 4; ++i)
	{
		SortedLInsert(list, (void*)i);
	}

	iter = SortedLFind(list, SortedLBegin(list), SortedLEnd(list), data);

	status = CheckValues(SortedLGetData(iter), data, result);

	SortedLDestroy(list);

	return status;
}

static int TestSortedListFindMiddle(test_result_t* result)
{
	sorted_list_t* list = SortedLCreate(ComparePointers);
	size_t i = 0;
	sorted_list_iter_t iter = {0};
	void* data = (void*)2;
	int status = PASS;

	for (i = 0; i < 4; ++i)
	{
		SortedLInsert(list, (void*)i);
	}

	iter = SortedLFind(list, SortedLBegin(list), SortedLEnd(list), data);

	status = CheckValues(SortedLGetData(iter), data, result);

	SortedLDestroy(list);

	return status;
}

static int TestSortedListFindEnd(test_result_t* result)
{
	sorted_list_t* list = SortedLCreate(ComparePointers);
	size_t i = 0;
	sorted_list_iter_t iter = {0};
	void* data = (void*)3;
	int status = PASS;

	for (i = 0; i < 4; ++i)
	{
		SortedLInsert(list, (void*)i);
	}

	iter = SortedLFind(list, SortedLBegin(list), SortedLEnd(list), data);

	status = CheckValues(SortedLGetData(iter), data, result);

	SortedLDestroy(list);

	return status;
}

static int TestSortedListFindFailed(test_result_t* result)
{
	sorted_list_t* list = SortedLCreate(ComparePointers);
	size_t i = 0;
	sorted_list_iter_t iter = {0};
	void* data = (void*)4;
	int status = PASS;

	for (i = 0; i < 4; ++i)
	{
		SortedLInsert(list, (void*)i);
	}

	iter = SortedLFind(list, SortedLBegin(list), SortedLEnd(list), data);

	status = CheckIters(iter, SortedLEnd(list), result);

	SortedLDestroy(list);

	return status;
}

static int TestSortedListFindAfterRemove(test_result_t* result)
{
	sorted_list_t* list = SortedLCreate(ComparePointers);
	size_t i = 0;
	sorted_list_iter_t iter = {0};
	void* data = (void*)0;
	int status = PASS;

	for (i = 0; i < 4; ++i)
	{
		SortedLInsert(list, (void*)i);
	}
	SortedLPopFront(list);

	iter = SortedLFind(list, SortedLBegin(list), SortedLEnd(list), data);

	status = CheckIters(iter, SortedLEnd(list), result);

	SortedLDestroy(list);

	return status;
}

static void RunTestSortedListFind()
{
	test_suite_t suite = {0};

	printf(COLOR_BLUE "Run tests for SortedListFind\n" COLOR_RESET);

	RUN_TEST(suite, TestSortedListFindBegin);
	RUN_TEST(suite, TestSortedListFindMiddle);
	RUN_TEST(suite, TestSortedListFindEnd);
	RUN_TEST(suite, TestSortedListFindFailed);
	RUN_TEST(suite, TestSortedListFindAfterRemove);

	PrintTestSummary(&suite);
	printf(COLOR_BLUE "Finished tests\n");
}

static int is_match(const void* data, const void* param)
{
	return (void*)((size_t)data * 2) == param;
}

static int TestSortedListFindIfBegin(test_result_t* result)
{
	sorted_list_t* list = SortedLCreate(ComparePointers);
	size_t i = 0;
	sorted_list_iter_t iter = {0};
	void* data = (void*)0;
	int status = PASS;

	for (i = 0; i < 4; ++i)
	{
		SortedLInsert(list, (void*)i);
	}

	iter = SortedLFindIf(SortedLBegin(list), SortedLEnd(list), is_match, data);

	status = CheckValues(SortedLGetData(iter), data, result);

	SortedLDestroy(list);

	return status;
}

static int TestSortedListFindIfMiddle(test_result_t* result)
{
	sorted_list_t* list = SortedLCreate(ComparePointers);
	size_t i = 0;
	sorted_list_iter_t iter = {0};
	void* data = (void*)4;
	void* expected = (void*)2;
	int status = PASS;

	for (i = 0; i < 4; ++i)
	{
		SortedLInsert(list, (void*)i);
	}

	iter = SortedLFindIf(SortedLBegin(list), SortedLEnd(list), is_match, data);

	status = CheckValues(SortedLGetData(iter), expected, result);

	SortedLDestroy(list);

	return status;
}

static int TestSortedListFindIfEnd(test_result_t* result)
{
	sorted_list_t* list = SortedLCreate(ComparePointers);
	size_t i = 0;
	sorted_list_iter_t iter = {0};
	void* data = (void*)6;
	void* expected = (void*)3;
	int status = PASS;

	for (i = 0; i < 4; ++i)
	{
		SortedLInsert(list, (void*)i);
	}

	iter = SortedLFindIf(SortedLBegin(list), SortedLEnd(list), is_match, data);

	status = CheckValues(SortedLGetData(iter), expected, result);

	SortedLDestroy(list);

	return status;
}

static int TestSortedListFindIfFailed(test_result_t* result)
{
	sorted_list_t* list = SortedLCreate(ComparePointers);
	size_t i = 0;
	sorted_list_iter_t iter = {0};
	void* data = (void*)8;
	int status = PASS;

	for (i = 0; i < 4; ++i)
	{
		SortedLInsert(list, (void*)i);
	}

	iter = SortedLFindIf(SortedLBegin(list), SortedLEnd(list), is_match, data);

	status = CheckIters(iter, SortedLEnd(list), result);

	SortedLDestroy(list);

	return status;
}

static int TestSortedListFindIfAfterRemove(test_result_t* result)
{
	sorted_list_t* list = SortedLCreate(ComparePointers);
	size_t i = 0;
	sorted_list_iter_t iter = {0};
	void* data = (void*)0;
	int status = PASS;

	for (i = 0; i < 4; ++i)
	{
		SortedLInsert(list, (void*)i);
	}
	SortedLPopFront(list);

	iter = SortedLFindIf(SortedLBegin(list), SortedLEnd(list), is_match, data);

	status = CheckIters(iter, SortedLEnd(list), result);

	SortedLDestroy(list);

	return status;
}

static void RunTestSortedListFindIf()
{
	test_suite_t suite = {0};

	printf(COLOR_BLUE "Run tests for SortedListFindIf\n" COLOR_RESET);

	RUN_TEST(suite, TestSortedListFindIfBegin);
	RUN_TEST(suite, TestSortedListFindIfMiddle);
	RUN_TEST(suite, TestSortedListFindIfEnd);
	RUN_TEST(suite, TestSortedListFindIfFailed);
	RUN_TEST(suite, TestSortedListFindIfAfterRemove);

	PrintTestSummary(&suite);
	printf(COLOR_BLUE "Finished tests\n");
}

static int Count(void* data, void* param)
{
	(void)data;
	(*(size_t*)param)++;

	return 0;
}

static int TestSortedListForEach(test_result_t* result)
{
	sorted_list_t* list = SortedLCreate(ComparePointers);
	size_t i = 0;
	size_t counter = 0;
	void* expected = (void*)4;
	int status = PASS;

	for (i = 0; i < 4; ++i)
	{
		SortedLInsert(list, (void*)i);
	}

	SortedLForEach(SortedLBegin(list), SortedLEnd(list), Count, &counter);


	status = CheckValues((void*)counter, expected, result);

	SortedLDestroy(list);

	return status;
}

static void RunTestSortedListForEach()
{
	test_suite_t suite = {0};

	printf(COLOR_BLUE "Run tests for SortedListForEach\n" COLOR_RESET);

	RUN_TEST(suite, TestSortedListForEach);

	PrintTestSummary(&suite);
	printf(COLOR_BLUE "Finished tests\n");
}

static int TestSortedListMergeToStart(test_result_t* result)
{
	sorted_list_t* dst_list = SortedLCreate(ComparePointers);
	sorted_list_t* src_list = SortedLCreate(ComparePointers);
	void* expected_list[] = {(void*)0, (void*)1, (void*)2, (void*)3, (void*)4, (void*)5};
	int status = PASS;

	SortedLInsert(dst_list, expected_list[5]);
	SortedLInsert(dst_list, expected_list[4]);
	SortedLInsert(dst_list, expected_list[3]);
	SortedLInsert(src_list, expected_list[2]);
	SortedLInsert(src_list, expected_list[1]);
	SortedLInsert(src_list, expected_list[0]);

	SortedLMerge(dst_list, src_list);

	status = CheckList(dst_list, expected_list, 6, result) &&
			 CheckListIsEmpty(src_list, result);

	SortedLDestroy(dst_list);
	SortedLDestroy(src_list);

	return status;
}

static int TestSortedListMergeToMiddle(test_result_t* result)
{
	sorted_list_t* dst_list = SortedLCreate(ComparePointers);
	sorted_list_t* src_list = SortedLCreate(ComparePointers);
	void* expected_list[] = {(void*)0, (void*)1, (void*)2, (void*)3, (void*)4, (void*)5};
	int status = PASS;

	SortedLInsert(dst_list, expected_list[5]);
	SortedLInsert(dst_list, expected_list[4]);
	SortedLInsert(src_list, expected_list[3]);
	SortedLInsert(src_list, expected_list[2]);
	SortedLInsert(src_list, expected_list[1]);
	SortedLInsert(dst_list, expected_list[0]);

	SortedLMerge(dst_list, src_list);

	status = CheckList(dst_list, expected_list, 6, result) &&
			 CheckListIsEmpty(src_list, result);

	SortedLDestroy(dst_list);
	SortedLDestroy(src_list);

	return status;
}

static int TestSortedListMergeToEnd(test_result_t* result)
{
	sorted_list_t* dst_list = SortedLCreate(ComparePointers);
	sorted_list_t* src_list = SortedLCreate(ComparePointers);
	void* expected_list[] = {(void*)0, (void*)1, (void*)2, (void*)3, (void*)4, (void*)5};
	int status = PASS;

	SortedLInsert(src_list, expected_list[5]);
	SortedLInsert(src_list, expected_list[4]);
	SortedLInsert(src_list, expected_list[3]);
	SortedLInsert(dst_list, expected_list[2]);
	SortedLInsert(dst_list, expected_list[1]);
	SortedLInsert(dst_list, expected_list[0]);

	SortedLMerge(dst_list, src_list);

	status = CheckList(dst_list, expected_list, 6, result) &&
			 CheckListIsEmpty(src_list, result);

	SortedLDestroy(dst_list);
	SortedLDestroy(src_list);

	return status;
}

static int TestSortedListMergeOneByOne(test_result_t* result)
{
	sorted_list_t* dst_list = SortedLCreate(ComparePointers);
	sorted_list_t* src_list = SortedLCreate(ComparePointers);
	void* expected_list[] = {(void*)0, (void*)1, (void*)2, (void*)3, (void*)4, (void*)5};
	int status = PASS;

	SortedLInsert(dst_list, expected_list[5]);
	SortedLInsert(src_list, expected_list[4]);
	SortedLInsert(dst_list, expected_list[3]);
	SortedLInsert(src_list, expected_list[2]);
	SortedLInsert(dst_list, expected_list[1]);
	SortedLInsert(src_list, expected_list[0]);

	SortedLMerge(dst_list, src_list);

	status = CheckList(dst_list, expected_list, 6, result) &&
			 CheckListIsEmpty(src_list, result);

	SortedLDestroy(dst_list);
	SortedLDestroy(src_list);

	return status;
}

static int TestSortedListMergeTwoByTwo(test_result_t* result)
{
	sorted_list_t* dst_list = SortedLCreate(ComparePointers);
	sorted_list_t* src_list = SortedLCreate(ComparePointers);
	void* expected_list[] = {(void*)0, (void*)1, (void*)2, (void*)3, (void*)4, (void*)5};
	int status = PASS;

	SortedLInsert(dst_list, expected_list[5]);
	SortedLInsert(dst_list, expected_list[4]);
	SortedLInsert(src_list, expected_list[3]);
	SortedLInsert(src_list, expected_list[2]);
	SortedLInsert(dst_list, expected_list[1]);
	SortedLInsert(src_list, expected_list[0]);

	SortedLMerge(dst_list, src_list);

	status = CheckList(dst_list, expected_list, 6, result) &&
			 CheckListIsEmpty(src_list, result);

	SortedLDestroy(dst_list);
	SortedLDestroy(src_list);

	return status;
}

static int TestSortedListMergeEmptyDst(test_result_t* result)
{
	sorted_list_t* dst_list = SortedLCreate(ComparePointers);
	sorted_list_t* src_list = SortedLCreate(ComparePointers);
	void* expected_list[] = {(void*)0, (void*)1, (void*)2, (void*)3, (void*)4, (void*)5};
	int status = PASS;

	SortedLInsert(src_list, expected_list[5]);
	SortedLInsert(src_list, expected_list[4]);
	SortedLInsert(src_list, expected_list[3]);
	SortedLInsert(src_list, expected_list[2]);
	SortedLInsert(src_list, expected_list[1]);
	SortedLInsert(src_list, expected_list[0]);

	SortedLMerge(dst_list, src_list);

	status = CheckList(dst_list, expected_list, 6, result) &&
			 CheckListIsEmpty(src_list, result);

	SortedLDestroy(dst_list);
	SortedLDestroy(src_list);

	return status;
}

static int TestSortedListMergeEmptySrc(test_result_t* result)
{
	sorted_list_t* dst_list = SortedLCreate(ComparePointers);
	sorted_list_t* src_list = SortedLCreate(ComparePointers);
	void* expected_list[] = {(void*)0, (void*)1, (void*)2, (void*)3, (void*)4, (void*)5};
	int status = PASS;

	SortedLInsert(dst_list, expected_list[5]);
	SortedLInsert(dst_list, expected_list[4]);
	SortedLInsert(dst_list, expected_list[3]);
	SortedLInsert(dst_list, expected_list[2]);
	SortedLInsert(dst_list, expected_list[1]);
	SortedLInsert(dst_list, expected_list[0]);

	SortedLMerge(dst_list, src_list);

	status = CheckList(dst_list, expected_list, 6, result) &&
			 CheckListIsEmpty(src_list, result);

	SortedLDestroy(dst_list);
	SortedLDestroy(src_list);

	return status;
}

static int TestSortedListMergeBothEmpty(test_result_t* result)
{
	sorted_list_t* dst_list = SortedLCreate(ComparePointers);
	sorted_list_t* src_list = SortedLCreate(ComparePointers);
	int status = PASS;

	SortedLMerge(dst_list, src_list);

	status = CheckListIsEmpty(dst_list, result) &&
			 CheckListIsEmpty(src_list, result);

	SortedLDestroy(dst_list);
	SortedLDestroy(src_list);

	return status;
}

static void RunTestSortedListMerge()
{
	test_suite_t suite = {0};

	printf(COLOR_BLUE "Run tests for SortedListMerge\n" COLOR_RESET);

	RUN_TEST(suite, TestSortedListMergeToStart);
	RUN_TEST(suite, TestSortedListMergeToMiddle);
	RUN_TEST(suite, TestSortedListMergeToEnd);
	RUN_TEST(suite, TestSortedListMergeOneByOne);
	RUN_TEST(suite, TestSortedListMergeTwoByTwo);
	RUN_TEST(suite, TestSortedListMergeEmptyDst);
	RUN_TEST(suite, TestSortedListMergeEmptySrc);
	RUN_TEST(suite, TestSortedListMergeBothEmpty);

	PrintTestSummary(&suite);
	printf(COLOR_BLUE "Finished tests\n");
}

static void RunTests()
{
	RunTestSortedListCreation();
	RunTestSortedListInsert();
	RunTestSortedListRemove();
	RunTestSortedListPops();
	RunTestSortedListFind();
	RunTestSortedListFindIf();
	RunTestSortedListForEach();
	RunTestSortedListMerge();
}

int main()
{
	RunTests();

	return 0;
}

