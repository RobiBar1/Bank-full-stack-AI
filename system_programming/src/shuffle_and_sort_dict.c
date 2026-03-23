/**
 Writer:  Robi
 Checker: Omer
 Date:    21/03/2026
 **/
#define _POSIX_C_SOURCE 199309L

#include <assert.h>  /* assert */
#include <pthread.h> /* pthread_t */
#include <stdio.h>   /* BUFSIZ */
#include <stdlib.h>  /* srand, rand */
#include <string.h>  /* strcspn, strcmp */
#include <time.h>    /* clock_gettime */

/*#include "shuffle_and_sort_dict.h" ->
no need here, I did all local for the checker will check it either if want
to check it(part of task in brainfuel)*/

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define RESET "\033[0m"

#define BILION 1000000000.0
#define NUM_THREADS 8
#define NUM_OF_DICTS 3

#define UNUSED(x) (void)(x)

#define SUCCESS 0
#define ALLOC_FAIL 1

typedef struct thread_data
{
    void (*sort_func)(unsigned char**, size_t);
    unsigned char** arr;
    size_t size;
} thread_data_t;

static int StrRandCmp(const void* left, const void* right)
{
    static size_t time_active_counter = 0;
    int num = 0;
    UNUSED(left);
    UNUSED(right);

    time_active_counter =
        10000 == time_active_counter ? 0 : time_active_counter;

    if (0 == time_active_counter)
    {
        srand((unsigned int)time(NULL));
    }

    ++time_active_counter;
    num = rand() % 10;
    switch (num)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        num = -1;
        break;

    case 6:
    case 7:
        num = 0;
        break;

    case 8:
    case 9:
    case 0:
        num = 1;
        break;
    }

    return num;
}
static int StrCmpWraper(const void* left, const void* right)
{
    assert(NULL != left);
    assert(NULL != right);

    return strcmp(*(char**)left, *(char**)right);
}
static void QuickSortWraper(unsigned char** str_arr, size_t size)
{
    assert(NULL != str_arr);
    assert(NULL != *str_arr);

    qsort(str_arr, size, sizeof(unsigned char*), StrCmpWraper);
}
static void* ThreadSorting(void* data)
{
    thread_data_t* td = (thread_data_t*)data;

    assert(NULL != td);

    td->sort_func(td->arr, td->size);

    return NULL;
}
static unsigned char** InitDict(size_t* total_word_num)
{
    unsigned char* current_word = NULL;
    unsigned char* memory_pool = NULL;
    unsigned char** dict = NULL;
    FILE* dict_file = NULL;
    char buffer[BUFSIZ] = {'\0'};
    size_t total_chars = 0;
    size_t word_length = 0;
    size_t word_count = 0;
    size_t i = 0;
    size_t j = 0;

    assert(NULL != total_word_num);

    dict_file = fopen("/usr/share/dict/words", "r");
    if (NULL == dict_file)
    {
        return NULL;
    }

    while (fgets(buffer, BUFSIZ, dict_file))
    {
        word_length = strcspn(buffer, "\n") + 1;
        total_chars += word_length;
        ++word_count;
    }

    *total_word_num = word_count * NUM_OF_DICTS;
    dict = (unsigned char**)malloc(*total_word_num * sizeof(unsigned char*));
    if (NULL == dict)
    {
        fclose(dict_file);
        dict_file = NULL;

        return NULL;
    }

    memory_pool = (unsigned char*)malloc(total_chars * NUM_OF_DICTS *
                                         sizeof(unsigned char));
    if (NULL == memory_pool)
    {
        free(dict);
        dict = NULL;
        fclose(dict_file);
        dict_file = NULL;

        return NULL;
    }

    current_word = memory_pool;
    for (; i < NUM_OF_DICTS; ++i)
    {
        fseek(dict_file, 0, SEEK_SET);
        while (fgets(buffer, BUFSIZ, dict_file))
        {
            word_length = strcspn(buffer, "\n") + 1;
            buffer[word_length - 1] = '\0';

            strncpy((char*)current_word, buffer, word_length);
            dict[j++] = current_word;
            current_word += word_length;
        }
    }

    fclose(dict_file);
    dict_file = NULL;

    return dict;
}
static unsigned char** AllocateAndCopy(unsigned char** arr, size_t size)
{
    unsigned char** copied_arr = NULL;
    size_t i = 0;

    assert(NULL != arr);
    assert(NULL != *arr);

    copied_arr = (unsigned char**)malloc(sizeof(unsigned char*) * size);
    if (NULL == copied_arr)
    {
        return NULL;
    }

    for (; i < size; ++i)
    {
        copied_arr[i] = arr[i];
    }

    return copied_arr;
}

static int InitMerge(size_t left_len, size_t right_len, size_t mid,
                     unsigned char** arr, unsigned char*** arr_l,
                     unsigned char*** arr_r)
{
    assert(NULL != arr);
    assert(NULL != *arr);
    assert(NULL != arr_l);
    assert(NULL != arr_r);

    *arr_l = AllocateAndCopy(arr, left_len);
    if (NULL == *arr_l)
    {
        return ALLOC_FAIL;
    }

    *arr_r = AllocateAndCopy(arr + mid + 1, right_len);
    if (NULL == *arr_r)
    {
        free(*arr_l);
        arr_l = NULL;

        return ALLOC_FAIL;
    }

    return SUCCESS;
}

static void SortLeftRight(size_t left_len, size_t right_len,
                          unsigned char** arr_l, unsigned char** arr_r,
                          unsigned char** arr)
{
    size_t i = 0;
    size_t j = 0;
    size_t k = 0;

    assert(NULL != arr);
    assert(NULL != arr_r);
    assert(NULL != *arr_r);
    assert(NULL != arr_l);
    assert(NULL != *arr_l);

    while (i < left_len && j < right_len)
    {
        arr[k++] = 0 >= strcmp((char*)arr_l[i], (char*)arr_r[j]) ? arr_l[i++]
                                                                 : arr_r[j++];
    }

    while (i < left_len)
    {
        arr[k++] = arr_l[i++];
    }
    while (j < right_len)
    {
        arr[k++] = arr_r[j++];
    }
}
static int Merge(unsigned char** arr, size_t mid, size_t right)
{
    unsigned char** arr_l = NULL;
    unsigned char** arr_r = NULL;
    size_t right_len = right - mid;
    size_t left_len = mid + 1;

    assert(NULL != arr);
    assert(NULL != *arr);

    InitMerge(left_len, right_len, mid, arr, &arr_l, &arr_r);
    SortLeftRight(left_len, right_len, arr_l, arr_r, arr);

    free(arr_l);
    arr_l = NULL;
    free(arr_r);
    arr_r = NULL;

    return 0;
}
static int DictSort(unsigned char** words, size_t amount_words)
{
    thread_data_t threads_data[NUM_THREADS] = {0};
    pthread_t threads[NUM_THREADS] = {0};
    unsigned char** words_start_ptr = words;
    size_t prev_finish = 0;
    size_t i = 0;
    size_t chunk_size = amount_words / NUM_THREADS;
    size_t left_after_divide = amount_words % NUM_THREADS;

    assert(NULL != words);
    assert(NULL != *words);

    for (; i < NUM_THREADS; ++i)
    {
        threads_data[i].size = chunk_size + (i < left_after_divide);
        threads_data[i].arr = words_start_ptr;
        threads_data[i].sort_func = QuickSortWraper;
        if (SUCCESS !=
            pthread_create(&threads[i], NULL, ThreadSorting, &threads_data[i]))
        {
            prev_finish = i;
            for (i = 0; i < prev_finish; ++i)
            {
                pthread_join(threads[i], NULL);
            }

            return ALLOC_FAIL;
        }

        words_start_ptr += threads_data[i].size;
    }

    for (i = 0; i < NUM_THREADS; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    prev_finish = threads_data[0].size - 1;

    for (i = 1; i < NUM_THREADS; ++i)
    {
        if (Merge(words, prev_finish, prev_finish + threads_data[i].size))
        {
            return ALLOC_FAIL;
        }

        prev_finish += threads_data[i].size;
    }

    return 0;
}
static void CheckIfSorted(const unsigned char** dict, size_t num_words)
{
    size_t i = 0;

    assert(NULL != dict);
    assert(NULL != *dict);

    for (i = 1; i < num_words; ++i)
    {
        if (0 < strcmp((char*)dict[i - 1], (char*)dict[i]))
        {
            printf(RED "Error, not sorted results\n" RESET);

            return;
        }
    }

    printf(GREEN "Passed: \ndictionary is sorted\n" RESET);
}
static void FreeStringArr(unsigned char** str_arr)
{
    assert(NULL != str_arr);
    assert(NULL != *str_arr);

    free(str_arr[0]);
    str_arr[0] = NULL;
    free(str_arr);
    str_arr = NULL;
}
static void TestDict(void)
{
    unsigned char** dict = NULL;
    struct timespec start = {0};
    struct timespec end = {0};
    size_t total_word_num = 0;
    double elapsed = 0.0;
    int status = 0;

    dict = InitDict(&total_word_num);
    if (NULL == dict)
    {
        return;
    }

    qsort(dict, total_word_num, sizeof(unsigned char*), StrRandCmp);
    clock_gettime(CLOCK_MONOTONIC, &start);
    status = DictSort(dict, total_word_num);
    if (SUCCESS != status)
    {
        printf(RED "Error sort with status %d\n" RESET, status);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (double)(end.tv_sec - start.tv_sec) +
              (double)(end.tv_nsec - start.tv_nsec) / BILION;
    printf(GREEN "Elapsed time: %.5f seconds\n" RESET, elapsed);

    CheckIfSorted((const unsigned char**)dict, total_word_num);
    FreeStringArr(dict);
}

int main()
{
    TestDict();

    return 0;
}