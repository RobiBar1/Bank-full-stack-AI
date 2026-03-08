#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define ARR_SIZE 5

void Swap(int *left, int *right) {
  int tmp = 0;

  assert(NULL != left);
  assert(NULL != right);

  tmp = *left;
  *left = *right;
  *right = tmp;
}

/*[3, 6, 7, 8, 1, 4, 0, 1, 9, 2, 5]*/
void BubbleSort(int *arr, size_t size) {
  int is_swap = 0;
  size_t i = 0;
  size_t j = 0;

  if (size < 2) {
    return;
  }

  assert(NULL != arr);

  for (; i < size; ++i) {
    is_swap = 0;
    for (j = 0; j < size - 1 - i; ++j) {
      if (arr[j + 1] < arr[j]) {
        Swap(&(arr[j]), &(arr[j + 1]));
        is_swap = 1;
      }
    }

    if (!is_swap) {
      return;
    }
  }
}

void PrintArr(int *arr) {
  size_t i = 0;

  for (; i < ARR_SIZE; ++i) {
    printf("%d ", arr[i]);
  }

  printf("\n");
}

int main() {
  size_t i = 0;
  int *arr = (int *)malloc(sizeof(int) * ARR_SIZE);
  if (NULL == arr) {
    return 1;
  }

  for (; i < ARR_SIZE; ++i) {
    arr[i] = ARR_SIZE * 2 - i;
  }

  PrintArr(arr);
  BubbleSort(arr, ARR_SIZE);
  PrintArr(arr);

  return 0;
}
