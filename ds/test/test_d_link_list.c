#include <stdio.h>

#include "d_link_list.h"

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

static int CheckListIsEmpty(dlist_t* list, test_result_t* result)
{
	int status = PASS;

	if (!DListIsEmpty(list))
	{
		sprintf(result->message, "List is not empty!\n");
		status = FAIL;
	}

	return status;
}

static int CheckListSize(dlist_t* list, size_t expected_size, test_result_t* result)
{
	size_t size = DListCount(list);

	if (size != expected_size)
	{
		sprintf(result->message,
				COLOR_RED "List is of Wrong Size! Got %lu, expected %lu" COLOR_RESET,
				size, expected_size);
		return FAIL;
	}

	return PASS;
}

static int CheckListValues(
	dlist_t* list,
	void* expected_list[],
	test_result_t* result)
{
	dlist_iter_t iter = DListBegin(list);
	dlist_iter_t iter_end =DListEnd(list);
	size_t i = 0;

	for (i = 0; !DListIsIterEqual(iter, iter_end); iter = DListGetNext(iter), ++i)
	{
		if (DListGetData(iter) != expected_list[i])
		{
			sprintf(result->message,
					COLOR_RED "Values in list are different at index %lu! Got %p, expected %p" COLOR_RESET,
					i, DListGetData(iter), expected_list[i]);
			return FAIL;
		}
	}

	return PASS;
}

static int CheckList(
	dlist_t* list,
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

static int CheckValues(void* res, void* expected, test_result_t* result)
{
	if (res != expected)
	{
		sprintf(result->message,
				COLOR_RED "Iter value is different! Got %p, expected %p\n",
				res, expected);
		return FAIL;
	}

	return PASS;
}

static int CheckIters(dlist_iter_t res, dlist_iter_t expected, test_result_t* result)
{
	if (!DListIsIterEqual(res, expected))
	{
		sprintf(result->message,
				COLOR_RED "Iters are not equal!\n" COLOR_RESET);
		return FAIL;
	}

	return PASS;
}

static int TestDListCreation(test_result_t* result)
{
	dlist_t* list = DListCreate();

	if (FAIL == CheckListIsEmpty(list, result))
	{
		DListDestroy(list);
		return FAIL;
	}

	DListDestroy(list);

	return PASS;
}

static void RunTestDListCreation()
{
	test_suite_t suite = {0};

	printf(COLOR_BLUE "Run tests for DList creation\n" COLOR_RESET);

	RUN_TEST(suite, TestDListCreation);

	PrintTestSummary(&suite);
	printf(COLOR_BLUE "Finished tests\n");
}

static int TestDListInsertToEnd(test_result_t* result)
{
	dlist_t* list = DListCreate();
	size_t i = 0;
	size_t expected_size = 4;
	void* expected_list[] = {(void*)0, (void*)1, (void*)2, (void*)3};
	int status = PASS;

	for (i = 0; i < expected_size; ++i)
	{
		DListInsert(DListEnd(list), (void*)i);
	}

	status = CheckList(list, expected_list, expected_size, result);

	DListDestroy(list);

	return status;
}

static int TestDListInsertToStart(test_result_t* result)
{
	dlist_t* list = DListCreate();
	size_t i = 0;
	size_t expected_size = 4;
	void* expected_list[] = {(void*)3, (void*)2, (void*)1, (void*)0};
	int status = PASS;

	for (i = 0; i < expected_size; ++i)
	{
		DListInsert(DListBegin(list), (void*)i);
	}

	status = CheckList(list, expected_list, expected_size, result);

	DListDestroy(list);

	return status;
}

static int TestDListInsertToMiddle(test_result_t* result)
{
	dlist_t* list = DListCreate();
	size_t i = 0;
	size_t expected_size = 4;
	void* expected_list[] = {(void*)0, (void*)1, (void*)3, (void*)2};
	int status = PASS;
	dlist_iter_t iter = {0};

	for (i = 0; i < expected_size - 1; ++i)
	{
		iter = DListInsert(DListEnd(list), (void*)i);
	}

	DListInsert(iter, (void*)i);

	status = CheckList(list, expected_list, expected_size, result);

	DListDestroy(list);

	return status;
}

static void RunTestDListInsert()
{
	test_suite_t suite = {0};

	printf(COLOR_BLUE "Run tests for DListInsert\n" COLOR_RESET);

	RUN_TEST(suite, TestDListInsertToEnd);
	RUN_TEST(suite, TestDListInsertToStart);
	RUN_TEST(suite, TestDListInsertToMiddle);

	PrintTestSummary(&suite);
	printf(COLOR_BLUE "Finished tests\n");
}

static int TestDListRemoveFromEnd(test_result_t* result)
{
	dlist_t* list = DListCreate();
	size_t i = 0;
	size_t expected_size = 3;
	void* expected_list[] = {(void*)0, (void*)1, (void*)2};
	dlist_iter_t iter = {0};
	int status = PASS;

	for (i = 0; i < expected_size + 1; ++i)
	{
		iter = DListInsert(DListEnd(list), (void*)i);
	}
	DListRemove(iter);

	status = CheckList(list, expected_list, expected_size, result);

	DListDestroy(list);

	return status;
}

static int TestDListRemoveFromStart(test_result_t* result)
{
	dlist_t* list = DListCreate();
	size_t i = 0;
	size_t expected_size = 3;
	void* expected_list[] = {(void*)1, (void*)2, (void*)3};
	int status = PASS;

	for (i = 0; i < expected_size + 1; ++i)
	{
		DListInsert(DListEnd(list), (void*)i);
	}

	DListRemove(DListBegin(list));

	status = CheckList(list, expected_list, expected_size, result);

	DListDestroy(list);

	return status;
}

static int TestDListRemoveFromMiddle(test_result_t* result)
{
	dlist_t* list = DListCreate();
	size_t i = 0;
	size_t expected_size = 3;
	void* expected_list[] = {(void*)0, (void*)1, (void*)3};
	int status = PASS;
	dlist_iter_t iter = {0};

	for (i = 0; i < expected_size + 1; ++i)
	{
		if (2 == i)
		{
			iter = DListInsert(DListEnd(list), (void*)i);
		}
		else
		{
			DListInsert(DListEnd(list), (void*)i);
		}
	}

	DListRemove(iter);

	status = CheckList(list, expected_list, expected_size, result);

	DListDestroy(list);

	return status;
}

static int TestDListRemoveTwiceFromStart(test_result_t* result)
{
	dlist_t* list = DListCreate();
	size_t i = 0;
	size_t expected_size = 2;
	void* expected_list[] = {(void*)2, (void*)3};
	dlist_iter_t iter = {0};
	int status = PASS;

	for (i = 0; i < expected_size + 2; ++i)
	{
		DListInsert(DListEnd(list), (void*)i);
	}

	iter = DListRemove(DListBegin(list));
	iter = DListRemove(iter);


	status = CheckList(list, expected_list, expected_size, result);

	DListDestroy(list);

	return status;
}

static void RunTestDListRemove()
{
	test_suite_t suite = {0};

	printf(COLOR_BLUE "Run tests for DListRemove\n" COLOR_RESET);

	RUN_TEST(suite, TestDListRemoveFromEnd);
	RUN_TEST(suite, TestDListRemoveFromStart);
	RUN_TEST(suite, TestDListRemoveFromMiddle);
	RUN_TEST(suite, TestDListRemoveTwiceFromStart);

	PrintTestSummary(&suite);
	printf(COLOR_BLUE "Finished tests\n");
}

static int TestDListPushBack(test_result_t* result)
{
	dlist_t* list = DListCreate();
	size_t i = 0;
	size_t expected_size = 4;
	void* expected_list[] = {(void*)0, (void*)1, (void*)2, (void*)3};
	int status = PASS;

	for (i = 0; i < expected_size; ++i)
	{
		DListPushBack(list, (void*)i);
	}

	status = CheckList(list, expected_list, expected_size, result);

	DListDestroy(list);

	return status;
}

static int TestDListPushFront(test_result_t* result)
{
	dlist_t* list = DListCreate();
	size_t i = 0;
	size_t expected_size = 4;
	void* expected_list[] = {(void*)3, (void*)2, (void*)1, (void*)0};
	int status = PASS;

	for (i = 0; i < expected_size; ++i)
	{
		DListPushFront(list, (void*)i);
	}

	status = CheckList(list, expected_list, expected_size, result);

	DListDestroy(list);

	return status;
}

static int TestDListPopBack(test_result_t* result)
{
	dlist_t* list = DListCreate();
	size_t i = 0;
	int status = PASS;

	for (i = 0; i < 4; ++i)
	{
		DListPushBack(list, (void*)i);
	}

	while (!DListIsEmpty(list) && PASS == status)
	{
		status = CheckValues(DListPopBack(list), (void*)(--i), result);
	}

	DListDestroy(list);

	return status;
}

static int TestDListPopFront(test_result_t* result)
{
	dlist_t* list = DListCreate();
	size_t i = 0;
	int status = PASS;

	for (i = 0; i < 4; ++i)
	{
		DListPushFront(list, (void*)i);
	}

	while (!DListIsEmpty(list) && PASS == status)
	{
		status = CheckValues(DListPopFront(list), (void*)(--i), result);
	}

	DListDestroy(list);

	return status;
}

static void RunTestDListPushAndPop()
{
	test_suite_t suite = {0};

	printf(COLOR_BLUE "Run tests for DList push and pop\n" COLOR_RESET);

	RUN_TEST(suite, TestDListPushBack);
	RUN_TEST(suite, TestDListPushFront);
	RUN_TEST(suite, TestDListPopBack);
	RUN_TEST(suite, TestDListPopFront);

	PrintTestSummary(&suite);
	printf(COLOR_BLUE "Finished tests\n");
}

static int TestDListSpliceFromBegin(test_result_t* result)
{
	dlist_t* src_list = DListCreate();
	dlist_t* dst_list = DListCreate();
	dlist_iter_t from = {0};
	dlist_iter_t to = {0};
	dlist_iter_t where = {0};
	void* expected_src[] = {(void*)2, (void*)3};
	void* expected_dst[] = {(void*)0, (void*)1, (void*)4, (void*)5, (void*)6, (void*)7};
	size_t expected_src_size = 2;
	size_t expected_dst_size = 6;
	size_t i = 0;
	int status = PASS;

	for (i = 0; i < 4; ++i)
	{
		DListPushBack(src_list, (void*)i);
	}
	for (; i < 8; ++i)
	{
		DListPushBack(dst_list, (void*)i);
	}

	from = DListBegin(src_list);
	to = DListGetPrev(DListGetPrev(DListEnd(src_list)));
	where = DListBegin(dst_list);

	DListSplice(from, to, where);

	status = CheckList(src_list, expected_src, expected_src_size, result) &&
			 CheckList(dst_list, expected_dst, expected_dst_size, result);

	DListDestroy(src_list);
	DListDestroy(dst_list);

	return status;
}

static int TestDListSpliceToEnd(test_result_t* result)
{
	dlist_t* src_list = DListCreate();
	dlist_t* dst_list = DListCreate();
	dlist_iter_t from = {0};
	dlist_iter_t to = {0};
	dlist_iter_t where = {0};
	void* expected_src[] = {(void*)0, (void*)1};
	void* expected_dst[] = {(void*)2, (void*)3, (void*)4, (void*)5, (void*)6, (void*)7};
	size_t expected_src_size = 2;
	size_t expected_dst_size = 6;
	size_t i = 0;
	int status = PASS;

	for (i = 0; i < 4; ++i)
	{
		DListPushBack(src_list, (void*)i);
	}
	for (; i < 8; ++i)
	{
		DListPushBack(dst_list, (void*)i);
	}

	from = DListGetNext(DListGetNext(DListBegin(src_list)));
	to = DListEnd(src_list);
	where = DListBegin(dst_list);

	DListSplice(from, to, where);

	status = CheckList(src_list, expected_src, expected_src_size, result) &&
			 CheckList(dst_list, expected_dst, expected_dst_size, result);

	DListDestroy(src_list);
	DListDestroy(dst_list);

	return status;
}

static int TestDListSpilceFromBeginToEnd(test_result_t* result)
{
	dlist_t* src_list = DListCreate();
	dlist_t* dst_list = DListCreate();
	dlist_iter_t from = {0};
	dlist_iter_t to = {0};
	dlist_iter_t where = {0};
	void* expected_dst[] = {(void*)0, (void*)1, (void*)2, (void*)3, (void*)4, (void*)5, (void*)6, (void*)7};
	size_t expected_dst_size = 8;
	size_t i = 0;
	int status = PASS;

	for (i = 0; i < 4; ++i)
	{
		DListPushBack(src_list, (void*)i);
	}
	for (; i < 8; ++i)
	{
		DListPushBack(dst_list, (void*)i);
	}

	from = DListBegin(src_list);
	to = DListEnd(src_list);
	where = DListBegin(dst_list);

	DListSplice(from, to, where);

	status = CheckListIsEmpty(src_list, result) &&
			 CheckList(dst_list, expected_dst, expected_dst_size, result);

	DListDestroy(src_list);
	DListDestroy(dst_list);

	return status;
}

static int TestDListSpilceMiddle(test_result_t* result)
{
	dlist_t* src_list = DListCreate();
	dlist_t* dst_list = DListCreate();
	dlist_iter_t from = {0};
	dlist_iter_t to = {0};
	dlist_iter_t where = {0};
	void* expected_src[] = {(void*)0, (void*)3};
	void* expected_dst[] = {(void*)1, (void*)2, (void*)4, (void*)5, (void*)6, (void*)7};
	size_t expected_src_size = 2;
	size_t expected_dst_size = 6;
	size_t i = 0;
	int status = PASS;

	for (i = 0; i < 4; ++i)
	{
		DListPushBack(src_list, (void*)i);
	}
	for (; i < 8; ++i)
	{
		DListPushBack(dst_list, (void*)i);
	}

	from = DListGetNext(DListBegin(src_list));
	to = DListGetPrev(DListEnd(src_list));
	where = DListBegin(dst_list);

	DListSplice(from, to, where);

	status = CheckList(src_list, expected_src, expected_src_size, result) &&
			 CheckList(dst_list, expected_dst, expected_dst_size, result);

	DListDestroy(src_list);
	DListDestroy(dst_list);

	return status;
}

static int TestDListSpliceNothing(test_result_t* result)
{
	dlist_t* src_list = DListCreate();
	dlist_t* dst_list = DListCreate();
	dlist_iter_t from = {0};
	dlist_iter_t to = {0};
	dlist_iter_t where = {0};
	void* expected_src[] = {(void*)0, (void*)1, (void*)2, (void*)3};
	void* expected_dst[] = {(void*)4, (void*)5, (void*)6, (void*)7};
	size_t expected_src_size = 4;
	size_t expected_dst_size = 4;
	size_t i = 0;
	int status = PASS;

	for (i = 0; i < 4; ++i)
	{
		DListPushBack(src_list, (void*)i);
	}
	for (; i < 8; ++i)
	{
		DListPushBack(dst_list, (void*)i);
	}

	from = DListBegin(src_list);
	to = DListBegin(src_list);
	where = DListBegin(dst_list);

	DListSplice(from, to, where);

	status = CheckList(src_list, expected_src, expected_src_size, result) &&
			 CheckList(dst_list, expected_dst, expected_dst_size, result);

	DListDestroy(src_list);
	DListDestroy(dst_list);

	return status;
}

static void RunTestDListSplice()
{
	test_suite_t suite = {0};

	printf(COLOR_BLUE "Run tests for DListSplice\n" COLOR_RESET);

	RUN_TEST(suite, TestDListSpliceFromBegin);
	RUN_TEST(suite, TestDListSpliceToEnd);
	RUN_TEST(suite, TestDListSpilceFromBeginToEnd);
	RUN_TEST(suite, TestDListSpilceMiddle);
	RUN_TEST(suite, TestDListSpliceNothing);

	PrintTestSummary(&suite);
	printf(COLOR_BLUE "Finished tests\n");
}

static int is_match(const void* data, const void* param)
{
	return data == param;
}

static int TestDListFindBegin(test_result_t* result)
{
	dlist_t* list = DListCreate();
	size_t i = 0;
	dlist_iter_t iter = {0};
	void* data = (void*)0;
	int status = PASS;

	for (i = 0; i < 4; ++i)
	{
		DListPushBack(list, (void*)i);
	}

	iter = DListFind(DListBegin(list), DListEnd(list), data, is_match);

	status = CheckValues(DListGetData(iter), data, result);

	DListDestroy(list);

	return status;
}

static int TestDListFindMiddle(test_result_t* result)
{
	dlist_t* list = DListCreate();
	size_t i = 0;
	dlist_iter_t iter = {0};
	void* data = (void*)2;
	int status = PASS;

	for (i = 0; i < 4; ++i)
	{
		DListPushBack(list, (void*)i);
	}

	iter = DListFind(DListBegin(list), DListEnd(list), data, is_match);

	status = CheckValues(DListGetData(iter), data, result);

	DListDestroy(list);

	return status;
}

static int TestDListFindEnd(test_result_t* result)
{
	dlist_t* list = DListCreate();
	size_t i = 0;
	dlist_iter_t iter = {0};
	void* data = (void*)3;
	int status = PASS;

	for (i = 0; i < 4; ++i)
	{
		DListPushBack(list, (void*)i);
	}

	iter = DListFind(DListBegin(list), DListEnd(list), data, is_match);

	status = CheckValues(DListGetData(iter), data, result);

	DListDestroy(list);

	return status;
}

static int TestDListFindFailed(test_result_t* result)
{
	dlist_t* list = DListCreate();
	size_t i = 0;
	dlist_iter_t iter = {0};
	void* data = (void*)4;
	int status = PASS;

	for (i = 0; i < 4; ++i)
	{
		DListPushBack(list, (void*)i);
	}

	iter = DListFind(DListBegin(list), DListEnd(list), data, is_match);

	status = CheckIters(iter, DListEnd(list), result);

	DListDestroy(list);

	return status;
}

static int TestDListFindAfterRemove(test_result_t* result)
{
	dlist_t* list = DListCreate();
	size_t i = 0;
	dlist_iter_t iter = {0};
	void* data = (void*)3;
	int status = PASS;

	for (i = 0; i < 4; ++i)
	{
		DListPushBack(list, (void*)i);
	}
	DListRemove(DListGetPrev(DListEnd(list)));

	iter = DListFind(DListBegin(list), DListEnd(list), data, is_match);

	status = CheckIters(iter, DListEnd(list), result);

	DListDestroy(list);

	return status;
}

static void RunTestFind()
{
	test_suite_t suite = {0};

	printf(COLOR_BLUE "Run tests for DListFind\n" COLOR_RESET);

	RUN_TEST(suite, TestDListFindBegin);
	RUN_TEST(suite, TestDListFindMiddle);
	RUN_TEST(suite, TestDListFindEnd);
	RUN_TEST(suite, TestDListFindFailed);
	RUN_TEST(suite, TestDListFindAfterRemove);

	PrintTestSummary(&suite);
	printf(COLOR_BLUE "Finished tests\n");
}

static int TestDListMultiFindNothing(test_result_t* result)
{
	dlist_t* list = DListCreate();
	dlist_t* out_list = DListCreate();
	size_t i = 0;
	void* data = (void*)5;
	int status = PASS;

	for (i = 0; i < 4; ++i)
	{
		DListPushBack(list, (void*)i);
	}

	DListMultiFind(DListBegin(list), DListEnd(list), data, is_match, out_list);

	status = CheckListIsEmpty(out_list, result);

	DListDestroy(list);
	DListDestroy(out_list);

	return status;
}

static int TestDListMultiFindSome(test_result_t* result)
{
	dlist_t* list = DListCreate();
	dlist_t* out_list = DListCreate();
	size_t i = 0;
	void* data = (void*)1;
	void* expected_list[] = {(void*)1, (void*)1};
	size_t expected_size = 2;
	int status = PASS;

	for (i = 0; i < 4; ++i)
	{
		DListPushBack(list, (void*)(i/2));
	}

	DListMultiFind(DListBegin(list), DListEnd(list), data, is_match, out_list);

	status = CheckList(out_list, expected_list, expected_size, result);

	DListDestroy(list);
	DListDestroy(out_list);

	return status;
}

static int TestDListMultiFindAll(test_result_t* result)
{
	dlist_t* list = DListCreate();
	dlist_t* out_list = DListCreate();
	size_t i = 0;
	void* data = (void*)0;
	void* expected_list[] = {(void*)0, (void*)0, (void*)0, (void*)0};
	size_t expected_size = 4;
	int status = PASS;

	for (i = 0; i < 4; ++i)
	{
		DListPushBack(list, (void*)0);
	}

	DListMultiFind(DListBegin(list), DListEnd(list), data, is_match, out_list);

	status = CheckList(out_list, expected_list, expected_size, result);

	DListDestroy(list);
	DListDestroy(out_list);

	return status;
}

static int TestDListMultiFindFromEmptyList(test_result_t* result)
{
	dlist_t* list = DListCreate();
	dlist_t* out_list = DListCreate();
	void* data = (void*)1;
	int status = PASS;

	DListMultiFind(DListBegin(list), DListEnd(list), data, is_match, out_list);

	status = CheckListIsEmpty(list, result) &&
			 CheckListIsEmpty(out_list, result);

	DListDestroy(list);
	DListDestroy(out_list);

	return status;
}

static int TestDListMultiFindToNonEmptyList(test_result_t* result)
{
	dlist_t* list = DListCreate();
	dlist_t* out_list = DListCreate();
	size_t i = 0;
	void* data = (void*)0;
	void* expected_list[] = {(void*)0, (void*)0, (void*)0, (void*)0, (void*)0};
	size_t expected_size = 5;
	int status = PASS;

	for (i = 0; i < 4; ++i)
	{
		DListPushBack(list, (void*)0);
	}
	DListPushBack(out_list, (void*)0);

	DListMultiFind(DListBegin(list), DListEnd(list), data, is_match, out_list);

	status = CheckList(out_list, expected_list, expected_size, result);

	DListDestroy(list);
	DListDestroy(out_list);

	return status;
}

static void RunTestMultiFind()
{
	test_suite_t suite = {0};

	printf(COLOR_BLUE "Run tests for DListFind\n" COLOR_RESET);

	RUN_TEST(suite, TestDListMultiFindNothing);
	RUN_TEST(suite, TestDListMultiFindSome);
	RUN_TEST(suite, TestDListMultiFindAll);
	RUN_TEST(suite, TestDListMultiFindFromEmptyList);
	RUN_TEST(suite, TestDListMultiFindToNonEmptyList);

	PrintTestSummary(&suite);
	printf(COLOR_BLUE "Finished tests\n");
}

static void RunTests()
{
	RunTestDListCreation();
	RunTestDListInsert();
	RunTestDListRemove();
	RunTestDListPushAndPop();
	RunTestDListSplice();
	RunTestFind();
	RunTestMultiFind();
}

int main()
{
	RunTests();

	return 0;
}

