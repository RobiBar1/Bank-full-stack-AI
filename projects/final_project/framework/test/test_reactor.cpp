#include <iostream>
#include <sstream>
#include <string>
#include <sys/epoll.h>
#include <sys/inotify.h>
#include <unistd.h>

#include "reactor.hpp"

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

class EpollListener : public ilrd::Reactor::IListener
{
  private:
    void RegisterListen(int epollFd, int fd, ilrd::Reactor::Mode mode)
    {
        struct epoll_event ev;
        if (ilrd::Reactor::Mode::READ == mode)
        {
            ev.events = EPOLLIN | EPOLLET;
        }
        else
        {
            ev.events = EPOLLOUT | EPOLLET;
        }
        ev.data.fd = fd;
        epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &ev);
    }

    void
    RegisterAllListens(int epollFd,
                       const std::vector<ilrd::Reactor::CallbackPair>& listenTo)
    {
        for (const auto& pair : listenTo)
        {
            RegisterListen(epollFd, pair.first, pair.second);
        }
    }

  public:
    virtual std::vector<ilrd::Reactor::CallbackPair>
    Listen(const std::vector<ilrd::Reactor::CallbackPair>& listenTo)
    {
        int epollFd = epoll_create1(0);
        RegisterAllListens(epollFd, listenTo);
        std::vector<struct epoll_event> events(10);

        int nfds = epoll_wait(epollFd, events.data(), events.size(), -1);
        events.resize(nfds);

        std::vector<ilrd::Reactor::CallbackPair> toHandle;

        for (auto& event : events)
        {
            int fd = event.data.fd;
            if (event.events & EPOLLIN)
            {
                std::cout << "Need to handle: " << fd << " which is a read"
                          << std::endl;
                toHandle.push_back(
                    std::make_pair(fd, ilrd::Reactor::Mode::READ));
            }
            else
            {
                std::cout << "Need to handle: " << fd << " which is a write"
                          << std::endl;
                toHandle.push_back(
                    std::make_pair(fd, ilrd::Reactor::Mode::WRITE));
            }
        }
        close(epollFd);

        return toHandle;
    }
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

bool RunTestsReactorConstructorSmokeTest(std::stringstream& result)
{

    ilrd::Reactor reactor(std::unique_ptr<EpollListener>(new EpollListener()));

    static_cast<void>(result);
    static_cast<void>(reactor);

    return true;
}

void RunTestsReactorConstructor()
{
    TestSuite suite;

    std::cout << COLOR_BLUE << "Running tests for Reactor Constructor"
              << COLOR_RESET << std::endl;

    RunTest(suite, "RunTestsReactorConstructorSmokeTest",
            RunTestsReactorConstructorSmokeTest);

    suite.PrintSummary();
    std::cout << COLOR_BLUE << "Finished tests" << COLOR_RESET << std::endl;
}

void StopReactor(int fd, ilrd::Reactor::Mode mode, ilrd::Reactor* const reactor)
{
    reactor->Stop();

    static_cast<void>(fd);
    static_cast<void>(mode);
}

bool RunTestReactorRun(std::stringstream& result)
{
    ilrd::Reactor reactor(std::unique_ptr<EpollListener>(new EpollListener()));
    auto read_from = [](int fd, ilrd::Reactor::Mode mode)
    {
        char buffer[1024];

        read(fd, buffer, 1024);

        std::cout << "Got string: " << buffer << " from fd: " << fd
                  << std::endl;

        static_cast<void>(mode);
    };
    using namespace std::placeholders;
    auto stop = std::bind(StopReactor, _1, _2, &reactor);

    int inotifyFd = inotify_init1(IN_NONBLOCK);
    inotify_add_watch(inotifyFd, "/tmp/temp.txt", IN_MODIFY);

    reactor.Add(STDIN_FILENO, ilrd::Reactor::Mode::READ, stop);
    reactor.Add(inotifyFd, ilrd::Reactor::Mode::READ, read_from);

    reactor.Run();
    static_cast<void>(result);

    return true;
}

void RunTestsReactorRun()
{
    TestSuite suite;

    std::cout << COLOR_BLUE << "Running tests for Reactor Run" << COLOR_RESET
              << std::endl;

    RunTest(suite, "RunTestsReactorRun", RunTestReactorRun);

    suite.PrintSummary();
    std::cout << COLOR_BLUE << "Finished tests" << COLOR_RESET << std::endl;
}

static void RunTests()
{
    RunTestsReactorConstructor();
    RunTestsReactorRun();
}

int main()
{
    RunTests();

    return 0;
}
