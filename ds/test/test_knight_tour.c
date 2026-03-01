#include <stdio.h>

#include "knight_tour.h"

#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_RESET "\033[0m"
#define PASS (1)
#define FAIL (0)
#define UNUSED(x) ((void)(x))
#define RUN_TEST(suite, test_fn)                                               \
  do {                                                                         \
    test_result_t result = {0};                                                \
    (suite).total++;                                                           \
                                                                               \
    if (test_fn(&result) == PASS) {                                            \
      (suite).passed++;                                                        \
    } else {                                                                   \
      printf(COLOR_RED "[FAIL] %s: %s\n" COLOR_RESET, #test_fn,                \
             result.message);                                                  \
    }                                                                          \
  } while (0)

typedef struct {
  size_t total;
  size_t passed;
} test_suite_t;

typedef struct {
  char message[256];
} test_result_t;

const char *dhcp_status_str[] = {"SUCCESS", "NOT_FOUND", "TIMEOUT"};

static void PrintTestSummary(test_suite_t *suite_ptr) {
  if (suite_ptr->passed == suite_ptr->total) {
    printf(COLOR_GREEN "PASS: ");
  } else {
    printf(COLOR_RED "FAIL: ");
  }
  printf("%lu/%lu passed\n" COLOR_RESET, suite_ptr->passed, suite_ptr->total);
}

static int CheckStatus(knight_status_t res_status,
                       knight_status_t expected_status, test_result_t *result) {
  if (res_status != expected_status) {
    sprintf(result->message, COLOR_RED "res is different! Got %s, expected %s",
            dhcp_status_str[res_status], dhcp_status_str[expected_status]);
    return FAIL;
  }

  return PASS;
}

static int TestKnightTour(test_result_t *result) {
  unsigned char start = 0;
  unsigned char result_path[BOARD_SIZE] = {0};
  knight_status_t res_status = SUCCESS;
  knight_status_t expected_status = SUCCESS;
  int status = PASS;

  res_status = KnightTour(start, result_path, 10, 0);

  status = CheckStatus(res_status, expected_status, result);

  return status;
}

static int TestKnightTourTimeout(test_result_t *result) {
  unsigned char start = 1;
  unsigned char result_path[BOARD_SIZE] = {0};
  knight_status_t res_status = SUCCESS;
  knight_status_t expected_status = TIMEOUT;
  int status = PASS;

  res_status = KnightTour(start, result_path, 10, 0);

  status = CheckStatus(res_status, expected_status, result);

  return status;
}

static int TestKnightTourHeuristic(test_result_t *result) {
  unsigned char start = 1;
  unsigned char result_path[BOARD_SIZE] = {0};
  knight_status_t res_status = SUCCESS;
  knight_status_t expected_status = SUCCESS;
  int status = PASS;

  res_status = KnightTour(start, result_path, 10, 1);

  status = CheckStatus(res_status, expected_status, result);

  return status;
}

static void RunTestKnightTour() {
  test_suite_t suite = {0};

  printf(COLOR_BLUE "Run test for KnightTour\n" COLOR_RESET);

  RUN_TEST(suite, TestKnightTour);
  RUN_TEST(suite, TestKnightTourTimeout);
  RUN_TEST(suite, TestKnightTourHeuristic);

  PrintTestSummary(&suite);
  printf(COLOR_BLUE "Finished tests\n" COLOR_RESET);
}

static void RunTests() { RunTestKnightTour(); }

int main() {
  RunTests();

  return 0;
}
