#include <iostream>
#include <sstream>
#include <string>

#include <thread>

#include "async_inject.hpp"
#include "handleton.hpp"
#include "scheduler.hpp"

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

class Task : public ilrd::Scheduler::ISchedulerTask
{
  public:
    Task(const std::string& message) : m_message(message) {}
    void Execute() const { std::cout << m_message << std::endl; }

  private:
    std::string m_message;
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

void RunTestSchedulerNormal()
{
    std::cout << COLOR_BLUE << "Running test for scheduler normal"
              << COLOR_RESET << std::endl;

    Task first("This is first task");
    Task last("this should print last");
    Task cut("this should come before last");

    ilrd::Scheduler* scheduler =
        ilrd::Handleton<ilrd::Scheduler>::GetInstance<>();

    scheduler->Add(std::make_shared<Task>(first),
                   std::chrono::milliseconds(500));
    scheduler->Add(std::make_shared<Task>(last),
                   std::chrono::milliseconds(2000));
    std::this_thread::sleep_for(std::chrono::seconds(1));

    scheduler->Add(std::make_shared<Task>(cut), std::chrono::milliseconds(500));
    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << COLOR_BLUE << "Finished tests" << COLOR_RESET << std::endl;
}
static int i = 0;
bool FirstAsync()
{
    std::cout << "I am the async first and this is my " << i << " iteration"
              << std::endl;
    ++i;

    if (i == 5)
    {
        i = 0;
        return true;
    }

    return false;
}

void RunTestSchedulerWithASyncInjection()
{
    std::cout << COLOR_BLUE << "Running test for scheduler async Injection"
              << COLOR_RESET << std::endl;

    new ilrd::AsyncInjection(FirstAsync, std::chrono::milliseconds(500));
    new ilrd::AsyncInjection(FirstAsync, std::chrono::milliseconds(500));
    std::this_thread::sleep_for(std::chrono::seconds(15));

    std::cout << COLOR_BLUE << "Finished tests" << COLOR_RESET << std::endl;
}

static void RunTests()
{
    RunTestSchedulerNormal();
    RunTestSchedulerWithASyncInjection();
}

int main()
{
    RunTests();

    return 0;
}
