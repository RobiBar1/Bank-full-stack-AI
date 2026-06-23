#include <iostream>
#include <sstream>
#include <string>

#include "Singleton.hpp"
#include "thread_pool.hpp"

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

bool RunTestSingletonGetInstanceOnce(std::stringstream& result)
{
    ilrd::ThreadPool* thread_pool =
        ilrd::Singleton<ilrd::ThreadPool>::GetInstance();

    static_cast<void>(thread_pool);
    static_cast<void>(result);

    return true;
}

bool RunTestSingletonGetInstanceTwice(std::stringstream& result)
{
    ilrd::ThreadPool* thread_pool1 =
        ilrd::Singleton<ilrd::ThreadPool>::GetInstance();
    ilrd::ThreadPool* thread_pool2 =
        ilrd::Singleton<ilrd::ThreadPool>::GetInstance();

    bool status = CheckValues(thread_pool1, thread_pool2, result);

    return status;
}

bool RunTestSingletonGetInstanceMultithread(std::stringstream& result)
{
    const std::size_t NUM_THREADS = 10;
    ilrd::ThreadPool* thread_pool =
        ilrd::Singleton<ilrd::ThreadPool>::GetInstance();
    std::vector<ilrd::ThreadPool*> expected(NUM_THREADS, thread_pool);
    std::vector<ilrd::ThreadPool*> res(NUM_THREADS, nullptr);
    std::vector<std::thread> threads;
    std::atomic<std::size_t> counter(0);
    std::size_t expected_counter = NUM_THREADS;
    ilrd::FuncTask increment([&counter] { ++counter; });

    for (std::size_t i = 0; i < NUM_THREADS; ++i)
    {
        threads.emplace_back(std::thread(
            [&res, &counter, &increment, i]
            {
                res[i] = ilrd::Singleton<ilrd::ThreadPool>::GetInstance();
                res[i]->Add(std::make_shared<ilrd::FuncTask>(increment));
            }));
    }

    thread_pool->Run();

    thread_pool->Stop();
    for (std::size_t i = 0; i < NUM_THREADS; ++i)
    {
        threads[i].join();
    }

    bool status = true;
    for (size_t i = 0; i < NUM_THREADS && true == status; ++i)
    {
        status = CheckValues(res[i], expected[i], result);
    }

    status = CheckValues(counter.load(), expected_counter, result);

    return status;
}

void RunTestSingletonGetInstance()
{
    TestSuite suite;

    std::cout << COLOR_BLUE << "Running tests for Singleton GetInstance"
              << COLOR_RESET << std::endl;

    RunTest(suite, "RunTestSingletonGetInstanceOnce",
            RunTestSingletonGetInstanceOnce);
    RunTest(suite, "RunTestSingletonGetInstanceTwice",
            RunTestSingletonGetInstanceTwice);
    RunTest(suite, "RunTestSingletonGetInstanceMultithread",
            RunTestSingletonGetInstanceMultithread);

    suite.PrintSummary();
    std::cout << COLOR_BLUE << "Finished tests" << COLOR_RESET << std::endl;
}

static void RunTests() { RunTestSingletonGetInstance(); }

int main()
{
    RunTests();

    return 0;
}
