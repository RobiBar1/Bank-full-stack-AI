/*
psodo Q1:
    producers:
        while(num_of_msgeges)
        {
            msg = CreateMsg();
            UpdateShareMsg(msg);
            --num_of_msgeges;
            SleepUntilConsumerRead();
        }
    consumer:
        while(num_of_msgeges)
        {
            msg = WaitToMsgFromProducers();
            print(msg);
        }
*/

#include <assert.h>    /* assert */
#include <errno.h>     /* errno, EINTR */
#include <pthread.h>   /* pthreads, mutex */
#include <sched.h>     /* sched_yield */
#include <semaphore.h> /* sem_t, sem_init, sem_wait, sem_post */
#include <stdio.h>     /* printf, fprintf, perror */
#include <stdlib.h>    /* malloc, free, exit */
#include <string.h>    /* memset */

#include "d_link_list.h" /* ex2 dlist_t */

/* colors */
#define RESET "\x1B[0m"
#define BLUE "\x1B[34m"
#define YELLOW "\x1B[33m"
#define GREEN "\x1B[32m"
#define RED "\033[1;31m"

#define EX1_MESSAGE_COUNT 7
#define SUCCESS 0

typedef struct spin_lock
{
    volatile int flag;
} spin_lock_t;

typedef struct exercise1_shared
{
    spin_lock_t lock;
    int has_message;
    int message;
    int total_messages;
} exercise1_shared_t;

typedef struct exercise1_thread_arg
{
    exercise1_shared_t* shared;
    int thread_id;
} exercise1_thread_arg_t;

static void CheckSysCalls(int status, const char* text)
{
    assert(NULL != text);

    if (0 != status)
    {
        printf("%s failed with status: %d\n", text, status);
        exit(status);
    }
}

static void SpinInit(spin_lock_t* lock)
{
    assert(NULL != lock);

    lock->flag = 0;

    return;
}

static void SpinLock(spin_lock_t* lock)
{
    assert(NULL != lock);

    while (1)
    {
        if (SUCCESS == __atomic_test_and_set(&(lock->flag), __ATOMIC_ACQUIRE))
        {
            return;
        }

        while (0 != lock->flag)
        {
            CheckSysCalls(sched_yield(), "sched_yield on SpinLock");
        }
    }
}

static void SpinUnlock(spin_lock_t* lock)
{
    assert(NULL != lock);

    __atomic_store_n(&(lock->flag), 0, __ATOMIC_RELEASE);
}

static void* Exercise1Producer(void* arg)
{
    exercise1_thread_arg_t* thread_arg = NULL;
    int index = 0;
    int value = 0;
    int placed = 0;

    assert(NULL != arg);
    assert(NULL != ((exercise1_thread_arg_t*)arg)->shared);

    thread_arg = (exercise1_thread_arg_t*)arg;
    for (; index < thread_arg->shared->total_messages; ++index)
    {
        value = 777 + index;
        placed = 0;
        while (0 == placed)
        {
            SpinLock(&(thread_arg->shared->lock));
            if (0 == thread_arg->shared->has_message)
            {
                thread_arg->shared->message = value;
                thread_arg->shared->has_message = 1;
                placed = 1;
            }

            SpinUnlock(&(thread_arg->shared->lock));
            if (0 == placed)
            {
                CheckSysCalls(sched_yield(),
                              "sched_yield on Exercise1Producer");
            }
        }

        printf("Exercise1 producer -> %d\n", value);
    }

    return NULL;
}

static void* Exercise1Consumer(void* arg)
{
    exercise1_thread_arg_t* thread_arg = NULL;
    int consumed_count = 0;
    int value = 0;
    int taken = 0;

    assert(NULL != arg);
    assert(NULL != ((exercise1_thread_arg_t*)arg)->shared);

    thread_arg = (exercise1_thread_arg_t*)arg;
    while (consumed_count < thread_arg->shared->total_messages)
    {
        taken = 0;
        while (0 == taken)
        {
            SpinLock(&(thread_arg->shared->lock));
            if (0 != thread_arg->shared->has_message)
            {
                value = thread_arg->shared->message;
                thread_arg->shared->has_message = 0;
                taken = 1;
            }

            SpinUnlock(&(thread_arg->shared->lock));
            if (0 == taken)
            {
                CheckSysCalls(sched_yield(),
                              "sched_yield on Exercise1Consumer");
            }
        }

        printf("Exercise1 consumer <- %d\n", value);
        ++consumed_count;
    }

    return NULL;
}

static void InitShared1(exercise1_shared_t* shared)
{
    shared->has_message = 0;
    shared->message = 0;
    shared->total_messages = EX1_MESSAGE_COUNT;
}

static void InitThreadArg(exercise1_thread_arg_t* x_arg,
                          exercise1_shared_t* shared)
{
    x_arg->shared = shared;
    x_arg->thread_id = 0;
}

static void EX1(void)
{
    exercise1_thread_arg_t producer_arg = {0};
    exercise1_thread_arg_t consumer_arg = {0};
    exercise1_shared_t shared = {0};
    pthread_t producer_thread = {0};
    pthread_t consumer_thread = {0};

    InitShared1(&shared);
    SpinInit(&shared.lock);

    InitThreadArg(&producer_arg, &shared);
    InitThreadArg(&consumer_arg, &shared);

    CheckSysCalls(pthread_create(&producer_thread, NULL, Exercise1Producer,
                                 &producer_arg),
                  "pthread_create");
    CheckSysCalls(pthread_create(&consumer_thread, NULL, Exercise1Consumer,
                                 &consumer_arg),
                  "pthread_create");
    CheckSysCalls(pthread_join(producer_thread, NULL), "pthread_join");
    CheckSysCalls(pthread_join(consumer_thread, NULL), "pthread_join");
}

/*===========================Start Ex2=======================*/
#define EX2_MESSAGE_COUNT 100
#define EX2_NUM_PRODUCERS 6
#define EX2_NUM_CONSUMERS 3

typedef struct exercise2_shared
{
    dlist_t* list;
    pthread_mutex_t mutex;
    int produced;
    int producers_done;
} exercise2_shared_t;

typedef struct exercise2_thread_arg
{
    exercise2_shared_t* shared;
    int thread_id;
} exercise2_thread_arg_t;

static void* Exercise2Producer(void* arg)
{
    exercise2_thread_arg_t* thread_arg = NULL;
    int value = 0;
    int status = 0;

    assert(NULL != arg);
    assert(NULL != ((exercise2_thread_arg_t*)arg)->shared);

    thread_arg = (exercise2_thread_arg_t*)arg;
    while (1)
    {
        CheckSysCalls(pthread_mutex_lock(&(thread_arg->shared)->mutex),
                      "mutex_lock in Exercise2Producer()");
        if (thread_arg->shared->produced >= EX2_MESSAGE_COUNT)
        {
            ++(thread_arg->shared->producers_done);
            CheckSysCalls(pthread_mutex_unlock(&(thread_arg->shared)->mutex),
                          "mutex_unlock in Exercise2Producer()");

            return NULL;
        }

        value = 777 + thread_arg->shared->produced++;
        status =
            DListPushBack(thread_arg->shared->list, (void*)((size_t)value));
        CheckSysCalls(pthread_mutex_unlock(&(thread_arg->shared)->mutex),
                      "mutex_unlock");
        CheckSysCalls(status, "DListPushBack in Exercise2Producer()");
        printf("Exercise2 producer[%d] -> %d\n", thread_arg->thread_id, value);
    }

    return NULL;
}

static void* Exercise2Consumer(void* arg)
{
    exercise2_thread_arg_t* thread_arg = NULL;
    int value = 0;

    assert(NULL != arg);
    assert(NULL != ((exercise2_thread_arg_t*)arg)->shared);

    thread_arg = (exercise2_thread_arg_t*)arg;
    while (1)
    {
        CheckSysCalls(pthread_mutex_lock(&(thread_arg->shared->mutex)),
                      "mutex_lock in Exercise2Consumer()");
        if (!DListIsEmpty(thread_arg->shared->list))
        {
            value = (int)(size_t)DListPopFront(thread_arg->shared->list);
            CheckSysCalls(pthread_mutex_unlock(&(thread_arg->shared->mutex)),
                          "mutex_unlock in Exercise2Consumer()");
            printf("Exercise2 consumer[%d]: %d\n", thread_arg->thread_id,
                   value);
        }
        else
        {
            CheckSysCalls(pthread_mutex_unlock(&(thread_arg->shared->mutex)),
                          "mutex_unlock in Exercise2Consumer()");
            if (EX2_NUM_PRODUCERS == thread_arg->shared->producers_done)
            {
                return NULL;
            }

            CheckSysCalls(sched_yield(), "sched_yield on Exercise2Consumer");
        }
    }

    return NULL;
}

static void EX2(void)
{
    exercise2_thread_arg_t producer_args[EX2_NUM_PRODUCERS] = {0};
    exercise2_thread_arg_t consumer_args[EX2_NUM_CONSUMERS] = {0};
    pthread_t producers[EX2_NUM_PRODUCERS] = {0};
    pthread_t consumers[EX2_NUM_CONSUMERS] = {0};
    exercise2_shared_t shared = {0};
    size_t i = 0;

    shared.list = DListCreate();
    if (NULL == shared.list)
    {
        printf("DListCreate failed\n");
        exit(1);
    }

    CheckSysCalls(pthread_mutex_init(&shared.mutex, NULL), "mutex_init");
    for (; i < EX2_NUM_PRODUCERS; ++i)
    {
        producer_args[i].shared = &shared;
        producer_args[i].thread_id = i;
        CheckSysCalls(pthread_create(&producers[i], NULL, Exercise2Producer,
                                     &producer_args[i]),
                      "pthread_create");
    }

    for (i = 0; i < EX2_NUM_CONSUMERS; ++i)
    {
        consumer_args[i].shared = &shared;
        consumer_args[i].thread_id = i;
        CheckSysCalls(pthread_create(&consumers[i], NULL, Exercise2Consumer,
                                     &consumer_args[i]),
                      "pthread_create");
    }

    for (i = 0; i < EX2_NUM_PRODUCERS; ++i)
    {
        CheckSysCalls(pthread_join(producers[i], NULL), "pthread_join");
    }

    for (i = 0; i < EX2_NUM_CONSUMERS; ++i)
    {
        CheckSysCalls(pthread_join(consumers[i], NULL), "pthread_join");
    }

    CheckSysCalls(pthread_mutex_destroy(&shared.mutex), "mutex_destroy");
    DListDestroy(shared.list);
    shared.list = NULL;
}

/*======================= End Ex2 ==================*/

/*===========================Start Ex3=======================
* psodo ex3:
    producers:
        while(num_of_msgeges)
        {
            LockMutex
                msg = CreateMsg();
                PusQueue(msg);
                ++num_of_msgeges;
            UNLockMutex
            UpdateSemaPhoreVal(+1)
        }
    consumer:
        while(num_of_msgeges)
        {
            sleep until SemaPhoreUpdate and when wakeup, do:
            LockMutex()
                if Queue not empty do:
                    msg = PopQueue();
                    UNLockMutex()
                    Print(msg);
                else
                    UNLockMutex()
                    return;
        }
*/

#define EX3_MESSAGE_COUNT 100
#define EX3_NUM_PRODUCERS 6
#define EX3_NUM_CONSUMERS 3

typedef struct exercise3_shared
{
    dlist_t* list;
    pthread_mutex_t mutex;
    sem_t sem;
    int produced;
} exercise3_shared_t;

typedef struct exercise3_thread_arg
{
    exercise3_shared_t* shared;
    int thread_id;
} exercise3_thread_arg_t;

static void* Exercise3Producer(void* arg)
{
    exercise3_thread_arg_t* thread_arg = NULL;
    int value = 0;
    int status = 0;

    assert(NULL != arg);
    assert(NULL != ((exercise3_thread_arg_t*)arg)->shared);

    thread_arg = (exercise3_thread_arg_t*)arg;
    while (1)
    {
        CheckSysCalls(pthread_mutex_lock(&(thread_arg->shared->mutex)),
                      "mutex_lock in Exercise3Producer()");
        if (thread_arg->shared->produced >= EX3_MESSAGE_COUNT)
        {
            CheckSysCalls(pthread_mutex_unlock(&(thread_arg->shared->mutex)),
                          "mutex_unlock in Exercise3Producer()");

            return NULL;
        }

        value = 777 + thread_arg->shared->produced++;
        status =
            DListPushBack(thread_arg->shared->list, (void*)((size_t)value));
        CheckSysCalls(pthread_mutex_unlock(&(thread_arg->shared->mutex)),
                      "mutex_unlock in Exercise3Producer()");
        CheckSysCalls(status, "DListPushBack in Exercise3Producer()");
        printf("Exercise3 producer[%d] -> %d\n", thread_arg->thread_id, value);
        CheckSysCalls(sem_post(&(thread_arg->shared->sem)),
                      "sem_post in Exercise3Producer()");
    }

    return NULL;
}

static void* Exercise3Consumer(void* arg)
{
    exercise3_thread_arg_t* thread_arg = NULL;
    int value = 0;

    assert(NULL != arg);
    assert(NULL != ((exercise3_thread_arg_t*)arg)->shared);

    thread_arg = (exercise3_thread_arg_t*)arg;
    while (1)
    {
        CheckSysCalls(sem_wait(&(thread_arg->shared->sem)),
                      "sem_wait in Exercise3Consumer()");
        CheckSysCalls(pthread_mutex_lock(&(thread_arg->shared->mutex)),
                      "mutex_lock in Exercise3Consumer()");
        if (1 == DListIsEmpty(thread_arg->shared->list))
        {
            CheckSysCalls(pthread_mutex_unlock(&(thread_arg->shared->mutex)),
                          "mutex_unlock in Exercise3Consumer()");

            return NULL;
        }

        value = (int)(size_t)DListPopFront(thread_arg->shared->list);
        CheckSysCalls(pthread_mutex_unlock(&(thread_arg->shared->mutex)),
                      "mutex_unlock in Exercise3Consumer()");
        printf("Exercise3 consumer[%d] <- %d\n", thread_arg->thread_id, value);
    }

    return NULL;
}

static void EX3(void)
{
    exercise3_thread_arg_t producer_args[EX3_NUM_PRODUCERS] = {0};
    exercise3_thread_arg_t consumer_args[EX3_NUM_CONSUMERS] = {0};
    pthread_t producers[EX3_NUM_PRODUCERS] = {0};
    pthread_t consumers[EX3_NUM_CONSUMERS] = {0};
    exercise3_shared_t shared = {0};
    size_t i = 0;

    shared.list = DListCreate();
    if (NULL == shared.list)
    {
        printf("DListCreate failed\n");
        exit(1);
    }

    CheckSysCalls(pthread_mutex_init(&shared.mutex, NULL), "mutex_init");
    CheckSysCalls(sem_init(&shared.sem, 0, 0), "sem_init");
    for (; i < EX3_NUM_PRODUCERS; ++i)
    {
        producer_args[i].shared = &shared;
        producer_args[i].thread_id = (int)i;
        CheckSysCalls(pthread_create(&producers[i], NULL, Exercise3Producer,
                                     &producer_args[i]),
                      "pthread_create");
    }

    for (i = 0; i < EX3_NUM_CONSUMERS; ++i)
    {
        consumer_args[i].shared = &shared;
        consumer_args[i].thread_id = (int)i;
        CheckSysCalls(pthread_create(&consumers[i], NULL, Exercise3Consumer,
                                     &consumer_args[i]),
                      "pthread_create");
    }

    for (i = 0; i < EX3_NUM_PRODUCERS; ++i)
    {
        CheckSysCalls(pthread_join(producers[i], NULL), "pthread_join");
    }

    for (i = 0; i < EX3_NUM_CONSUMERS; ++i)
    {
        CheckSysCalls(sem_post(&shared.sem), "sem_post for cleanup");
    }

    for (i = 0; i < EX3_NUM_CONSUMERS; ++i)
    {
        CheckSysCalls(pthread_join(consumers[i], NULL), "pthread_join");
    }

    CheckSysCalls(pthread_mutex_destroy(&shared.mutex), "mutex_destroy");
    CheckSysCalls(sem_destroy(&shared.sem), "sem_destroy");
    DListDestroy(shared.list);
    shared.list = NULL;
}
/*======================= End Ex3 ==================*/

/*===========================Start Ex4=======================*/

/* psodo:
* Producer:
*   while(produced <= EX4_COUNT_MSG) do:
        LockMutex
        loadMsg
        UnLockMutex
        SemWait to empty_slot
        LockMutex
        QueuePushEnd(Msg)
        UnLockMutex
        print?
        SemPost to full_slots
* Consumer:
    loop while head != tail and do:
        SemWait to full_slot
        LockMutex
        val = QueuePopFront()
        UnLockMutex
        print?
        SemPost to empty_slots
*/
#define EX4_MESSAGE_COUNT 100
#define EX4_NUM_PRODUCERS 6
#define EX4_NUM_CONSUMERS 3
#define EX4_QUEUE_CAPACITY 10

typedef struct exercise4_shared
{
    int* queue;
    size_t capacity;
    size_t head;
    size_t tail;
    size_t produced;
    size_t consumed;
    pthread_mutex_t mutex;
    sem_t empty_slots;
    sem_t full_slots;
} exercise4_shared_t;

typedef struct exercise4_thread_arg
{
    exercise4_shared_t* shared;
    int thread_id;
} exercise4_thread_arg_t;

static void* Exercise4Producer(void* arg)
{
    exercise4_thread_arg_t* thread_arg = NULL;
    int value = 0;

    assert(NULL != arg);
    assert(NULL != ((exercise4_thread_arg_t*)arg)->shared);

    thread_arg = (exercise4_thread_arg_t*)arg;
    while (1)
    {
        CheckSysCalls(pthread_mutex_lock(&(thread_arg->shared->mutex)),
                      "mutex_lock in Exercise4Producer()");
        if (EX4_MESSAGE_COUNT <= thread_arg->shared->produced)
        {
            CheckSysCalls(pthread_mutex_unlock(&(thread_arg->shared->mutex)),
                          "mutex_unlock in Exercise4Producer()");

            return NULL;
        }

        value = thread_arg->shared->produced++;
        CheckSysCalls(pthread_mutex_unlock(&(thread_arg->shared->mutex)),
                      "mutex_unlock in Exercise4Producer()");
        value += 777;
        CheckSysCalls(sem_wait(&(thread_arg->shared->empty_slots)),
                      "sem_wait in Exercise4Producer()");
        CheckSysCalls(pthread_mutex_lock(&(thread_arg->shared->mutex)),
                      "mutex_lock in Exercise4Producer()");
        thread_arg->shared->queue[thread_arg->shared->tail] = value;
        thread_arg->shared->tail =
            (thread_arg->shared->tail + 1) % thread_arg->shared->capacity;
        CheckSysCalls(pthread_mutex_unlock(&(thread_arg->shared->mutex)),
                      "mutex_unlock in Exercise4Producer()");
        printf("Exercise4 producer[%d] -> %d\n", thread_arg->thread_id, value);
        CheckSysCalls(sem_post(&(thread_arg->shared->full_slots)),
                      "sem_post in Exercise4Producer()");
    }

    return NULL;
}

static void* Exercise4Consumer(void* arg)
{
    exercise4_thread_arg_t* thread_arg = NULL;
    int value = 0;
    int is_done = 0;

    assert(NULL != arg);
    assert(NULL != ((exercise4_thread_arg_t*)arg)->shared);

    thread_arg = (exercise4_thread_arg_t*)arg;
    while (1)
    {
        CheckSysCalls(sem_wait(&(thread_arg->shared->full_slots)),
                      "sem_wait in Exercise4Consumer()");
        CheckSysCalls(pthread_mutex_lock(&(thread_arg->shared->mutex)),
                      "mutex_lock in Exercise4Consumer()");
        if (EX4_MESSAGE_COUNT <= thread_arg->shared->consumed)
        {
            CheckSysCalls(pthread_mutex_unlock(&(thread_arg->shared->mutex)),
                          "mutex_unlock in Exercise4Consumer()");
            CheckSysCalls(sem_post(&(thread_arg->shared->full_slots)),
                          "sem_post chain in Exercise4Consumer()");

            return NULL;
        }

        value = thread_arg->shared->queue[thread_arg->shared->head];
        thread_arg->shared->head =
            (thread_arg->shared->head + 1) % thread_arg->shared->capacity;
        ++thread_arg->shared->consumed;
        is_done = (EX4_MESSAGE_COUNT <= thread_arg->shared->consumed);
        CheckSysCalls(pthread_mutex_unlock(&(thread_arg->shared->mutex)),
                      "mutex_unlock in Exercise4Consumer()");
        printf("Exercise4 consumer[%d] <- %d\n", thread_arg->thread_id, value);
        CheckSysCalls(sem_post(&(thread_arg->shared->empty_slots)),
                      "sem_post in Exercise4Consumer()");
        if (is_done)
        {
            CheckSysCalls(sem_post(&(thread_arg->shared->full_slots)),
                          "sem_post done in Exercise4Consumer()");

            return NULL;
        }
    }

    return NULL;
}

static void EX4(void)
{
    exercise4_thread_arg_t producer_args[EX4_NUM_PRODUCERS] = {0};
    exercise4_thread_arg_t consumer_args[EX4_NUM_CONSUMERS] = {0};
    pthread_t producers[EX4_NUM_PRODUCERS] = {0};
    pthread_t consumers[EX4_NUM_CONSUMERS] = {0};
    exercise4_shared_t shared = {0};
    size_t i = 0;

    shared.capacity = EX4_QUEUE_CAPACITY;
    shared.queue = (int*)malloc(sizeof(int) * shared.capacity);
    if (NULL == shared.queue)
    {
        printf("malloc failed in EX4\n");
        exit(1);
    }

    shared.produced = 0;
    shared.consumed = 0;

    CheckSysCalls(pthread_mutex_init(&shared.mutex, NULL), "mutex_init");
    CheckSysCalls(sem_init(&shared.empty_slots, 0, shared.capacity),
                  "sem_init");
    CheckSysCalls(sem_init(&shared.full_slots, 0, 0), "sem_init");
    for (; i < EX4_NUM_PRODUCERS; ++i)
    {
        producer_args[i].shared = &shared;
        producer_args[i].thread_id = (int)i;
        CheckSysCalls(pthread_create(&producers[i], NULL, Exercise4Producer,
                                     &producer_args[i]),
                      "pthread_create");
    }

    for (i = 0; i < EX4_NUM_CONSUMERS; ++i)
    {
        consumer_args[i].shared = &shared;
        consumer_args[i].thread_id = (int)i;
        CheckSysCalls(pthread_create(&consumers[i], NULL, Exercise4Consumer,
                                     &consumer_args[i]),
                      "pthread_create");
    }

    for (i = 0; i < EX4_NUM_PRODUCERS; ++i)
    {
        CheckSysCalls(pthread_join(producers[i], NULL), "pthread_join");
    }

    for (i = 0; i < EX4_NUM_CONSUMERS; ++i)
    {
        CheckSysCalls(pthread_join(consumers[i], NULL), "pthread_join");
    }

    CheckSysCalls(pthread_mutex_destroy(&shared.mutex), "mutex_destroy");
    CheckSysCalls(sem_destroy(&shared.empty_slots), "sem_destroy");
    CheckSysCalls(sem_destroy(&shared.full_slots), "sem_destroy");
    free(shared.queue);
    shared.queue = NULL;
}
/*======================= End Ex4 ==================*/

/*===========================Start Ex5=======================*/
/*===========================Start Ex5=======================*/
#define EX5_MESSAGE_COUNT 100
#define EX5_NUM_PRODUCERS 6
#define EX5_NUM_CONSUMERS 3
#define EX5_QUEUE_CAPACITY 10

typedef struct fsq
{
    dlist_t* list;
    pthread_mutex_t read_mutex;
    pthread_mutex_t write_mutex;
    sem_t empty_slots;
    sem_t full_slots;
} fsq_t;

typedef struct exercise5_shared
{
    fsq_t* fsq;
    size_t produced;
    size_t producers_done;
    pthread_mutex_t prod_mutex;
} exercise5_shared_t;

typedef struct exercise5_thread_arg
{
    exercise5_shared_t* shared;
    int thread_id;
} exercise5_thread_arg_t;

static fsq_t* FsqCreate(size_t capacity)
{
    fsq_t* fsq = NULL;

    assert(0 != capacity);

    fsq = (fsq_t*)malloc(sizeof(fsq_t));
    if (NULL == fsq)
    {
        exit(1);
    }

    fsq->list = DListCreate();
    if (NULL == fsq->list)
    {
        free(fsq);
        fsq = NULL;
        exit(1);
    }

    CheckSysCalls(pthread_mutex_init(&(fsq->read_mutex), NULL), "mutex_init");
    CheckSysCalls(pthread_mutex_init(&(fsq->write_mutex), NULL), "mutex_init");
    CheckSysCalls(sem_init(&(fsq->empty_slots), 0, (unsigned int)capacity),
                  "sem_init");
    CheckSysCalls(sem_init(&(fsq->full_slots), 0, 0), "sem_init");

    return fsq;
}

static void FsqDestroy(fsq_t* fsq)
{
    assert(NULL != fsq);

    CheckSysCalls(pthread_mutex_destroy(&(fsq->read_mutex)), "mutex_destroy");
    CheckSysCalls(pthread_mutex_destroy(&(fsq->write_mutex)), "mutex_destroy");
    CheckSysCalls(sem_destroy(&(fsq->empty_slots)), "sem_destroy");
    CheckSysCalls(sem_destroy(&(fsq->full_slots)), "sem_destroy");
    DListDestroy(fsq->list);
    fsq->list = NULL;
    free(fsq);
    fsq = NULL;
}

static void FsqEnqueue(fsq_t* fsq, int data)
{
    int status = 0;

    assert(NULL != fsq);

    CheckSysCalls(sem_wait(&(fsq->empty_slots)), "sem_wait in FsqEnqueue");
    CheckSysCalls(pthread_mutex_lock(&(fsq->write_mutex)),
                  "mutex_lock in FsqEnqueue");
    status = DListPushBack(fsq->list, (void*)((size_t)data));
    CheckSysCalls(pthread_mutex_unlock(&(fsq->write_mutex)),
                  "mutex_unlock in FsqEnqueue");
    CheckSysCalls(status, "DListPushBack in FsqEnqueue");
    CheckSysCalls(sem_post(&(fsq->full_slots)), "sem_post in FsqEnqueue");
}

static int FsqDequeue(fsq_t* fsq)
{
    int data = 0;

    assert(NULL != fsq);

    CheckSysCalls(sem_wait(&(fsq->full_slots)), "sem_wait in FsqDequeue");
    CheckSysCalls(pthread_mutex_lock(&(fsq->read_mutex)),
                  "mutex_lock in FsqDequeue");
    /*CheckSysCalls(pthread_mutex_lock(&(fsq->write_mutex)),
                  "mutex_lock in FsqDequeue");*/
    data = (int)(size_t)DListPopFront(fsq->list);
    /*CheckSysCalls(pthread_mutex_unlock(&(fsq->write_mutex)),
                  "mutex_unlock in FsqDequeue");*/
    CheckSysCalls(pthread_mutex_unlock(&(fsq->read_mutex)),
                  "mutex_unlock in FsqDequeue");
    CheckSysCalls(sem_post(&(fsq->empty_slots)), "sem_post in FsqDequeue");

    return data;
}

static void* Exercise5Producer(void* arg)
{
    exercise5_thread_arg_t* thread_arg = NULL;
    int value = 0;
    int is_last = 0;

    assert(NULL != arg);
    assert(NULL != ((exercise5_thread_arg_t*)arg)->shared);

    thread_arg = (exercise5_thread_arg_t*)arg;
    while (1)
    {
        CheckSysCalls(pthread_mutex_lock(&(thread_arg->shared->prod_mutex)),
                      "mutex_lock in Exercise5Producer");
        if (EX5_MESSAGE_COUNT <= thread_arg->shared->produced)
        {
            ++thread_arg->shared->producers_done;
            is_last = (EX5_NUM_PRODUCERS == thread_arg->shared->producers_done);
            CheckSysCalls(
                pthread_mutex_unlock(&(thread_arg->shared->prod_mutex)),
                "mutex_unlock in Exercise5Producer");
            if (is_last)
            {
                FsqEnqueue(thread_arg->shared->fsq, -1);
            }

            return NULL;
        }

        value = thread_arg->shared->produced++;
        CheckSysCalls(pthread_mutex_unlock(&(thread_arg->shared->prod_mutex)),
                      "mutex_unlock in Exercise5Producer");
        value += 777;
        FsqEnqueue(thread_arg->shared->fsq, value);
        printf("Exercise5 producer[%d] -> %d\n", thread_arg->thread_id, value);
    }

    return NULL;
}

static void* Exercise5Consumer(void* arg)
{
    exercise5_thread_arg_t* thread_arg = NULL;
    int value = 0;

    assert(NULL != arg);
    assert(NULL != ((exercise5_thread_arg_t*)arg)->shared);

    thread_arg = (exercise5_thread_arg_t*)arg;
    while (1)
    {
        if (thread_arg->shared->) /* check here only if last element so check
                                     lock writer mutex either */
            value = FsqDequeue(thread_arg->shared->fsq);
        if (-1 == value)
        {
            FsqEnqueue(thread_arg->shared->fsq, -1);

            return NULL;
        }

        printf("Exercise5 consumer[%d] <- %d\n", thread_arg->thread_id, value);
    }

    return NULL;
}

static void EX5(void)
{
    exercise5_thread_arg_t producer_args[EX5_NUM_PRODUCERS] = {0};
    exercise5_thread_arg_t consumer_args[EX5_NUM_CONSUMERS] = {0};
    pthread_t producers[EX5_NUM_PRODUCERS] = {0};
    pthread_t consumers[EX5_NUM_CONSUMERS] = {0};
    exercise5_shared_t shared = {0};
    size_t i = 0;

    shared.fsq = FsqCreate(EX5_QUEUE_CAPACITY);
    shared.produced = 0;
    shared.producers_done = 0;
    CheckSysCalls(pthread_mutex_init(&(shared.prod_mutex), NULL), "mutex_init");

    for (; i < EX5_NUM_PRODUCERS; ++i)
    {
        producer_args[i].shared = &shared;
        producer_args[i].thread_id = (int)i;
        CheckSysCalls(pthread_create(&producers[i], NULL, Exercise5Producer,
                                     &producer_args[i]),
                      "pthread_create");
    }

    for (i = 0; i < EX5_NUM_CONSUMERS; ++i)
    {
        consumer_args[i].shared = &shared;
        consumer_args[i].thread_id = (int)i;
        CheckSysCalls(pthread_create(&consumers[i], NULL, Exercise5Consumer,
                                     &consumer_args[i]),
                      "pthread_create");
    }

    for (i = 0; i < EX5_NUM_PRODUCERS; ++i)
    {
        CheckSysCalls(pthread_join(producers[i], NULL), "pthread_join");
    }

    for (i = 0; i < EX5_NUM_CONSUMERS; ++i)
    {
        CheckSysCalls(pthread_join(consumers[i], NULL), "pthread_join");
    }

    CheckSysCalls(pthread_mutex_destroy(&(shared.prod_mutex)), "mutex_destroy");
    FsqDestroy(shared.fsq);
    shared.fsq = NULL;
}
/*======================= End Ex5 ==================*/

/*======================= Start Main ==================*/
static void PrintStart(char* func_name)
{
    printf(BLUE "============================ Start "
                "=========================================\n" RESET);
    printf(GREEN "Start running " YELLOW "-->%s<--\n" RESET, func_name);
}

static void PrintEnd(char* func_name)
{
    printf(GREEN "\nEnd running " YELLOW "-->%s<--\n" RESET, func_name);
    printf(RED "============================ End "
               "=========================================\n" RESET);
}

typedef void (*active_func)(void);

static void ActiveEx(active_func func, char* func_name)
{
    PrintStart(func_name);
    func();
    PrintEnd(func_name);
}

int main()
{
    ActiveEx(EX1, "EX1");
    ActiveEx(EX2, "EX2");
    ActiveEx(EX3, "EX3");
    ActiveEx(EX4, "EX4");
    ActiveEx(EX5, "EX5");

    return 0;
}