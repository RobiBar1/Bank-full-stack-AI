/*
for test it need the file dll_test_plugin in the src dir, make him so, then run
the test and drag the .so into the dir that monitoring.
then if pass -> will print "Hello from init!".
then delete the file from there.
then if pass -> will print "Hello from dtor!".
*/

#include <iostream>

#include "dir_monitor.hpp"
#include "dll_loader.hpp"
#include "handleton.hpp"
#include "logger.hpp"
#include "test_macros.hpp"

using namespace std;
using namespace test;

static void SetUp(void) {}

static void TearDown(void) {}

static void SmokeTest()
{
    DEBUG_INFO("start test");
    ilrd::DirMonitor monitor("dir_monitor_test_dir/");
    ilrd::DLLLoader* loader = ilrd::Handleton<ilrd::DLLLoader>::GetInstance();
    ilrd::Callback<const std::string&, ilrd::DLLLoader> loadCallback(
        *loader, &ilrd::DLLLoader::CallbackLoad);
    ilrd::Callback<const std::string&, ilrd::DLLLoader> unloadCallback(
        *loader, &ilrd::DLLLoader::CallbackUnload);
    ilrd::Callback<const std::string&, ilrd::DLLLoader> modifyCallback(
        *loader, &ilrd::DLLLoader::CallbackModify);
    monitor.Subscribe(ilrd::DirMonitor::ADDED, &loadCallback);
    monitor.Subscribe(ilrd::DirMonitor::REMOVED, &unloadCallback);
    monitor.Subscribe(ilrd::DirMonitor::CHANGED, &modifyCallback);

    monitor.Run();

    std::string input;
    while ("q" != input)
    {
        cin >> input;
    }
}

int main()
{
    cout << "Testing dll_loader...\n";
    ilrd::Handleton<ilrd::Logger>::GetInstance()->SetLevel(ilrd::Logger::DEBUG);
    TEST_RUN(SmokeTest);
    return 0;
}
