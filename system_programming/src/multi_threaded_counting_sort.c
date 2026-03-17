/*
Writer:  Robi
Checker: Chen
Date     17.03.2026
*/

#include <assert.h>  /* assert */
#include <pthread.h> /* pthread_t, pthread_create.. */
#include <stddef.h>  /* size_t, NULL */
#include <stdlib.h>  /* malloc, calloc.. */

#include "multi_threaded_counting_sort.h" /* our api */

#define UNSIGNED_CHAR_RANGE (256)
#define NUM_OF_THREADS                                                         \
    8 /* my sweat spot -> Time taken: 0.019268 on                              \
         benchmark with 100 dict.*/
typedef struct thread_shared_data
{
    const unsigned char* arr;
    size_t size;
    size_t local_count[UNSIGNED_CHAR_RANGE];
} thread_data_t;

void* ThreadCounting(void* arg)
{
    thread_data_t* data = (thread_data_t*)arg;
    size_t temp[UNSIGNED_CHAR_RANGE] = {0};
    size_t i = 0;

    assert(NULL != data);

    for (; i < data->size; ++i)
    {
        ++temp[data->arr[i]];
    }

    for (i = 0; i < UNSIGNED_CHAR_RANGE; ++i)
    {
        data->local_count[i] = temp[i];
    }

    return NULL;
}

static int CreateArrys(pthread_t** threads, thread_data_t** threads_data)
{

    assert(NULL != threads);
    assert(NULL != threads_data);

    *threads = (pthread_t*)malloc(sizeof(pthread_t) * NUM_OF_THREADS);
    if (NULL == *threads)
    {

        return 1;
    }

    *threads_data =
        (thread_data_t*)calloc(NUM_OF_THREADS, sizeof(thread_data_t));
    if (NULL == *threads_data)
    {
        free(*threads);
        *threads = NULL;

        return 1;
    }

    return 0;
}
static void CreateAndInitThreads(unsigned char** arr,
                                 thread_data_t* threads_data,
                                 pthread_t* threads, size_t chunk,
                                 size_t leftovers)
{
    size_t i = 0;

    assert(NULL != arr);
    assert(NULL != *arr);
    assert(NULL != threads_data);
    assert(NULL != threads);

    for (; i < NUM_OF_THREADS; ++i)
    {
        threads_data[i].arr = *arr;
        threads_data[i].size = chunk + (i < leftovers);

        pthread_create(&(threads[i]), NULL, ThreadCounting, &threads_data[i]);
        *arr += threads_data[i].size;
    }
}
static void JoinAndMerge(pthread_t* threads, thread_data_t* threads_data,
                         size_t* global_count)
{
    size_t i = 0;
    size_t j = 0;

    assert(NULL != threads_data);
    assert(NULL != threads);
    assert(NULL != global_count);

    for (; i < NUM_OF_THREADS; ++i)
    {
        pthread_join(threads[i], NULL);
        for (j = 0; j < UNSIGNED_CHAR_RANGE; ++j)
        {
            global_count[j] += threads_data[i].local_count[j];
        }
    }
}

int CountingSort(unsigned char* arr, size_t size)
{
    size_t global_count[UNSIGNED_CHAR_RANGE] = {0};
    pthread_t* threads = NULL;
    thread_data_t* threads_data = NULL;
    size_t i = 0;

    assert(NULL != arr);
    assert(0 < size);

    if (CreateArrys(&threads, &threads_data))
    {
        return 1;
    }

    CreateAndInitThreads(&arr, threads_data, threads, size / NUM_OF_THREADS,
                         size % NUM_OF_THREADS);
    JoinAndMerge(threads, threads_data, global_count);

    arr -= size;
    for (; i < UNSIGNED_CHAR_RANGE; ++i)
    {
        while (0 < global_count[i])
        {
            *arr++ = (unsigned char)i;
            --(global_count[i]);
        }
    }

    free(threads);
    threads = NULL;
    free(threads_data);
    threads_data = NULL;

    return 0;
}