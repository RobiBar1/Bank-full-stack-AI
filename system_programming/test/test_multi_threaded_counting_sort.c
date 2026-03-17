#define _POSIX_C_SOURCE 199309L
#define BILLION (1000000000.0)

/*
gcc -I include/ -std=c89 -O3 -pedantic -Wall -Wextra -pthread
src/multi_threaded_counting_sort.c test/test_multi_threaded_counting_sort.c


*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "multi_threaded_counting_sort.h"

#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_RESET "\033[0m"
#define PASS (1)
#define FAIL (0)
#define SIZE (5 * 1024)
#define SUCCESS (0)
#define NUM_OF_DICTS (5)
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

static void PrintTestSummary(test_suite_t *suite_ptr) {
  if (suite_ptr->passed == suite_ptr->total) {
    printf(COLOR_GREEN "PASS: ");
  } else {
    printf(COLOR_RED "FAIL: ");
  }
  printf("%lu/%lu passed\n" COLOR_RESET, suite_ptr->passed, suite_ptr->total);
}

static int CompareArrays(const unsigned char *res,
                         const unsigned char *expected, size_t size,
                         test_result_t *result) {
  size_t i = 0;

  for (i = 0; i < size; ++i) {
    if (res[i] != expected[i]) {
      sprintf(result->message,
              COLOR_RED
              "Wrong value at index %lu! Got %d, expected %d" COLOR_RESET,
              i, res[i], expected[i]);
      return FAIL;
    }
  }

  return PASS;
}

static int CompareUnsignedCharsAsc(const void *one, const void *other) {
  const unsigned char *char_one = (const unsigned char *)one;
  const unsigned char *char_other = (const unsigned char *)other;

  if (*char_one > *char_other) {
    return 1;
  }
  if (*char_one < *char_other) {
    return -1;
  }

  return 0;
}

unsigned char *InitArray(size_t *size) {
  unsigned char *array = NULL;
  FILE *words = NULL;
  int c = '\0';
  size_t i = 0;
  size_t j = 0;

  words = fopen("/usr/share/dict/words", "r");

  while (EOF != (c = fgetc(words))) {
    ++i;
  }

  i *= NUM_OF_DICTS;

  *size = i;
  array = (unsigned char *)malloc(i * sizeof(unsigned char));

  for (j = 0; j < NUM_OF_DICTS; ++j) {
    fseek(words, 0, SEEK_SET);
    while (EOF != (c = fgetc(words))) {
      array[--i] = (unsigned char)c;
    }
  }

  fclose(words);

  return array;
}

static int TestCountingSort(test_result_t *result) {
  unsigned char *array = NULL;
  unsigned char *expected = NULL;
  size_t size = 0;
  int status = PASS;

  array = InitArray(&size);

  expected = (unsigned char *)malloc(size * sizeof(unsigned char));
  memcpy(expected, array, size);

  CountingSort(array, size);
  qsort(expected, size, sizeof(unsigned char), CompareUnsignedCharsAsc);

  status = CompareArrays(array, expected, size, result);

  free(array);
  free(expected);

  return status;
}

static void RunTestCountingSort() {
  test_suite_t suite = {0};

  printf(COLOR_BLUE "Run test for counting sort\n" COLOR_RESET);

  RUN_TEST(suite, TestCountingSort);

  PrintTestSummary(&suite);
  printf(COLOR_BLUE "Finished tests\n" COLOR_RESET);
}

static void RunCountingSortingBenchmark() {
  unsigned char *array = NULL;
  size_t size = 0;
  struct timespec start = {0};
  struct timespec end = {0};
  double elapsed = 0.0;

  array = InitArray(&size);

  clock_gettime(CLOCK_MONOTONIC, &start);
  CountingSort(array, size);
  clock_gettime(CLOCK_MONOTONIC, &end);

  elapsed =
      (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / BILLION;
  printf("Time taken: %f\n", elapsed);

  free(array);
}

static void RunBenchmark() {
  printf("Running benchmark\n");
  printf("CountingSort\n");
  RunCountingSortingBenchmark(CountingSort);
}

static void RunTests() {
  RunTestCountingSort();
  RunBenchmark();
}

int main() {
  RunTests();

  return 0;
}
