#include <filesystem>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>

#include "dir_monitor.hpp"
#include "test_macros.hpp"

using namespace std;
using namespace test;

static constexpr const char* dirPath = "dir_monitor_test_dir"; // do menual
static constexpr const char* filePath = "dir_monitor_test_dir/new_file";

static void SetUp(void) {}

static void TearDown(void) {}

class Observer
{
  public:
    Observer() {}

    void SetPath(const std::string& event)
    {
        {
            lock_guard<mutex> lock(m_mtx);
            m_path = event;
        }

        cond.notify_all();
    }

    const std::string& GetPath() { return m_path; }

    condition_variable cond;
    std::mutex m_mtx;

  private:
    std::string m_path;
};

static void TestAddFileSmoke()
{
    TEST_START("TestAddFileSmoke");

    ilrd::DirMonitor monitor(dirPath);
    Observer obs;
    ilrd::Callback<const std::string&, Observer> callback(obs,
                                                          &Observer::SetPath);
    monitor.Subscribe(ilrd::DirMonitor::ADDED, &callback);
    monitor.Subscribe(ilrd::DirMonitor::CHANGED, &callback);

    monitor.Run();

    {
        unique_lock<mutex> lock(obs.m_mtx);
        ofstream newFile(filePath);
        while (!obs.cond.wait_for(lock, chrono::milliseconds(1), [&obs]
                                  { return (0 != obs.GetPath().length()); }))
        {
            TEST_ASSERT_TRUE(newFile.good());
            newFile << "Hello World!";
            newFile.close();
        }
    }
    TEST_ASSERT_EQUAL_STR(filePath, obs.GetPath().c_str());
    cout << "Res: " << obs.GetPath().c_str() << "\n";

    {
    }

    TEST_PASS();
}

int main()
{
    cout << "Testing dir_monitor...\n";
#ifndef NDEBUG
    cout << "Debugging!\n";
#endif
    ilrd::Handleton<ilrd::Logger>::GetInstance()->SetLevel(ilrd::Logger::DEBUG);
    TEST_RUN(TestAddFileSmoke);

    TEST_REPORT();
    return !(0 == tests_failed);
}
