#include <atomic>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_set>

#include "priority_queue.hpp"
#include "waitable_queue.hpp"

#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_RESET "\033[0m"

class TestSuite
{
  public:
    TestSuite() : total(0), passed(0) {}

    void AddResult(bool pass)
    {
        ++total;
        if (pass)
            ++passed;
    }

    void PrintSummary() const
    {
        if (passed == total)
            std::cout << COLOR_GREEN << "PASS: ";
        else
            std::cout << COLOR_RED << "FAIL: ";

        std::cout << passed << "/" << total << " passed\n" << COLOR_RESET;
    }

  private:
    size_t total;
    size_t passed;
};

template <typename T>
bool CheckValues(const T& res, const T& expected, std::stringstream& result)
{
    if (res != expected)
    {
        result << COLOR_RED << "values are different! Got " << res
               << ", expected " << expected << std::endl;
        return false;
    }

    return true;
}

template <typename Func>
void RunTest(TestSuite& suite, const char* name, Func test_fn)
{
    std::stringstream result;
    bool pass = test_fn(result);

    suite.AddResult(pass);

    if (!pass)
    {
        std::cout << COLOR_RED << "[FAIL] " << name << ": " << result.str()
                  << COLOR_RESET;
    }
}

bool RunTestWaitableQueueConstructor(std::stringstream& result)
{
    ilrd::WaitableQueue<int> queue;

    static_cast<void>(result);

    return true;
}

void RunTestsWaitableQueueConstructors()
{
    TestSuite suite;

    std::cout << COLOR_BLUE << "Running tests for WaitableQueue constructors"
              << COLOR_RESET << std::endl;

    RunTest(suite, "RunTestWaitableQueueConstructor",
            RunTestWaitableQueueConstructor);

    suite.PrintSummary();
    std::cout << COLOR_BLUE << "Finished tests" << COLOR_RESET << std::endl;
}

bool RunTestWaitableQueueSinglePush(std::stringstream& result)
{
    ilrd::WaitableQueue<int> queue;

    queue.Push(1);

    static_cast<void>(result);

    return true;
}

bool RunTestWaitableQueueMultiplePush(std::stringstream& result)
{
    ilrd::WaitableQueue<int> queue;

    queue.Push(1);
    queue.Push(2);
    queue.Push(3);
    queue.Push(4);

    static_cast<void>(result);

    return true;
}

bool RunTestWaitableQueueSinglePushAndPop(std::stringstream& result)
{
    ilrd::WaitableQueue<int> queue;
    int expected = 1;

    queue.Push(expected);

    int res;
    queue.Pop(&res);

    bool status = CheckValues(res, expected, result);

    return status;
}

bool RunTestWaitableQueueMultiplePushAndPop(std::stringstream& result)
{
    ilrd::WaitableQueue<int> queue;
    int expected1 = 1;
    int expected2 = 2;

    queue.Push(expected1);
    queue.Push(expected2);

    int res1;
    int res2;
    queue.Pop(&res1);
    queue.Pop(&res2);

    bool status = CheckValues(res1, expected1, result) &&
                  CheckValues(res2, expected2, result);

    return status;
}

bool RunTestWaitableQueueSinglePushAnPopNoTimeout(std::stringstream& result)
{
    ilrd::WaitableQueue<int> queue;
    int expected_elem = 1;
    bool expected_return = true;

    queue.Push(expected_elem);

    int result_elem;
    bool result_return =
        queue.Pop(&result_elem, std::chrono::milliseconds(100));

    bool status = CheckValues(result_return, expected_return, result) &&
                  CheckValues(result_elem, expected_elem, result);

    return status;
}

bool RunTestWaitableQueueSinglePushAnPopTimeout(std::stringstream& result)
{
    ilrd::WaitableQueue<int> queue;
    bool expected_return = false;

    int result_elem;
    bool result_return =
        queue.Pop(&result_elem, std::chrono::milliseconds(100));

    bool status = CheckValues(result_return, expected_return, result);

    return status;
}

void RunTestsWaitableQueuePush()
{
    TestSuite suite;

    std::cout << COLOR_BLUE << "Running tests for WaitableQueue push and pop"
              << COLOR_RESET << std::endl;

    RunTest(suite, "RunTestWaitableQueueSinglePush",
            RunTestWaitableQueueSinglePush);
    RunTest(suite, "RunTestWaitableQueueMultiplePush",
            RunTestWaitableQueueMultiplePush);
    RunTest(suite, "RunTestWaitableQueueSinglePushAndPop",
            RunTestWaitableQueueSinglePushAndPop);
    RunTest(suite, "RunTestWaitableQueueMultiplePushAndPop",
            RunTestWaitableQueueMultiplePushAndPop);
    RunTest(suite, "RunTestWaitableQueueSinglePushAnPopNoTimeout",
            RunTestWaitableQueueSinglePushAnPopNoTimeout);
    RunTest(suite, "RunTestWaitableQueueSinglePushAnPopTimeout",
            RunTestWaitableQueueSinglePushAnPopTimeout);

    suite.PrintSummary();
    std::cout << COLOR_BLUE << "Finished tests" << COLOR_RESET << std::endl;
}

bool RunTestWaitableQueueIsEmptyEmpty(std::stringstream& result)
{
    ilrd::WaitableQueue<int> queue;
    bool expected = true;

    bool res = queue.IsEmpty();

    bool status = CheckValues(res, expected, result);

    return status;
}

bool RunTestWaitableQueueIsEmptyNotEmpty(std::stringstream& result)
{
    ilrd::WaitableQueue<int> queue;
    bool expected = false;

    queue.Push(1);

    bool res = queue.IsEmpty();

    bool status = CheckValues(res, expected, result);

    return status;
}

void RunTestsWaitableQueueIsEmpty()
{
    TestSuite suite;

    std::cout << COLOR_BLUE << "Running tests for WaitableQueue IsEmpty"
              << COLOR_RESET << std::endl;

    RunTest(suite, "RunTestWaitableQueueIsEmptyEmpty",
            RunTestWaitableQueueIsEmptyEmpty);
    RunTest(suite, "RunTestWaitableQueueIsEmptyNotEmpty",
            RunTestWaitableQueueIsEmptyNotEmpty);

    suite.PrintSummary();
    std::cout << COLOR_BLUE << "Finished tests" << COLOR_RESET << std::endl;
}

bool RunTestWaitableQueuePopThanPush(std::stringstream& result)
{
    ilrd::WaitableQueue<int> queue;
    int expected = 1;
    int res;
    auto producer_func = [&] { queue.Push(expected); };
    auto consumer_func = [&] { queue.Pop(&res); };

    std::thread consumer(consumer_func);
    std::thread producer(producer_func);

    consumer.join();
    producer.join();

    bool status = CheckValues(res, expected, result);

    return status;
}

bool RunTestWaitableQueuePushThanPop(std::stringstream& result)
{
    ilrd::WaitableQueue<int> queue;
    int expected = 1;
    int res;
    auto producer_func = [&] { queue.Push(expected); };
    auto consumer_func = [&] { queue.Pop(&res); };

    std::thread producer(producer_func);
    std::thread consumer(consumer_func);

    consumer.join();
    producer.join();

    bool status = CheckValues(res, expected, result);

    return status;
}

bool RunTestWaitableQueueManyProducersOneConsumer(std::stringstream& result)
{
    ilrd::WaitableQueue<size_t> queue;
    const std::size_t NUM_PRODUCERS = 4;
    const std::size_t NUM_ITEMS_TO_PRODUCE = 1000;
    bool expected = true;
    bool res;
    auto producer_func = [&]
    {
        for (std::size_t i = 0; i < NUM_ITEMS_TO_PRODUCE; ++i)
        {
            queue.Push(i);
        }
    };
    auto consumer_func = [&]
    {
        size_t out;
        for (std::size_t i = 0; i < NUM_PRODUCERS * NUM_ITEMS_TO_PRODUCE; ++i)
        {
            if (!queue.Pop(&out, std::chrono::milliseconds(500)))
            {
                res = false;
                return;
            }
        }
        res = true;
    };

    std::vector<std::thread> producers;
    for (size_t i = 0; i < NUM_PRODUCERS; ++i)
    {
        producers.emplace_back(producer_func);
    }
    std::thread consumer(consumer_func);

    for (size_t i = 0; i < NUM_PRODUCERS; ++i)
    {
        producers.at(i).join();
    }

    consumer.join();

    bool status = CheckValues(res, expected, result) &&
                  CheckValues(queue.IsEmpty(), true, result);

    return status;
}

bool RunTestWaitableQueueOneProducerManyConsmers(std::stringstream& result)
{
    ilrd::WaitableQueue<size_t> queue;
    const std::size_t NUM_CONSUMERS = 4;
    const std::size_t NUM_ITEMS_TO_PRODUCE = 4000;
    std::size_t expected = NUM_ITEMS_TO_PRODUCE;
    std::size_t count = 0;
    std::mutex lock;
    auto producer_func = [&]
    {
        for (std::size_t i = 0; i < NUM_ITEMS_TO_PRODUCE; ++i)
        {
            queue.Push(i);
        }
    };
    auto consumer_func = [&]
    {
        size_t out;
        while (count < NUM_ITEMS_TO_PRODUCE)
        {
            lock.lock();
            if (count < NUM_ITEMS_TO_PRODUCE)
            {
                ++count;
                queue.Pop(&out);
            }

            lock.unlock();
        }
    };

    std::thread producer(producer_func);
    std::vector<std::thread> consumers;
    for (size_t i = 0; i < NUM_CONSUMERS; ++i)
    {
        consumers.emplace_back(consumer_func);
    }

    producer.join();

    for (size_t i = 0; i < NUM_CONSUMERS; ++i)
    {
        consumers.at(i).join();
    }

    bool status = CheckValues(std::size_t(count), expected, result) &&
                  CheckValues(queue.IsEmpty(), true, result);

    return status;
}

bool RunTestWaitableQueueManyProducerManyConsmers(std::stringstream& result)
{
    ilrd::WaitableQueue<std::size_t, ilrd::PriorityQueue<std::size_t>> queue;
    const std::size_t NUM_PRODUCERS = 4;
    const std::size_t NUM_CONSUMERS = 4;
    const std::size_t NUM_ITEMS_TO_PRODUCE = 1000;
    std::size_t expected = NUM_PRODUCERS * NUM_ITEMS_TO_PRODUCE;
    std::unordered_set<size_t> set;
    std::size_t count = 0;
    std::mutex lock;
    auto producer_func = [&](std::size_t p)
    {
        for (std::size_t i = 0; i < NUM_ITEMS_TO_PRODUCE; ++i)
        {
            queue.Push(p * NUM_ITEMS_TO_PRODUCE + i);
        }
    };
    auto consumer_func = [&]
    {
        size_t out;
        while (count < NUM_PRODUCERS * NUM_ITEMS_TO_PRODUCE)
        {
            lock.lock();
            if (count < NUM_PRODUCERS * NUM_ITEMS_TO_PRODUCE)
            {
                ++count;
                queue.Pop(&out);
                set.insert(out);
            }

            lock.unlock();
        }
    };

    std::vector<std::thread> producers;
    for (size_t i = 0; i < NUM_PRODUCERS; ++i)
    {
        producers.emplace_back(producer_func, i);
    }
    std::vector<std::thread> consumers;
    for (size_t i = 0; i < NUM_CONSUMERS; ++i)
    {
        consumers.emplace_back(consumer_func);
    }

    for (size_t i = 0; i < NUM_PRODUCERS; ++i)
    {
        producers.at(i).join();
    }
    for (size_t i = 0; i < NUM_CONSUMERS; ++i)
    {
        consumers.at(i).join();
    }

    bool status = CheckValues(set.size(), expected, result) &&
                  CheckValues(queue.IsEmpty(), true, result);

    return status;
}

void RunTestsWaitableQueueMultithreaded()
{
    TestSuite suite;

    std::cout << COLOR_BLUE << "Running tests for WaitableQueue Multithread"
              << COLOR_RESET << std::endl;

    RunTest(suite, "RunTestWaitableQueuePushThanPop",
            RunTestWaitableQueuePopThanPush);
    RunTest(suite, "RunTestWaitableQueuePushThanPop",
            RunTestWaitableQueuePushThanPop);
    RunTest(suite, "RunTestWaitableQueueManyProducersOneConsumer",
            RunTestWaitableQueueManyProducersOneConsumer);
    RunTest(suite, "RunTestWaitableQueueOneProducerManyConsmers",
            RunTestWaitableQueueOneProducerManyConsmers);
    RunTest(suite, "RunTestWaitableQueueManyProducerManyConsmers",
            RunTestWaitableQueueManyProducerManyConsmers);

    suite.PrintSummary();
    std::cout << COLOR_BLUE << "Finished tests" << COLOR_RESET << std::endl;
}

static void RunTests()
{
    RunTestsWaitableQueueConstructors();
    RunTestsWaitableQueuePush();
    RunTestsWaitableQueueIsEmpty();
    RunTestsWaitableQueueMultithreaded();
}

int main()
{
    RunTests();

    return 0;
}
