#include <stdio.h>
#include "quick_merge.h"

#define GREEN "\033[0;32m"
#define RED   "\033[0;31m"
#define RESET "\033[0m"

static void TestBinarySerchIter(char* func_name)
{
    int arr[] = {1, 3, 5, 6, 7, 8, 10, 15, 30, 33, 36, 39, 51, 53};
    int arr2[] = {1};
    size_t size = sizeof(arr) / sizeof(int);
    size_t size2 = sizeof(arr2) / sizeof(int);
    int num_to_find = 7;
    int expted_answer = 4;
    int real_answer = 0;

    real_answer = BinarySerchIterative(arr, size, num_to_find);
    if (expted_answer != real_answer)
    {
        printf(RED " Failed on %s test, should return index %d and return %d\n" RESET
                        , func_name, expted_answer, real_answer);
        return;
    }

    num_to_find = 36;
    expted_answer = 10;
    real_answer = BinarySerchIterative(arr, size, num_to_find);
    if (expted_answer != real_answer)
    {
        printf(RED " Failed on %s test, should return index %d and return %d\n" RESET
                        , func_name, expted_answer, real_answer);
        return;
    }

    num_to_find = 12;
    expted_answer = -1;
    real_answer = BinarySerchIterative(arr, size, num_to_find);
    if (expted_answer != real_answer)
    {
        printf(RED " Failed on %s test, should return index %d and return %d\n" RESET
                        , func_name, expted_answer, real_answer);
        return;
    }

    num_to_find = 1;
    expted_answer = 0;
    real_answer = BinarySerchIterative(arr2, size2, num_to_find);
    if (expted_answer != real_answer)
    {
        printf(RED " Failed on %s test, should return index %d and return %d\n" RESET
                        , func_name, expted_answer, real_answer);
        return;
    }

    num_to_find = 0;
    expted_answer = -1;
    real_answer = BinarySerchIterative(arr2, size2, num_to_find);
    if (expted_answer != real_answer)
    {
        printf(RED " Failed on %s test, should return index %d and return %d\n" RESET
                        , func_name, expted_answer, real_answer);
        return;
    }

    printf(GREEN "SUCCESS on test %s\n" RESET, func_name);
}

static void TestBinarySerchRecursive(char* func_name)
{
    int arr[] = {1, 3, 5, 6, 7, 8, 10, 15, 30, 33, 36, 39, 51, 53};
    int arr2[] = {1};
    size_t size = sizeof(arr) / sizeof(int);
    size_t size2 = sizeof(arr2) / sizeof(int);
    int num_to_find = 7;
    int expted_answer = 4;
    int real_answer = 0;

    real_answer = BinarySerchRecursive(arr, size, num_to_find);
    if (expted_answer != real_answer)
    {
        printf(RED " Failed on %s test, should return index %d and return %d\n" RESET
                        , func_name, expted_answer, real_answer);
        return;
    }

    num_to_find = 36;
    expted_answer = 10;
    real_answer = BinarySerchRecursive(arr, size, num_to_find);
    if (expted_answer != real_answer)
    {
        printf(RED " Failed on %s test, should return index %d and return %d\n" RESET
                        , func_name, expted_answer, real_answer);
        return;
    }

    num_to_find = 12;
    expted_answer = -1;
    real_answer = BinarySerchRecursive(arr, size, num_to_find);
    if (expted_answer != real_answer)
    {
        printf(RED " Failed on %s test, should return index %d and return %d\n" RESET
                        , func_name, expted_answer, real_answer);
        return;
    }

    num_to_find = 1;
    expted_answer = 0;
    real_answer = BinarySerchRecursive(arr2, size2, num_to_find);
    if (expted_answer != real_answer)
    {
        printf(RED " Failed on %s test, should return index %d and return %d\n" RESET
                        , func_name, expted_answer, real_answer);
        return;
    }

    num_to_find = 0;
    expted_answer = -1;
    real_answer = BinarySerchRecursive(arr2, size2, num_to_find);
    if (expted_answer != real_answer)
    {
        printf(RED " Failed on %s test, should return index %d and return %d\n" RESET
                        , func_name, expted_answer, real_answer);
        return;
    }

    printf(GREEN "SUCCESS on test %s\n" RESET, func_name);
}

int main() 
{
    char tst1[] = "BinarySerchIterative";
    char tst2[] = "BinarySerchRecursive";

    TestBinarySerchIter(tst1);
    TestBinarySerchRecursive(tst2);

    return 0;
}
