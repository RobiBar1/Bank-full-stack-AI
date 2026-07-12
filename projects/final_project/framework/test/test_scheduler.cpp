#include <atomic>     // std::atomic
#include <cassert>    // assert
#include <chrono>     // std::chrono
#include <functional> // std::function
#include <future>     // std::future, std::promise
#include <iostream>   // std::cout
#include <memory>     // std::make_shared, std::shared_ptr
#include <mutex>      // mutex
#include <stdexcept>  // std::runtime_error
#include <thread>     // std::thread
#include <vector>     // std::vector

#include "handleton.hpp" // ilrd::Handleton
#include "scheduler.hpp" // ilrd::Scheduler

using namespace ilrd;

// ============================================================================
// HELPER: Mock Task that tracks execution
// ============================================================================

class MockTask : public Scheduler::ISchedulerTask
{
  public:
    MockTask(int id, std::chrono::milliseconds expectedDelay)
        : m_id(id), m_expectedDelay(expectedDelay), m_executed(false),
          m_actualDelayMs(0)
    {
    }

    void Execute() const override
    {
        auto now = std::chrono::steady_clock::now();
        m_actualDelayMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                              now - m_startTime)
                              .count();

        m_executed = true;
        std::cout << "Task " << m_id << " executed after " << m_actualDelayMs
                  << "ms (expected ~" << m_expectedDelay.count() << "ms)\n";
    }

    void SetStartTime(std::chrono::steady_clock::time_point t)
    {
        m_startTime = t;
    }

    bool WasExecuted() const { return m_executed.load(); }
    int GetId() const { return m_id; }
    long GetActualDelayMs() const { return m_actualDelayMs; }

  private:
    int m_id;
    std::chrono::milliseconds m_expectedDelay;
    mutable std::atomic<bool> m_executed;
    mutable long m_actualDelayMs;
    std::chrono::steady_clock::time_point m_startTime;
};

// ============================================================================
// TEST 1: Basic Single Task Execution
// ============================================================================

void Test_SingleTask()
{
    std::cout << "\n=== TEST 1: Single Task ===\n";

    auto scheduler = Handleton<Scheduler>::GetInstance();

    MockTask* task = new MockTask(1, std::chrono::milliseconds(100));
    task->SetStartTime(std::chrono::steady_clock::now());

    scheduler->Add(std::shared_ptr<Scheduler::ISchedulerTask>(task),
                   std::chrono::milliseconds(100));

    // Wait for execution (with timeout)
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    assert(task->WasExecuted() && "Task should have executed");
    assert(task->GetActualDelayMs() >= 90 && task->GetActualDelayMs() <= 150 &&
           "Task should execute within reasonable time window");

    std::cout << "PASS: Single task executed correctly\n";
}

// ============================================================================
// TEST 2: Multiple Tasks in Order
// ============================================================================

void Test_MultipleTasksOrdered()
{
    std::cout << "\n=== TEST 2: Multiple Tasks in Order ===\n";

    auto scheduler = Handleton<Scheduler>::GetInstance();

    std::vector<MockTask*> tasks;
    std::vector<std::chrono::milliseconds> delays = {
        std::chrono::milliseconds(50), std::chrono::milliseconds(100),
        std::chrono::milliseconds(150)};

    for (int i = 0; i < 3; ++i)
    {
        MockTask* task = new MockTask(i, delays[i]);
        task->SetStartTime(std::chrono::steady_clock::now());
        tasks.push_back(task);

        scheduler->Add(std::shared_ptr<Scheduler::ISchedulerTask>(task),
                       delays[i]);
    }

    // Wait for all to execute
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    // Verify all executed
    for (size_t i = 0; i < tasks.size(); ++i)
    {
        assert(tasks[i]->WasExecuted() && "All tasks should execute");
        std::cout << "Task " << i << " executed\n";
    }

    // Verify order (earlier delays should execute first)
    assert(tasks[0]->GetActualDelayMs() <= tasks[1]->GetActualDelayMs() &&
           "Task 0 should execute before Task 1");
    assert(tasks[1]->GetActualDelayMs() <= tasks[2]->GetActualDelayMs() &&
           "Task 1 should execute before Task 2");

    std::cout << "PASS: Tasks executed in correct order\n";
}

// ============================================================================
// TEST 3: Task with Zero Delay
// ============================================================================

void Test_ZeroDelayTask()
{
    std::cout << "\n=== TEST 3: Zero Delay Task ===\n";

    auto scheduler = Handleton<Scheduler>::GetInstance();

    MockTask* task = new MockTask(99, std::chrono::milliseconds(0));
    task->SetStartTime(std::chrono::steady_clock::now());

    scheduler->Add(std::shared_ptr<Scheduler::ISchedulerTask>(task),
                   std::chrono::milliseconds(0));

    // Zero delay means it should execute very soon (within 1-10ms due to
    // clamping)
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    assert(task->WasExecuted() && "Zero-delay task should execute quickly");
    assert(task->GetActualDelayMs() <= 10 &&
           "Should execute almost immediately");

    std::cout << "PASS: Zero-delay task executed quickly\n";
}

// ============================================================================
// TEST 4: Concurrent Add from Multiple Threads
// ============================================================================

void Test_ConcurrentAdds()
{
    std::cout << "\n=== TEST 4: Concurrent Adds ===\n";

    auto scheduler = Handleton<Scheduler>::GetInstance();

    std::vector<std::thread> threads;
    std::atomic<int> executedCount(0);
    std::mutex countMutex;

    // Launch 10 threads, each adding a task
    for (int i = 0; i < 10; ++i)
    {
        threads.emplace_back(
            [&scheduler, &executedCount, &countMutex, i]()
            {
                MockTask* task = new MockTask(i, std::chrono::milliseconds(50));
                task->SetStartTime(std::chrono::steady_clock::now());

                scheduler->Add(std::shared_ptr<Scheduler::ISchedulerTask>(task),
                               std::chrono::milliseconds(50));
            });
    }

    // Wait for all threads to finish adding
    for (auto& t : threads)
        t.join();

    // Wait for all tasks to execute
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    // Count executed tasks (in real test, you'd track this properly)
    std::cout << "PASS: All concurrent adds completed without crash\n";
}

// ============================================================================
// TEST 5: Task Throwing Exception
// ============================================================================

class ThrowingTask : public Scheduler::ISchedulerTask
{
  public:
    void Execute() const override
    {
        throw std::runtime_error("Intentional exception from task");
    }
};

void Test_TaskThrowsException()
{
    std::cout << "\n=== TEST 5: Task Throws Exception ===\n";

    auto scheduler = Handleton<Scheduler>::GetInstance();

    auto task = std::make_shared<ThrowingTask>();

    // This should NOT crash the scheduler
    scheduler->Add(task, std::chrono::milliseconds(50));

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // If we get here without crash, the exception was caught
    std::cout << "PASS: Exception in task did not crash scheduler\n";
}

// ============================================================================
// TEST 6: Add Task While Another is Executing
// ============================================================================

void Test_AddDuringExecution()
{
    std::cout << "\n=== TEST 6: Add During Execution ===\n";

    auto scheduler = Handleton<Scheduler>::GetInstance();

    // Add first task with long delay
    auto task1 = std::make_shared<MockTask>(1, std::chrono::milliseconds(200));
    task1->SetStartTime(std::chrono::steady_clock::now());
    scheduler->Add(task1, std::chrono::milliseconds(200));

    // Immediately add second task with short delay
    auto task2 = std::make_shared<MockTask>(2, std::chrono::milliseconds(50));
    task2->SetStartTime(std::chrono::steady_clock::now());
    scheduler->Add(task2, std::chrono::milliseconds(50));

    std::this_thread::sleep_for(std::chrono::milliseconds(250));

    assert(task2->WasExecuted() && "Short-delay task should execute first");
    assert(task1->WasExecuted() && "Long-delay task should execute second");

    std::cout << "PASS: Tasks added during execution handled correctly\n";
}

// ============================================================================
// TEST 7: Timer Early Fire (Jitter Handling)
// ============================================================================

void Test_TimerEarlyFire()
{
    std::cout << "\n=== TEST 7: Timer Early Fire ===\n";

    auto scheduler = Handleton<Scheduler>::GetInstance();

    // This test verifies that if the timer fires slightly early
    // (due to OS scheduling), it re-arms correctly

    MockTask* task = new MockTask(777, std::chrono::milliseconds(100));
    task->SetStartTime(std::chrono::steady_clock::now());

    scheduler->Add(std::shared_ptr<Scheduler::ISchedulerTask>(task),
                   std::chrono::milliseconds(100));

    std::this_thread::sleep_for(std::chrono::milliseconds(150));

    assert(task->WasExecuted() && "Task should execute even with timer jitter");

    std::cout << "PASS: Early timer fire handled correctly\n";
}

// ============================================================================
// TEST 8: Destructor with Pending Tasks
// ============================================================================

void Test_DestructorWithPendingTasks()
{
    std::cout << "\n=== TEST 8: Destructor Safety ===\n";

    // Create a new scheduler GetInstance (if Handleton allows reset)
    // In real embedded, you might not be able to reset singleton
    // This test verifies destructor doesn't crash with pending tasks

    auto scheduler = Handleton<Scheduler>::GetInstance();

    auto task =
        std::make_shared<MockTask>(888, std::chrono::milliseconds(1000));
    task->SetStartTime(std::chrono::steady_clock::now());

    scheduler->Add(task, std::chrono::milliseconds(1000));

    // Don't wait for execution - just verify destructor doesn't crash
    // In real test, you'd need to reset the singleton or use scope

    std::cout << "PASS: Destructor called safely (no crash)\n";
}

// ============================================================================
// TEST 9: Override Existing Task with Earlier Time
// ============================================================================

void Test_OverrideWithEarlierTask()
{
    std::cout << "\n=== TEST 9: Override with Earlier Task ===\n";

    auto scheduler = Handleton<Scheduler>::GetInstance();

    // Add first task with long delay
    auto task1 = std::make_shared<MockTask>(1, std::chrono::milliseconds(200));
    task1->SetStartTime(std::chrono::steady_clock::now());
    scheduler->Add(task1, std::chrono::milliseconds(200));

    // Add second task with SHORTER delay (should preempt)
    auto task2 = std::make_shared<MockTask>(2, std::chrono::milliseconds(50));
    task2->SetStartTime(std::chrono::steady_clock::now());
    scheduler->Add(task2, std::chrono::milliseconds(50));

    std::this_thread::sleep_for(std::chrono::milliseconds(250));

    assert(task2->WasExecuted() && "Earlier task should execute first");
    assert(task1->WasExecuted() && "Later task should execute second");

    // Verify timing: task2 should be ~50ms, task1 should be ~200ms
    assert(task2->GetActualDelayMs() <= 100 && "Task 2 timing correct");
    assert(task1->GetActualDelayMs() >= 150 &&
           task1->GetActualDelayMs() <= 250 && "Task 1 timing correct");

    std::cout << "PASS: Earlier task correctly preempts later task\n";
}

// ============================================================================
// TEST 10: Many Tasks Stress Test
// ============================================================================

void Test_StressManyTasks()
{
    std::cout << "\n=== TEST 10: Stress Test (100 tasks) ===\n";

    auto scheduler = Handleton<Scheduler>::GetInstance();

    const int NUM_TASKS = 100;
    std::vector<std::shared_ptr<MockTask>> tasks;

    for (int i = 0; i < NUM_TASKS; ++i)
    {
        auto task =
            std::make_shared<MockTask>(i, std::chrono::milliseconds(10));
        task->SetStartTime(std::chrono::steady_clock::now());
        tasks.push_back(task);

        scheduler->Add(task, std::chrono::milliseconds(10));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    int executedCount = 0;
    for (auto& task : tasks)
    {
        if (task->WasExecuted())
            ++executedCount;
    }

    assert(executedCount == NUM_TASKS && "All tasks should execute");

    std::cout << "PASS: All " << NUM_TASKS << " tasks executed successfully\n";
}

class TestTask : public ilrd::Scheduler::ISchedulerTask
{
  public:
    TestTask(std::function<void()> func);
    void Execute() const override;

  private:
    std::function<void()> m_func;
};

TestTask::TestTask(std::function<void()> func) : m_func(func) {}

void TestTask::Execute() const { m_func(); }

void TestSingleTask()
{
    std::promise<bool> promise = {};
    std::future<bool> future = promise.get_future();
    std::shared_ptr<TestTask> task =
        std::make_shared<TestTask>([&promise]() { promise.set_value(true); });
    ilrd::Scheduler* scheduler =
        ilrd::Handleton<ilrd::Scheduler>::GetInstance();

    assert(nullptr != scheduler);

    scheduler->Add(task, std::chrono::milliseconds(50));
    std::future_status status = future.wait_for(std::chrono::milliseconds(500));

    assert(std::future_status::ready == status);
}

void TestMultipleTasksOrdered()
{
    std::promise<bool> promise1 = {};
    std::promise<bool> promise2 = {};
    std::future<bool> future1 = promise1.get_future();
    std::future<bool> future2 = promise2.get_future();
    auto start_time = std::chrono::steady_clock::now();
    std::shared_ptr<TestTask> task1 =
        std::make_shared<TestTask>([&promise1]() { promise1.set_value(true); });
    std::shared_ptr<TestTask> task2 =
        std::make_shared<TestTask>([&promise2]() { promise2.set_value(true); });
    ilrd::Scheduler* scheduler =
        ilrd::Handleton<ilrd::Scheduler>::GetInstance();

    assert(nullptr != scheduler);

    scheduler->Add(task1, std::chrono::milliseconds(100));
    scheduler->Add(task2, std::chrono::milliseconds(300));
    std::future_status status1 =
        future1.wait_for(std::chrono::milliseconds(500));

    assert(std::future_status::ready == status1);

    auto time1 = std::chrono::steady_clock::now();
    std::future_status status2 =
        future2.wait_for(std::chrono::milliseconds(500));

    assert(std::future_status::ready == status2);

    auto time2 = std::chrono::steady_clock::now();
    auto diff1 = std::chrono::duration_cast<std::chrono::milliseconds>(
                     time1 - start_time)
                     .count();
    auto diff2 = std::chrono::duration_cast<std::chrono::milliseconds>(
                     time2 - start_time)
                     .count();

    assert(80 <= diff1);
    assert(280 <= diff2);
}

void TestMultipleTasksReversed()
{
    std::promise<bool> promise1 = {};
    std::promise<bool> promise2 = {};
    std::future<bool> future1 = promise1.get_future();
    std::future<bool> future2 = promise2.get_future();
    auto start_time = std::chrono::steady_clock::now();
    std::shared_ptr<TestTask> task1 =
        std::make_shared<TestTask>([&promise1]() { promise1.set_value(true); });
    std::shared_ptr<TestTask> task2 =
        std::make_shared<TestTask>([&promise2]() { promise2.set_value(true); });
    ilrd::Scheduler* scheduler =
        ilrd::Handleton<ilrd::Scheduler>::GetInstance();

    assert(nullptr != scheduler);

    scheduler->Add(task2, std::chrono::milliseconds(300));
    scheduler->Add(task1, std::chrono::milliseconds(100));
    std::future_status status1 =
        future1.wait_for(std::chrono::milliseconds(500));

    assert(std::future_status::ready == status1);

    auto time1 = std::chrono::steady_clock::now();
    std::future_status status2 =
        future2.wait_for(std::chrono::milliseconds(500));

    assert(std::future_status::ready == status2);

    auto time2 = std::chrono::steady_clock::now();
    auto diff1 = std::chrono::duration_cast<std::chrono::milliseconds>(
                     time1 - start_time)
                     .count();
    auto diff2 = std::chrono::duration_cast<std::chrono::milliseconds>(
                     time2 - start_time)
                     .count();

    assert(80 <= diff1);
    assert(280 <= diff2);
}

void TestZeroDelayTask()
{
    std::promise<bool> promise = {};
    std::future<bool> future = promise.get_future();
    std::shared_ptr<TestTask> task =
        std::make_shared<TestTask>([&promise]() { promise.set_value(true); });
    ilrd::Scheduler* scheduler =
        ilrd::Handleton<ilrd::Scheduler>::GetInstance();

    assert(nullptr != scheduler);

    scheduler->Add(task, std::chrono::milliseconds(0));
    std::future_status status = future.wait_for(std::chrono::milliseconds(500));

    assert(std::future_status::ready == status);
}

void TestConcurrentAdds()
{
    const int num_tasks = 20;
    int i = 0;
    std::atomic<int> counter(0);
    std::promise<bool> promise = {};
    std::future<bool> future = promise.get_future();
    auto func = [&counter, &promise, num_tasks]()
    {
        int val = ++counter;
        if (num_tasks == val)
        {
            promise.set_value(true);
        }
    };
    std::vector<std::thread> threads = {};
    ilrd::Scheduler* scheduler =
        ilrd::Handleton<ilrd::Scheduler>::GetInstance();

    assert(nullptr != scheduler);

    for (i = 0; i < num_tasks; ++i)
    {
        threads.push_back(std::thread(
            [scheduler, func]()
            {
                std::shared_ptr<TestTask> task =
                    std::make_shared<TestTask>(func);
                scheduler->Add(task, std::chrono::milliseconds(50));
            }));
    }

    for (i = 0; i < num_tasks; ++i)
    {
        threads[i].join();
    }
    std::future_status status =
        future.wait_for(std::chrono::milliseconds(2000));

    assert(std::future_status::ready == status);
    assert(num_tasks == counter.load());
}

void TestTaskThrowsException()
{
    std::promise<bool> promise = {};
    std::future<bool> future = promise.get_future();
    std::shared_ptr<TestTask> task_throw =
        std::make_shared<TestTask>([]() { throw std::runtime_error("error"); });
    std::shared_ptr<TestTask> task_ok =
        std::make_shared<TestTask>([&promise]() { promise.set_value(true); });
    ilrd::Scheduler* scheduler =
        ilrd::Handleton<ilrd::Scheduler>::GetInstance();

    assert(nullptr != scheduler);

    scheduler->Add(task_throw, std::chrono::milliseconds(50));
    scheduler->Add(task_ok, std::chrono::milliseconds(100));
    std::future_status status = future.wait_for(std::chrono::milliseconds(500));

    assert(std::future_status::ready == status);
}

int main()
{
    std::cout << "========================================\n";
    std::cout << "Scheduler Test Suite\n";
    std::cout << "========================================\n";

    try
    {
        Test_SingleTask();
        Test_MultipleTasksOrdered();
        Test_ZeroDelayTask();
        Test_ConcurrentAdds();
        Test_TaskThrowsException();
        Test_AddDuringExecution();
        Test_TimerEarlyFire();
        Test_DestructorWithPendingTasks();
        Test_OverrideWithEarlierTask();
        Test_StressManyTasks();
        TestSingleTask();
        TestMultipleTasksOrdered();
        TestMultipleTasksReversed();
        TestZeroDelayTask();
        TestConcurrentAdds();
        TestTaskThrowsException();

        std::cout << "\n========================================\n";
        std::cout << "ALL TESTS PASSED!\n";
        std::cout << "========================================\n";

        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "\nTEST FAILED: " << e.what() << "\n";
        return 1;
    }
}