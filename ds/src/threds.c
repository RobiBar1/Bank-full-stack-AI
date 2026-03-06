#include <assert.h>       /* assert */
#include <pthread.h>      /* pthread_create, pthread_t */
#include <stddef.h>       /* size_t */
#include <stdio.h>        /* printf */
#include <stdlib.h>       /* malloc, free */
#include <sys/resource.h> /* struct rlimit */
#include <time.h>         /* time_t */
#include <unistd.h>       /* sleep */

#define ARR_SIZE (1000000)
#define TIME_TO_RUN 10
#define NUM_TO_SUM_DIVISERS 10000000000UL
#define NUM_OF_THREAD 5

#define MIN(x) ((x) < 1 ? 1 : (x))

struct padded_int
{
    int value;
    char padding[60];
};

struct padded_int g_pad_arr[ARR_SIZE];
int g_arr[ARR_SIZE];

void* FillArr(void* arg)
{
    g_arr[(size_t)arg] = (size_t)arg;
    return NULL;
}

void* FillPadArr(void* index)
{
    g_pad_arr[(size_t)index].value = ((size_t)index);
    return NULL;
}

static void* SumOfDivisorsOneThread(void* number)
{
    size_t i = 0;
    size_t sum_divisors = 0;

    for (i = 1; i <= (size_t)number; ++i)
    {
        if (0 == (size_t)number % i)
        {
            sum_divisors += i;
        }
    }

    return (void*)sum_divisors;
}

void* SumOfDivisorsMultiThreads(void* number) /* 1000, 10 -> 1 100, 2 200 */
{
    size_t sum_of_divisors = 0;
    struct rlimit limit = {0};
    size_t thread_limit = 0;
    size_t i = 0;
    size_t end = 0;

    if (0 == getrlimit(RLIMIT_NPROC, &limit))
    {
        thread_limit =
            limit.rlim_cur > NUM_OF_THREAD ? NUM_OF_THREAD : limit.rlim_cur;
    }

    i = ((size_t)number * (NUM_TO_SUM_DIVISERS / thread_limit)) + 1;
    end = (size_t)number == (thread_limit - 1)
              ? NUM_TO_SUM_DIVISERS
              : i + (NUM_TO_SUM_DIVISERS / thread_limit) - 1;
    for (; i <= end; ++i)
    {
        if (0 == NUM_TO_SUM_DIVISERS % i)
        {
            sum_of_divisors += i;
        }
    }

    pthread_exit(((void*)sum_of_divisors));
}

static void Q1();
static void Q2();
static void Q3();
static void Q4();
static void Q5();

int main()
{
    Q5();
    return 0;
}

static void Q5()
{
    size_t i = 0;
    size_t sum_of_all = 0;
    void* return_val = NULL;
    pthread_t* thread = (pthread_t*)malloc(sizeof(pthread_t) * 15);

    if (NULL == thread)
    {
        return;
    }

    pthread_create(&(thread[i]), NULL, SumOfDivisorsOneThread, (void*)i);
    pthread_join(thread[i], &return_val);
    sum_of_all += (size_t)return_val;

    free(thread);
    thread = NULL;
}

static void Q4(size_t thread_num_to_use)
{
    pthread_t* thread = NULL;
    size_t i = 0;
    size_t thread_limit = thread_num_to_use;
    struct rlimit limit = {0};
    void* return_val = NULL;
    size_t sum_of_all = 0;

    if (!thread_num_to_use && 0 == getrlimit(RLIMIT_NPROC, &limit))
    {
        printf("Soft limit: %ld\n", limit.rlim_cur);
        printf("Hard limit: %ld\n", limit.rlim_max);
        /*
        thread_limit = limit.rlim_cur > NUM_TO_SUM_DIVISERS
                           ? NUM_TO_SUM_DIVISERS
                           : limit.rlim_cur;
        */
    }
    else
    {
        perror("getrlimit");
    }

    thread_limit =
        limit.rlim_cur > NUM_OF_THREAD ? NUM_OF_THREAD : limit.rlim_cur;
    thread = (pthread_t*)malloc(sizeof(pthread_t) * thread_limit);
    for (; i < thread_limit; ++i) /*1000, 985*/
    {
        pthread_create(&(thread[i]), NULL, SumOfDivisorsMultiThreads, (void*)i);
    }

    for (i = 0; i < thread_limit; ++i)
    {
        pthread_join(thread[i], &return_val);
        sum_of_all += (size_t)return_val;
    }

    printf("sum of all is: %lu\n", sum_of_all);

    free(thread);
    thread = NULL;
}

static void Q3()
{
    pthread_t* thread = (pthread_t*)malloc(sizeof(pthread_t) * ARR_SIZE);
    size_t i = 0;
    time_t time_start = time(NULL);

    for (; i < ARR_SIZE; ++i)
    {
        while (0 != pthread_create(&(thread[i]), NULL, FillPadArr, (void*)i))
        {
            printf("Fail alocate val in i %lu is: %d\n", i, g_pad_arr[i].value);
        }

        pthread_detach(thread[i]);
    }

    for (i = 0; i < ARR_SIZE; ++i)
    {
        printf("val in i %lu is: %d\n", i, g_pad_arr[i].value);
    }

    free(thread);
    thread = NULL;
}

static void Q2()
{
    pthread_t* thread = (pthread_t*)malloc(sizeof(pthread_t) * ARR_SIZE);
    size_t i = 0;
    time_t time_start = time(NULL);

    for (; i < ARR_SIZE; ++i)
    {
        while (0 != pthread_create(&(thread[i]), NULL, FillArr, (void*)i))
        {
            printf("Fail alocate val in i %lu is: %d\n", i, g_arr[i]);
        }
    }

    for (i = 0; i < ARR_SIZE; ++i)
    {
        printf("val in i %lu is: %d\n", i, g_arr[i]);
    }

    free(thread);
    thread = NULL;
}

static void Q1()
{
    pthread_t* thread = (pthread_t*)malloc(sizeof(pthread_t) * ARR_SIZE);
    size_t i = 0;
    time_t time_start = time(NULL);

    for (; i < ARR_SIZE; ++i)
    {
        pthread_create(&(thread[i]), NULL, FillArr, (void*)i);
    }

    for (i = 0; i < ARR_SIZE; ++i)
    {
        printf("val in i %lu is: %d\n", i, g_arr[i]);
    }

    free(thread);
    thread = NULL;
}