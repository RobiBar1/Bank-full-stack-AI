#include <assert.h> /* assert */
#include <stdio.h>  /* printf */

#include "pqueue.h"

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"

#define APPLY_TEST(func_call) CheckTest(func_call, #func_call)

typedef enum { TEST_STATUS_SUCCESS, TEST_STATUS_FAILURE } test_status_t;

/* gd test_pqueue.c ../src/pqueue.c ../src/sorted_list.c ../src/dlist.c -I
 * ../include/ */

/* ==== FORWARD DELARATIONS ==== */

static void CheckTest(test_status_t result, char *name);

static test_status_t PQueueCreate_SmokeTest(void);
static test_status_t PQueueEnqueue_BasicTest(void);
static test_status_t SortedLInsertRemove_BasicTest(void);
static test_status_t PQueueDequeue_BasicTest(void);

/* ==== TEST FUNCTION ==== */

static void CheckTest(test_status_t result, char *name) {
  if (result == TEST_STATUS_SUCCESS) {
    printf("-> %s: ", name);
    printf(GREEN "PASSED\n\n" RESET);
  } else {
    printf(RED "-> %s: FAILED\n\n" RESET, name);
  }
}

int main() {

  printf("\n ---- TESTS ----\n\n");

  APPLY_TEST(PQueueCreate_SmokeTest());
  APPLY_TEST(PQueueEnqueue_BasicTest());
  APPLY_TEST(SortedLInsertRemove_BasicTest());
  APPLY_TEST(PQueueDequeue_BasicTest());

  return 0;
}

/* ==== HELPER FUNCTIONS ==== */

int CompareInts(const void *one, const void *two) {
  /*return (*(int*)one - *(int*)two);*/
  return (*(int *)two - *(int *)one);
}

static int IntsAreEqual(const void *num1, const void *num2) {
  return (*(int *)num1 == *(int *)num2);
}

/*
static int AddParamToNum(void* num, void* param)
{
        *(int*)num += *(int*)param;

        return 0;
}
*/

/* ==== TEST FUNCTIONS ==== */

static test_status_t PQueueCreate_SmokeTest(void) {
  pqueue_t *pqueue_list = PQueueCreate(&CompareInts);
  PQueueDestroy(pqueue_list);

  return TEST_STATUS_SUCCESS;
}

static test_status_t PQueueEnqueue_BasicTest(void) {
  test_status_t res = TEST_STATUS_SUCCESS;
  pqueue_t *pqueue_list = PQueueCreate(&CompareInts);

  int a = 6;
  int b = 8;
  int c = 4;
  int d = 5;

  if (0 != PQueueSize(pqueue_list)) {
    res = TEST_STATUS_FAILURE;
  }

  if (1 != PQueueIsEmpty(pqueue_list)) {
    res = TEST_STATUS_FAILURE;
  }

  PQueueEnqueue(pqueue_list, (void *)&a);

  if (a != *(int *)PQueuePeek(pqueue_list)) {
    res = TEST_STATUS_FAILURE;
  }

  if (0 != PQueueIsEmpty(pqueue_list)) {
    res = TEST_STATUS_FAILURE;
  }

  PQueueEnqueue(pqueue_list, (void *)&b);

  if (a != *(int *)PQueuePeek(pqueue_list)) {
    res = TEST_STATUS_FAILURE;
  }

  PQueueEnqueue(pqueue_list, (void *)&c);

  if (c != *(int *)PQueuePeek(pqueue_list)) {
    res = TEST_STATUS_FAILURE;
  }

  PQueueEnqueue(pqueue_list, (void *)&d);

  if (c != *(int *)PQueuePeek(pqueue_list)) {
    res = TEST_STATUS_FAILURE;
  }

  PQueueDestroy(pqueue_list);
  return res;
}

static test_status_t SortedLInsertRemove_BasicTest(void) {
  test_status_t res = TEST_STATUS_SUCCESS;
  pqueue_t *pqueue = PQueueCreate(&CompareInts);

  int a = 6;
  int b = 12;
  int c = 4;

  int rem_val = a;

  if (0 != PQueueSize(pqueue)) {
    res = TEST_STATUS_FAILURE;
  }

  PQueueEnqueue(pqueue, (void *)&a);
  PQueueEnqueue(pqueue, (void *)&b);
  PQueueEnqueue(pqueue, (void *)&c);

  if (c != *(int *)PQueuePeek(pqueue)) {
    printf("fail first\n");
    res = TEST_STATUS_FAILURE;
  }

  rem_val = *(int *)PQueueRemove(pqueue, &rem_val, &IntsAreEqual);

  if (2 != PQueueSize(pqueue)) {
    printf("fail second\n");
    res = TEST_STATUS_FAILURE;
  }

  if (rem_val != a) {
    printf("fail third\n");
    res = TEST_STATUS_FAILURE;
  }

  if (c != *(int *)PQueuePeek(pqueue)) {
    printf("fail four\n");
    res = TEST_STATUS_FAILURE;
  }

  rem_val = c;
  rem_val = *(int *)PQueueRemove(pqueue, &rem_val, &IntsAreEqual);

  if (1 != PQueueSize(pqueue)) {
    printf("fail 5\n");
    res = TEST_STATUS_FAILURE;
  }

  if (rem_val != c) {
    printf("fail 6\n");
    res = TEST_STATUS_FAILURE;
  }

  if (b != *(int *)PQueuePeek(pqueue)) {
    printf("fail 7\n");
    res = TEST_STATUS_FAILURE;
  }

  PQueueDestroy(pqueue);
  return res;
}

static test_status_t PQueueDequeue_BasicTest(void) {
  test_status_t res = TEST_STATUS_SUCCESS;
  pqueue_t *pqueue = PQueueCreate(&CompareInts);

  int a = 6;
  int b = 12;
  int c = 4;
  int d = 5;

  if (0 != PQueueSize(pqueue)) {
    res = TEST_STATUS_FAILURE;
  }

  PQueueEnqueue(pqueue, (void *)&a);
  PQueueEnqueue(pqueue, (void *)&b);
  PQueueEnqueue(pqueue, (void *)&c);

  if (c != *(int *)PQueuePeek(pqueue)) {
    res = TEST_STATUS_FAILURE;
  }

  PQueueDequeue(pqueue);

  if (2 != PQueueSize(pqueue)) {
    res = TEST_STATUS_FAILURE;
  }

  if (a != *(int *)PQueuePeek(pqueue)) {
    res = TEST_STATUS_FAILURE;
  }

  PQueueDequeue(pqueue);

  if (1 != PQueueSize(pqueue)) {
    res = TEST_STATUS_FAILURE;
  }

  if (b != *(int *)PQueuePeek(pqueue)) {
    res = TEST_STATUS_FAILURE;
  }

  PQueueEnqueue(pqueue, (void *)&d);

  if (2 != PQueueSize(pqueue)) {
    res = TEST_STATUS_FAILURE;
  }

  if (d != *(int *)PQueuePeek(pqueue)) {
    res = TEST_STATUS_FAILURE;
  }

  PQueueDestroy(pqueue);
  return res;
}
