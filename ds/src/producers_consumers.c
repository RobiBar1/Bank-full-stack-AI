/*
Writer:  Robi
Checker: Alon
Date:    13.03.2026

gd -I include/ src/producers_consumers.c src/d_link_list.c
*/

#include <assert.h>    /* assert */
#include <errno.h>     /* errno, EINTR */
#include <pthread.h>   /* pthreads, mutex */
#include <sched.h>     /* sched_yield */
#include <semaphore.h> /* sem_t, sem_init, sem_wait, sem_post */
#include <signal.h>    /* sig_atomic_t */
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

#define UNUSED(x) (void)(x)

/*================================ Start Ex1 ==============================*/
/*
psodo Q1:
    Producer:
        while messages_to_produce:
            produce value
            spin while has_message == 1:
                yield
            shared_message = value
            has_message = 1
            messages_to_produce -= 1

    Consumer:
        while messages_to_read:
            spin while has_message == 0:
                yield
            value = shared_message
            has_message = 0
            consume value
            messages_to_read -= 1
*/

#define EX1_MESSAGE_COUNT 7
#define SUCCESS 0
#define MARK_AS_HAVE_MESSAGE 1
#define MARK_AS_HAVE_NO_MESSAGE 0

static volatile sig_atomic_t g_ex1_message = 0;
static volatile sig_atomic_t g_ex1_has_message = 0;

static void CheckSysCalls(int status, const char* text)
{
    assert(NULL != text);

    UNUSED(text);
    if (0 != status)
    {
        exit(status);
    }
}

static void* Exercise1Producer(void* arg)
{
    int index = 0;
    int value = 0;

    UNUSED(arg);
    for (; index < EX1_MESSAGE_COUNT; ++index)
    {
        value = 777 + index;
        while (!(__sync_bool_compare_and_swap(&g_ex1_has_message, 0, 0)))
        {
            sched_yield();
        }

        g_ex1_message = value;
        __sync_fetch_and_add(&g_ex1_has_message, 1);
    }

    return NULL;
}

static void* Exercise1Consumer(void* arg)
{
    int consumed_count = 0;
    int value = 0;

    UNUSED(arg);
    for (; EX1_MESSAGE_COUNT > consumed_count; ++consumed_count)
    {
        while (!(__sync_bool_compare_and_swap(&g_ex1_has_message, 1, 0)))
        {
            sched_yield();
        }

        value = g_ex1_message;
        UNUSED(value);
    }

    return NULL;
}

static void EX1(void)
{
    pthread_t producer_thread = {0};
    pthread_t consumer_thread = {0};

    CheckSysCalls(
        pthread_create(&producer_thread, NULL, Exercise1Producer, NULL),
        "pthread_create in EX1");
    CheckSysCalls(
        pthread_create(&consumer_thread, NULL, Exercise1Consumer, NULL),
        "pthread_create in EX1");
    CheckSysCalls(pthread_join(producer_thread, NULL), "pthread_join in EX1");
    CheckSysCalls(pthread_join(consumer_thread, NULL), "pthread_join in EX1");
}

/*===========================Start Ex2=======================*/

/*
Psodo Ex2:
    Producer:
        loop forever:
            lock mutex
            if produced >= MAX:
                producers_done += 1
                local_done = 1
            else
                value = produced;
                produced += 1
            unlock mutex
            if local_done == 1
                return
            val = CalcValue(val)
            lock mutex
            status = PushToList(value)
            unlock mutex
            if status == FAIL
                exit(FAIL)

    Consumer:
        loop forever:
            lock mutex
            if list not empty:
                val = pop value from list
            else:
                val = no_val_flag

            if val != no_val_flag
                do_action(val)
            else
                if all producers done:
                    return
            yield

*/
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
    exercise2_thread_arg_t* thread_arg = (exercise2_thread_arg_t*)arg;
    int value = 0;
    int status = 0;
    int is_done = 0;

    assert(NULL != thread_arg);
    assert(NULL != thread_arg->shared);

    while (1)
    {
        pthread_mutex_lock(&(thread_arg->shared)->mutex);
        if (thread_arg->shared->produced >= EX2_MESSAGE_COUNT)
        {
            ++(thread_arg->shared->producers_done);
            is_done = 1;
        }
        else
        {
            value = thread_arg->shared->produced++;
        }

        pthread_mutex_unlock(&(thread_arg->shared)->mutex);
        if (is_done)
        {
            return NULL;
        }

        value += 777;
        pthread_mutex_lock(&(thread_arg->shared)->mutex);
        status =
            DListPushBack(thread_arg->shared->list, (void*)((size_t)value));
        pthread_mutex_unlock(&(thread_arg->shared)->mutex);
        if (SUCCESS != status)
        {
            exit(status);
        }
    }

    return NULL;
}

static void* Exercise2Consumer(void* arg)
{
    exercise2_thread_arg_t* thread_arg = (exercise2_thread_arg_t*)arg;
    int value = 0;
    int got_value = 0;
    int is_done = 0;

    assert(NULL != thread_arg);

    while (1)
    {
        pthread_mutex_lock(&(thread_arg->shared->mutex));
        if (!DListIsEmpty(thread_arg->shared->list))
        {
            value = (int)(size_t)DListPopFront(thread_arg->shared->list);
            got_value = 1;
        }
        else
        {
            got_value = 0;
            is_done = (EX2_NUM_PRODUCERS <= thread_arg->shared->producers_done);
        }

        pthread_mutex_unlock(&(thread_arg->shared->mutex));
        if (got_value)
        {
            UNUSED(value);
            /*action(value); (print/send/calc/etc..)*/
        }
        else if (is_done)
        {
            return NULL;
        }

        sched_yield();
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
        forever loop and do:
            LockMutex
                if (produced_count >= EX3_MESSAGE_COUNT)
                    UnLockMutex

                    return
            val = produced_count
            produced_count += 1
            UnLockMutex
            msg = CreateMsg(val)

            LockMutex
                PushQueue(msg)
            UnLockMutex

            sem_post(sem)

    consumer:
        forever while
        {
            sem_wait(sem)
            LockMutex()
                val = PopQueue()
            UNLockMutex()
            if (val == EX3_STOP_VALUE) do:
                return
        }

    EX3:
        CreateList
        CreateMutex
        CreateSem
        CreateProducers(EX3_NUM_PRODUCERS)
        CreateConsumers(EX3_NUM_CONSUMERS)
        JoinProducers(EX3_NUM_PRODUCERS)
        for each Consumers do:
            lock mutex
            PushList(STOP_VALUE)
            unlock mutex
            sem_post(sem)
        JoinConsumers(EX3_NUM_CONSUMERS)
        CleanUp()
*/

#define EX3_MESSAGE_COUNT 100
#define EX3_NUM_PRODUCERS 6
#define EX3_NUM_CONSUMERS 3
#define EX3_STOP_VALUE 0

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
    exercise3_thread_arg_t* thread_arg = (exercise3_thread_arg_t*)arg;
    int value = 0;
    int status = 0;
    int is_done = 0;

    assert(NULL != thread_arg);
    assert(NULL != thread_arg->shared);

    while (1)
    {
        pthread_mutex_lock(&(thread_arg->shared->mutex));
        if (EX3_MESSAGE_COUNT <= thread_arg->shared->produced)
        {
            is_done = 1;
        }
        else
        {
            value = thread_arg->shared->produced++;
        }

        pthread_mutex_unlock(&(thread_arg->shared->mutex));
        if (is_done)
        {
            return NULL;
        }

        value += 777;
        pthread_mutex_lock(&(thread_arg->shared->mutex));
        status =
            DListPushBack(thread_arg->shared->list, (void*)((size_t)value));
        pthread_mutex_unlock(&(thread_arg->shared->mutex));

        CheckSysCalls(status, "DListPushBack in Exercise3Producer()");
        CheckSysCalls(sem_post(&(thread_arg->shared->sem)),
                      "sem_post in Exercise3Producer()");
    }

    return NULL;
}

static void* Exercise3Consumer(void* arg)
{
    exercise3_thread_arg_t* thread_arg = (exercise3_thread_arg_t*)arg;
    int value = 0;

    assert(NULL != thread_arg);
    assert(NULL != thread_arg->shared);

    while (1)
    {
        CheckSysCalls(sem_wait(&(thread_arg->shared->sem)),
                      "sem_wait in Exercise3Consumer()");
        pthread_mutex_lock(&(thread_arg->shared->mutex));
        value = (int)(size_t)DListPopFront(thread_arg->shared->list);
        pthread_mutex_unlock(&(thread_arg->shared->mutex));
        if (EX3_STOP_VALUE == value)
        {
            return NULL;
        }
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
    int status = 0;

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
        pthread_mutex_lock(&shared.mutex);
        status = DListPushBack(shared.list, (void*)((size_t)EX3_STOP_VALUE));
        pthread_mutex_unlock(&shared.mutex);
        if (SUCCESS != status)
        {
            exit(status);
        }

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
*   loop forever and do:
        LockMutex
        if (EX4_COUNT_MSG <= produced)
            UnLockMutex
            return
        value = produced_count
        produced_count += 1
        UnLockMutex
        msg = CreateMsg(value)
        SemWait to empty_slot
        LockMutex
        QueuePushEnd(msg)
        UpDateFSQWritePosition()
        UnLockMutex
        SemPost to full_slots

* Consumer:
    loop forever and do:
        SemWait to full_slot
        LockMutex
        if (EX4_MESSAGE_COUNT <= consumed)
            UnLockMutex
            SemPost to full_slots

            return

        val = QueuePopFront()
        UpDateFSQReadPosition()
        consumed += 1
        if (EX4_MESSAGE_COUNT <= consumed)
            is_done = 1
        UnLockMutex
        SemPost to empty_slots

        if (is_done)
            SemPost to full_slots
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
    exercise4_thread_arg_t* thread_arg = (exercise4_thread_arg_t*)arg;
    int value = 0;

    assert(NULL != thread_arg);
    assert(NULL != thread_arg->shared);

    while (1)
    {
        pthread_mutex_lock(&(thread_arg->shared->mutex));
        if (EX4_MESSAGE_COUNT <= thread_arg->shared->produced)
        {
            pthread_mutex_unlock(&(thread_arg->shared->mutex));

            return NULL;
        }

        value = thread_arg->shared->produced++;
        pthread_mutex_unlock(&(thread_arg->shared->mutex));

        value += 777;
        CheckSysCalls(sem_wait(&(thread_arg->shared->empty_slots)),
                      "sem_wait in Exercise4Producer()");
        pthread_mutex_lock(&(thread_arg->shared->mutex));
        thread_arg->shared->queue[thread_arg->shared->tail] = value;
        thread_arg->shared->tail =
            (thread_arg->shared->tail + 1) % thread_arg->shared->capacity;
        pthread_mutex_unlock(&(thread_arg->shared->mutex));
        CheckSysCalls(sem_post(&(thread_arg->shared->full_slots)),
                      "sem_post in Exercise4Producer()");
    }

    return NULL;
}

static void* Exercise4Consumer(void* arg)
{
    exercise4_thread_arg_t* thread_arg = (exercise4_thread_arg_t*)arg;
    int value = 0;
    int is_done = 0;

    assert(NULL != thread_arg);
    assert(NULL != thread_arg->shared);

    while (1)
    {
        CheckSysCalls(sem_wait(&(thread_arg->shared->full_slots)),
                      "sem_wait in Exercise4Consumer()");
        pthread_mutex_lock(&(thread_arg->shared->mutex));
        if (EX4_MESSAGE_COUNT <= thread_arg->shared->consumed)
        {
            pthread_mutex_unlock(&(thread_arg->shared->mutex));
            CheckSysCalls(sem_post(&(thread_arg->shared->full_slots)),
                          "sem_post chain in Exercise4Consumer()");

            return NULL;
        }

        value = thread_arg->shared->queue[thread_arg->shared->head];
        thread_arg->shared->head =
            (thread_arg->shared->head + 1) % thread_arg->shared->capacity;
        is_done = (EX4_MESSAGE_COUNT <= (++(thread_arg->shared->consumed)));
        pthread_mutex_unlock(&(thread_arg->shared->mutex));
        UNUSED(value);
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
/* psodo:
* Producer:
*   while(more_msg_to_produce) do:
        FSQEnqueue(BuildMsg())

* FSQEnqueue(val):
    WaitSemSignal(fsq.empty_slots)
    mutex_luck(fsq.write)
    fsq.buffer[fsq.write_index] = val
    MoveAhed(fsq.write_index)
    mutex_unluck(fsq.write)
    SendSemSignal(fsq.full_slot)

* Consumer:
    while(more_msg_to_read) do:
        Action(FSQDequeue())

* FSQDequeue()
    WaitSemSignal(fsq.full_slot)
    mutex_luck(fsq.read)
    val = fsq.buffer[fsq.read_index]
    MoveAhed(fsq.read_index)
    mutex_unluck(fsq.read)
    SendSemSignal(fsq.empty_slots)
    return val

* Main:
    CreateFSQ()
    masse_need_to_preduce_per_producer = calc(EX5_MESSAGE_COUNT,
                                              EX5_NUM_PRODUCERS)
    masse_need_to_read_per_consumer = calc(EX5_MESSAGE_COUNT,
                                             EX5_NUM_CONSUMERS)
    CreateAllProducers()
    CreateAllConsumers()
    JoinAllProducers()
    JoinAllConsumers()
    CleanUp()
*/
#define EX5_MESSAGE_COUNT 50
#define EX5_NUM_PRODUCERS 6
#define EX5_NUM_CONSUMERS 3
#define EX5_QUEUE_CAPACITY 10

typedef struct fsq
{
    int* buffer;
    size_t capacity;
    size_t read_index;
    size_t write_index;
    pthread_mutex_t read_mutex;
    pthread_mutex_t write_mutex;
    sem_t empty_slots;
    sem_t full_slots;
} fsq_t;

static fsq_t* FSQCreate(size_t capacity)
{
    fsq_t* fsq = NULL;

    assert(0 < capacity);

    fsq = (fsq_t*)malloc(sizeof(fsq_t));
    if (NULL == fsq)
    {
        return NULL;
    }

    fsq->buffer = (int*)malloc(sizeof(int) * capacity);
    if (NULL == fsq->buffer)
    {
        free(fsq);
        fsq = NULL;

        return NULL;
    }

    fsq->capacity = capacity;
    fsq->read_index = 0;
    fsq->write_index = 0;
    CheckSysCalls(pthread_mutex_init(&fsq->read_mutex, NULL), "mutex_init");
    CheckSysCalls(pthread_mutex_init(&fsq->write_mutex, NULL), "mutex_init");
    CheckSysCalls(sem_init(&fsq->empty_slots, 0, (unsigned int)capacity),
                  "sem_init");
    CheckSysCalls(sem_init(&fsq->full_slots, 0, 0), "sem_init");

    return fsq;
}

static void FSQDestroy(fsq_t* fsq)
{
    assert(NULL != fsq);

    CheckSysCalls(pthread_mutex_destroy(&fsq->read_mutex), "mutex_destroy");
    CheckSysCalls(pthread_mutex_destroy(&fsq->write_mutex), "mutex_destroy");
    CheckSysCalls(sem_destroy(&fsq->empty_slots), "sem_destroy");
    CheckSysCalls(sem_destroy(&fsq->full_slots), "sem_destroy");
    free(fsq->buffer);
    fsq->buffer = NULL;
    free(fsq);
    fsq = NULL;
}

static void FSQEnqueue(fsq_t* fsq, int value)
{
    assert(NULL != fsq);

    CheckSysCalls(sem_wait(&fsq->empty_slots), "sem_wait in FSQEnqueue()");
    pthread_mutex_lock(&(fsq->write_mutex));
    fsq->buffer[fsq->write_index] = value;
    fsq->write_index = (fsq->write_index + 1) % fsq->capacity;
    pthread_mutex_unlock(&(fsq->write_mutex));
    CheckSysCalls(sem_post(&fsq->full_slots), "sem_post in FSQEnqueue()");
}

static int FSQDequeue(fsq_t* fsq)
{
    int value = 0;

    assert(NULL != fsq);

    CheckSysCalls(sem_wait(&(fsq->full_slots)), "sem_wait in FSQDequeue()");
    pthread_mutex_lock(&(fsq->read_mutex));
    value = fsq->buffer[fsq->read_index];
    fsq->read_index = (fsq->read_index + 1) % fsq->capacity;
    pthread_mutex_unlock(&(fsq->read_mutex));
    CheckSysCalls(sem_post(&fsq->empty_slots), "sem_post in FSQDequeue()");

    return value;
}

typedef struct exercise5_thread_arg
{
    fsq_t* fsq;
    size_t thread_id;
    size_t my_count;
    size_t my_start;
} exercise5_thread_arg_t;

static void* Exercise5Producer(void* arg)
{
    exercise5_thread_arg_t* thread_arg = (exercise5_thread_arg_t*)arg;
    int value = 0;
    size_t i = 0;

    assert(NULL != thread_arg);

    for (; i < thread_arg->my_count; ++i)
    {
        value = 777 + (int)(thread_arg->my_start + i);
        FSQEnqueue(thread_arg->fsq, value);
    }

    return NULL;
}

static void* Exercise5Consumer(void* arg)
{
    exercise5_thread_arg_t* thread_arg = (exercise5_thread_arg_t*)arg;
    size_t i = 0;

    assert(NULL != thread_arg);

    for (; i < thread_arg->my_count; ++i)
    {
        FSQDequeue(thread_arg->fsq);
    }

    return NULL;
}

static void EX5(void)
{
    exercise5_thread_arg_t producer_args[EX5_NUM_PRODUCERS] = {0};
    exercise5_thread_arg_t consumer_args[EX5_NUM_CONSUMERS] = {0};
    pthread_t producers[EX5_NUM_PRODUCERS] = {0};
    pthread_t consumers[EX5_NUM_CONSUMERS] = {0};
    fsq_t* fsq = NULL;
    size_t base = 0;
    size_t remainder = 0;
    size_t offset = 0;
    size_t i = 0;

    fsq = FSQCreate(EX5_QUEUE_CAPACITY);
    if (NULL == fsq)
    {
        printf("FSQCreate failed\n");
        exit(1);
    }

    base = EX5_MESSAGE_COUNT / EX5_NUM_PRODUCERS;      /* 20 / 3 = 6 */
    remainder = EX5_MESSAGE_COUNT % EX5_NUM_PRODUCERS; /* 20 % 3 = 2*/
    for (; i < EX5_NUM_PRODUCERS; ++i)
    {
        producer_args[i].fsq = fsq;
        producer_args[i].thread_id = i;
        producer_args[i].my_count = base + (i < remainder);
        producer_args[i].my_start = offset;
        offset += producer_args[i].my_count;
        CheckSysCalls(pthread_create(&producers[i], NULL, Exercise5Producer,
                                     &producer_args[i]),
                      "pthread_create");
    }

    base = EX5_MESSAGE_COUNT / EX5_NUM_CONSUMERS;
    remainder = EX5_MESSAGE_COUNT % EX5_NUM_CONSUMERS;
    for (i = 0; i < EX5_NUM_CONSUMERS; ++i)
    {
        consumer_args[i].fsq = fsq;
        consumer_args[i].thread_id = i;
        consumer_args[i].my_count = base + (i < remainder);
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

    FSQDestroy(fsq);
}

/*======================= End Ex5 ====================*/

/*===========================Start Ex6=======================*/
/* psodo:

*  CreateBarrier:
        loop EX6_NUM_CONSUMERS times and do:
            sem_wait(consumers_ready)

*  Producer:
   while(more_msg_to_produce) do:
        CreateBarrier(shared.consumers_ready)
        msg = CreateMsg()
        LockMutex(shared.mutex)
        shared.msg = msg
        UpdateVersionChange()
        UnLockMutex(shared.mutex)
        Action(msg)
        more_msg_to_produce -= 1
        BroudcastAllLisnter(shared.cond) (will wake up all consumers)

    CreateBarrier(shared.consumers_ready)
    LockMutex(shared.mutex)
    MarkAsFinished()
    UpdateVersionChange()
    UnLockMutex(shared.mutex)
    BroudcastAllLisnter(shared.cond)

*   Consumer:
        loop forever and do:
            sem_post(shared.consumers_ready)
            LockMutex(shared.mutex)
            while my_version == shared.version
                cond_wait(shared.cond, shared.mutex)
            LockMutex(shared.mutex)
            if shared->done == 1
                local_done = 1
            else
                msg = shared.message;
                my_version = shared.last_version;
            UnLockMutex(shared.mutex)
            if local_done == 1
                return
            action(msg)
*/

#define EX6_MESSAGE_COUNT 10
#define EX6_NUM_CONSUMERS 4

typedef struct exercise6_shared
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    sem_t consumers_ready;
    int message;
    int version;
    int done;
} exercise6_shared_t;

typedef struct exercise6_consumer_arg
{
    exercise6_shared_t* shared;
    size_t thread_id;
    int last_version;
} exercise6_consumer_arg_t;

static void* Exercise6Producer(void* arg)
{
    exercise6_shared_t* shared = (exercise6_shared_t*)arg;
    size_t index = 0;
    size_t val = 0;
    size_t i = 0;

    assert(NULL != shared);

    for (; index <= EX6_MESSAGE_COUNT; ++index)
    {
        for (i = 0; i < EX6_NUM_CONSUMERS; ++i)
        {
            CheckSysCalls(sem_wait(&shared->consumers_ready),
                          "sem_wait in Exercise6Producer()");
        }

        val = 777 + index;
        pthread_mutex_lock(&(shared->mutex));
        if (index == (size_t)EX6_MESSAGE_COUNT)
        {
            shared->done = 1;
        }
        else
        {
            shared->message = val;
        }

        ++(shared->version);
        pthread_mutex_unlock(&(shared->mutex));
        CheckSysCalls(pthread_cond_broadcast(&shared->cond),
                      "cond_broadcast in Exercise6Producer()");
    }

    return NULL;
}

static void* Exercise6Consumer(void* arg)
{
    exercise6_consumer_arg_t* thread_arg = (exercise6_consumer_arg_t*)arg;
    int msg = 0;
    int is_done = 0;

    assert(NULL != thread_arg);

    while (1)
    {
        CheckSysCalls(sem_post(&thread_arg->shared->consumers_ready),
                      "sem_post in Exercise6Consumer()");
        pthread_mutex_lock(&(thread_arg->shared->mutex));
        while (thread_arg->last_version == thread_arg->shared->version)
        {
            CheckSysCalls(pthread_cond_wait(&thread_arg->shared->cond,
                                            &thread_arg->shared->mutex),
                          "cond_wait in Exercise6Consumer()");
        }

        if (thread_arg->shared->done)
        {
            is_done = 1;
        }
        else
        {
            msg = thread_arg->shared->message;
            thread_arg->last_version = thread_arg->shared->version;
        }

        pthread_mutex_unlock(&(thread_arg->shared->mutex));
        UNUSED(msg);
        if (is_done)
        {
            return NULL;
        }
    }

    return NULL;
}

static void EX6(void)
{
    exercise6_consumer_arg_t consumer_args[EX6_NUM_CONSUMERS] = {0};
    pthread_t consumers[EX6_NUM_CONSUMERS] = {0};
    pthread_t producer_thread = {0};
    exercise6_shared_t shared = {0};
    size_t i = 0;

    shared.version = 0;
    shared.done = 0;
    shared.message = 0;
    CheckSysCalls(pthread_mutex_init(&shared.mutex, NULL), "mutex_init");
    CheckSysCalls(pthread_cond_init(&shared.cond, NULL), "cond_init");
    CheckSysCalls(sem_init(&shared.consumers_ready, 0, 0), "sem_init");

    for (; i < EX6_NUM_CONSUMERS; ++i)
    {
        consumer_args[i].shared = &shared;
        consumer_args[i].thread_id = i;
        consumer_args[i].last_version = 0;
        CheckSysCalls(pthread_create(&consumers[i], NULL, Exercise6Consumer,
                                     &consumer_args[i]),
                      "pthread_create");
    }

    CheckSysCalls(
        pthread_create(&producer_thread, NULL, Exercise6Producer, &shared),
        "pthread_create");
    CheckSysCalls(pthread_join(producer_thread, NULL), "pthread_join");

    for (i = 0; i < EX6_NUM_CONSUMERS; ++i)
    {
        CheckSysCalls(pthread_join(consumers[i], NULL), "pthread_join");
    }

    CheckSysCalls(pthread_mutex_destroy(&shared.mutex), "mutex_destroy");
    CheckSysCalls(pthread_cond_destroy(&shared.cond), "cond_destroy");
    CheckSysCalls(sem_destroy(&shared.consumers_ready), "sem_destroy");
}

/*======================= End Ex6 ====================*/

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
    ActiveEx(EX6, "EX6");

    return 0;
}