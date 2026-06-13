#include <algorithm> // std::sort, std::unique
#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "thread_pool.hpp"

using namespace ilrd;

const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";

typedef enum
{
    TEST_STATUS_SUCCESS,
    TEST_STATUS_FAILURE
} test_status_t;

static test_status_t ThreadPool_SmokeTest(void);
static test_status_t ThreadPool_SingleThread_SingleTask_Test(void);
static test_status_t ThreadPool_SingleThread_MultipleTasks_Test(void);
static test_status_t ThreadPool_MultiThread_BasicTasks_Test(void);
static test_status_t ThreadPool_MultiThread_DifferentThreadsUsed_Test(void);
static test_status_t ThreadPool_FuncTask_BasicUsage_Test(void);
static test_status_t ThreadPool_MultiThread_ComplexTask_Test(void);
static test_status_t ThreadPool_Pause_TasksDontRunWhilePaused_Test(void);
static test_status_t ThreadPool_RunThenPause_StopsExecution_Test(void);
static test_status_t ThreadPool_PauseThenResume_Test(void);
static test_status_t ThreadPool_Stop_AllTasksComplete_Test(void);
static test_status_t ThreadPool_Priority_HighBeforeLow_Test(void);
static test_status_t ThreadPool_SetNumOfThread_Increase_Test(void);
static test_status_t ThreadPool_SetNumOfThread_Decrease_Test(void);
static test_status_t ThreadPool_StressTest_HighLoad_Test(void);
static test_status_t ThreadPool_AddWhileRunning_Test(void);

static void CheckTest(test_status_t result, std::string name)
{
    if (result == TEST_STATUS_SUCCESS)
    {
        std::cout << "-> " << name << ": " << GREEN << "PASSED\n\n" << RESET;
    }
    else
    {
        std::cout << "-> " << name << ": " << RED << "FAILED\n\n" << RESET;
    }
}

int test_pack1()
{
    std::cout << "\n ---- THREAD POOL TESTS ----\n\n";

    CheckTest(ThreadPool_SmokeTest(), "ThreadPool_SmokeTest");
    CheckTest(ThreadPool_SingleThread_SingleTask_Test(),
              "ThreadPool_SingleThread_SingleTask_Test");
    CheckTest(ThreadPool_SingleThread_MultipleTasks_Test(),
              "ThreadPool_SingleThread_MultipleTasks_Test");
    CheckTest(ThreadPool_MultiThread_BasicTasks_Test(),
              "ThreadPool_MultiThread_BasicTasks_Test");
    CheckTest(ThreadPool_MultiThread_DifferentThreadsUsed_Test(),
              "ThreadPool_MultiThread_DifferentThreadsUsed_Test");
    CheckTest(ThreadPool_FuncTask_BasicUsage_Test(),
              "ThreadPool_FuncTask_BasicUsage_Test");
    CheckTest(ThreadPool_MultiThread_ComplexTask_Test(),
              "ThreadPool_MultiThread_ComplexTask_Test");
    CheckTest(ThreadPool_Pause_TasksDontRunWhilePaused_Test(),
              "ThreadPool_Pause_TasksDontRunWhilePaused_Test");
    CheckTest(ThreadPool_RunThenPause_StopsExecution_Test(),
              "ThreadPool_RunThenPause_StopsExecution_Test");
    CheckTest(ThreadPool_PauseThenResume_Test(),
              "ThreadPool_PauseThenResume_Test");
    CheckTest(ThreadPool_Stop_AllTasksComplete_Test(),
              "ThreadPool_Stop_AllTasksComplete_Test");
    CheckTest(ThreadPool_Priority_HighBeforeLow_Test(),
              "ThreadPool_Priority_HighBeforeLow_Test");
    CheckTest(ThreadPool_SetNumOfThread_Increase_Test(),
              "ThreadPool_SetNumOfThread_Increase_Test");
    CheckTest(ThreadPool_SetNumOfThread_Decrease_Test(),
              "ThreadPool_SetNumOfThread_Decrease_Test");
    CheckTest(ThreadPool_StressTest_HighLoad_Test(),
              "ThreadPool_StressTest_HighLoad_Test");
    CheckTest(ThreadPool_AddWhileRunning_Test(),
              "ThreadPool_AddWhileRunning_Test");

    return 0;
}

class CounterTask : public ThreadPool::IThreadPoolTask
{
  public:
    explicit CounterTask(std::atomic<int>& counter) : m_counter(counter) {}
    void Execute() const { ++m_counter; }

  private:
    std::atomic<int>& m_counter;
};

class TidRecorderTask : public ThreadPool::IThreadPoolTask
{
  public:
    TidRecorderTask(std::vector<std::thread::id>& ids, std::mutex& mtx)
        : m_ids(ids), m_mtx(mtx)
    {
    }
    void Execute() const
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        m_ids.push_back(std::this_thread::get_id());
    }

  private:
    std::vector<std::thread::id>& m_ids;
    std::mutex& m_mtx;
};

class SleepTask : public ThreadPool::IThreadPoolTask
{
  public:
    explicit SleepTask(int ms, std::atomic<int>* done = NULL)
        : m_ms(ms), m_done(done)
    {
    }
    void Execute() const
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(m_ms));
        if (m_done)
        {
            ++(*m_done);
        }
    }

  private:
    int m_ms;
    std::atomic<int>* m_done;
};

class PriorityRecorderTask : public ThreadPool::IThreadPoolTask
{
  public:
    PriorityRecorderTask(Priority p, std::vector<Priority>& order,
                         std::mutex& mtx)
        : m_priority(p), m_order(order), m_mtx(mtx)
    {
    }
    void Execute() const
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        m_order.push_back(m_priority);
    }

  private:
    Priority m_priority;
    std::vector<Priority>& m_order;
    std::mutex& m_mtx;
};

/*static bool WaitFor(std::function<bool()> pred, int timeout_ms = 1000)
{
    auto deadline = std::chrono::steady_clock::now() +
                    std::chrono::milliseconds(timeout_ms);
    while (std::chrono::steady_clock::now() < deadline)
    {
        if (pred()) { return true; }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    return pred();
}*/

static test_status_t ThreadPool_SmokeTest(void)
{
    ThreadPool tp(2);

    return TEST_STATUS_SUCCESS;
}

static test_status_t ThreadPool_SingleThread_SingleTask_Test(void)
{
    std::atomic<int> counter(0);
    {
        ThreadPool tp(1);

        tp.Add(std::make_shared<CounterTask>(counter));
        tp.Run();
    }

    if (1 != counter.load())
    {
        return TEST_STATUS_FAILURE;
    }

    return TEST_STATUS_SUCCESS;
}

static test_status_t ThreadPool_SingleThread_MultipleTasks_Test(void)
{
    std::atomic<int> counter(0);
    const int N = 10;
    {
        ThreadPool tp(1);

        for (int i = 0; i < N; ++i)
        {
            tp.Add(std::make_shared<CounterTask>(counter));
        }

        tp.Run();
    }

    if (N != counter.load())
    {
        return TEST_STATUS_FAILURE;
    }

    /*if (false == WaitFor([&counter, N]{ return counter.load() == N; }))
    {
        return TEST_STATUS_FAILURE;
    }*/

    return TEST_STATUS_SUCCESS;
}

static test_status_t ThreadPool_MultiThread_BasicTasks_Test(void)
{
    std::atomic<int> counter(0);
    const int NUM_THREADS = 4;
    const int NUM_TASKS = 100;
    {
        ThreadPool tp(NUM_THREADS);

        for (int i = 0; i < NUM_TASKS; ++i)
        {
            tp.Add(std::make_shared<CounterTask>(counter));
        }

        tp.Run();
    }

    if (NUM_TASKS != counter.load())
    {
        return TEST_STATUS_FAILURE;
    }

    return TEST_STATUS_SUCCESS;
}

static test_status_t ThreadPool_MultiThread_DifferentThreadsUsed_Test(void)
{
    std::vector<std::thread::id> ids;
    std::mutex mtx;
    const int NUM_THREADS = 4;
    const int NUM_TASKS = 40;
    {
        ThreadPool tp(NUM_THREADS);

        for (int i = 0; i < NUM_TASKS; ++i)
        {
            tp.Add(std::make_shared<TidRecorderTask>(ids, mtx));
        }

        tp.Run();
    }

    if (NUM_TASKS != int(ids.size()))
    {
        return TEST_STATUS_FAILURE;
    }

    std::sort(ids.begin(), ids.end());
    auto unique_end = std::unique(ids.begin(), ids.end());
    int unique_count = int(std::distance(ids.begin(), unique_end));

    if (unique_count < 4)
    {
        std::cout << "Okay to fail here in Valgrind" << std::endl;
        return TEST_STATUS_FAILURE;
    }

    return TEST_STATUS_SUCCESS;
}

static test_status_t ThreadPool_FuncTask_BasicUsage_Test(void)
{
    std::atomic<int> counter(0);
    {
        ThreadPool tp(2);

        tp.Add(std::shared_ptr<ThreadPool::IThreadPoolTask>(
            new FuncTask([&counter] { ++counter; })));
        tp.Add(std::shared_ptr<ThreadPool::IThreadPoolTask>(
            new FuncTask([&counter] { ++counter; })));

        tp.Run();
    }
    if (2 != counter.load())
    {
        return TEST_STATUS_FAILURE;
    }

    return TEST_STATUS_SUCCESS;
}

static test_status_t ThreadPool_MultiThread_ComplexTask_Test(void)
{
    std::atomic<long> total(0);
    const int NUM_THREADS = 4;
    const int CHUNKS = 20;
    {
        ThreadPool tp(NUM_THREADS);

        for (int c = 0; c < CHUNKS; ++c)
        {
            tp.Add(std::shared_ptr<ThreadPool::IThreadPoolTask>(new FuncTask(
                [&total]
                {
                    long long sum = 0;
                    for (int i = 1; i <= 50; ++i)
                    {
                        sum += i;
                    }
                    total += sum;
                })));
        }

        tp.Run();
    }
    long expected = long(CHUNKS * (50 * 51 / 2));
    if (total.load() != expected)
    {
        return TEST_STATUS_FAILURE;
    }

    return TEST_STATUS_SUCCESS;
}

static test_status_t ThreadPool_Pause_TasksDontRunWhilePaused_Test(void)
{
    std::atomic<int> counter(0);
    {
        ThreadPool tp(2);

        for (int i = 0; i < 10; ++i)
        {
            tp.Add(std::make_shared<CounterTask>(counter));
        }

        if (0 != counter.load())
        {
            return TEST_STATUS_FAILURE;
        }
    }

    return TEST_STATUS_SUCCESS;
}

static test_status_t ThreadPool_RunThenPause_StopsExecution_Test(void)
{
    std::atomic<int> counter(0);
    int snapshot = 0;
    const int FAST_TASKS = 5;
    {
        ThreadPool tp(2);

        for (int i = 0; i < FAST_TASKS; ++i)
        {
            tp.Add(std::make_shared<CounterTask>(counter));
        }

        tp.Run();

        while (counter.load() != FAST_TASKS)
        {
            // empty
        }

        tp.Pause();
        snapshot = counter.load();

        for (int i = 0; i < 20; ++i)
        {
            tp.Add(std::make_shared<SleepTask>(20, &counter));
        }

        if (counter.load() != snapshot)
        {
            return TEST_STATUS_FAILURE;
        }
    }

    return TEST_STATUS_SUCCESS;
}

static test_status_t ThreadPool_PauseThenResume_Test(void)
{
    std::atomic<int> counter(0);
    {
        ThreadPool tp(2);

        for (int i = 0; i < 10; ++i)
        {
            tp.Add(std::make_shared<CounterTask>(counter));
        }

        if (0 != counter.load())
        {
            return TEST_STATUS_FAILURE;
        }

        tp.Run();
    }

    if (counter.load() != 10)
    {
        return TEST_STATUS_FAILURE;
    }

    return TEST_STATUS_SUCCESS;
}

static test_status_t ThreadPool_Stop_AllTasksComplete_Test(void)
{
    std::atomic<int> counter(0);
    const int N = 20;
    {
        ThreadPool tp(4);

        for (int i = 0; i < N; ++i)
        {
            tp.Add(std::make_shared<CounterTask>(counter));
        }

        tp.Run();
        tp.Stop();
    }
    if (counter.load() != N)
    {
        return TEST_STATUS_FAILURE;
    }

    return TEST_STATUS_SUCCESS;
}

static test_status_t ThreadPool_Priority_HighBeforeLow_Test(void)
{
    std::vector<Priority> order;
    std::mutex mtx;
    {
        ThreadPool tp(1);

        for (int i = 0; i < 5; ++i)
        {
            tp.Add(std::make_shared<PriorityRecorderTask>(LOW, order, mtx),
                   LOW);
        }
        for (int i = 0; i < 5; ++i)
        {
            tp.Add(std::make_shared<PriorityRecorderTask>(HIGH, order, mtx),
                   HIGH);
        }

        tp.Run();

        std::cout << "DEADLOCK HERE INDICATES RUN PROBLEMS" << std::endl;
        while (int(order.size()) != 10)
        {
            //
        }
    }

    std::lock_guard<std::mutex> lock(mtx);
    for (int i = 0; i < 5; ++i)
    {
        if (HIGH != order[i])
        {
            return TEST_STATUS_FAILURE;
        }
    }
    for (int i = 5; i < 10; ++i)
    {
        if (LOW != order[i])
        {
            return TEST_STATUS_FAILURE;
        }
    }

    return TEST_STATUS_SUCCESS;
}

static test_status_t ThreadPool_SetNumOfThread_Increase_Test(void)
{
    std::atomic<int> counter(0);
    const int NUM_TASKS = 40;
    {
        ThreadPool tp(2);

        for (int i = 0; i < NUM_TASKS; ++i)
        {
            tp.Add(std::make_shared<SleepTask>(10, &counter));
        }

        tp.Run();
        tp.SetNumOfThread(6);
    }

    if (counter.load() != NUM_TASKS)
    {
        return TEST_STATUS_FAILURE;
    }

    return TEST_STATUS_SUCCESS;
}

static test_status_t ThreadPool_SetNumOfThread_Decrease_Test(void)
{
    std::atomic<int> counter(0);
    {
        ThreadPool tp(4);

        tp.Run();
        tp.SetNumOfThread(1);

        for (int i = 0; i < 5; ++i)
        {
            tp.Add(std::make_shared<CounterTask>(counter));
        }
    }

    if (counter.load() != 5)
    {
        return TEST_STATUS_FAILURE;
    }

    return TEST_STATUS_SUCCESS;
}

static test_status_t ThreadPool_StressTest_HighLoad_Test(void)
{
    std::atomic<int> counter(0);
    const int NUM_THREADS = 8;
    const int NUM_TASKS = 500;
    {
        ThreadPool tp(NUM_THREADS);

        for (int i = 0; i < NUM_TASKS; ++i)
        {
            tp.Add(std::make_shared<CounterTask>(counter));
        }

        tp.Run();
    }

    if (counter.load() != NUM_TASKS)
    {
        return TEST_STATUS_FAILURE;
    }

    return TEST_STATUS_SUCCESS;
}

static test_status_t ThreadPool_AddWhileRunning_Test(void)
{
    std::atomic<int> counter(0);
    {
        ThreadPool tp(3);

        tp.Run();

        for (int i = 0; i < 15; ++i)
        {
            tp.Add(std::make_shared<CounterTask>(counter));
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
    }

    if (counter.load() != 15)
    {
        return TEST_STATUS_FAILURE;
    }

    return TEST_STATUS_SUCCESS;
}

int main()
{
    test_pack1();

    return 0;
}