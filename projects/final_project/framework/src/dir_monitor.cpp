/*
Writer:  Robi
Checker: Maayan
Date:    01.07.2026
*/

#include <assert.h>      // assert
#include <logger.hpp>    // DEBUG_INFO
#include <poll.h>        // poll
#include <sys/inotify.h> // struct inotify_event
#include <unistd.h>      // read

#include "dir_monitor.hpp" // my api

namespace ilrd
{

DirMonitor::DirMonitor(const std::string& dirPath) : m_dirPath(dirPath) {}

DirMonitor::~DirMonitor()
{
    m_threadRun = false;

    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

void DirMonitor::Subscribe(DispatcherType dispatcher,
                           BaseCallback<const std::string&>* callback)
{
    m_dispatchers[dispatcher].Subscribe(callback);
}

void DirMonitor::Unsubscribe(DispatcherType dispatcher,
                             BaseCallback<const std::string&>* callback)
{
    m_dispatchers[dispatcher].Unsubscribe(callback);
}

void DirMonitor::Run()
{
    m_threadRun = true;
    m_thread = std::thread(&DirMonitor::Work, this);
}

static DirMonitor::DispatcherType GetEventType(struct inotify_event* event)
{
    assert(nullptr != event);

    DirMonitor::DispatcherType eventType =
        DirMonitor::DispatcherType::NUM_TYPES;

    if ((event->mask & IN_MOVED_TO) != 0U)
    {
        DEBUG_INFO("File was created!");
        eventType = DirMonitor::DispatcherType::ADDED;
    }

    else if ((event->mask & IN_CLOSE_WRITE) != 0U)
    {
        DEBUG_INFO("File was modified!");
        eventType = DirMonitor::DispatcherType::CHANGED;
    }

    else if ((event->mask & (IN_MOVED_FROM | IN_DELETE)) != 0U)
    {
        DEBUG_INFO("File was deleted!");
        eventType = DirMonitor::DispatcherType::REMOVED;
    }

    return eventType;
}

void DirMonitor::Work()
{
    const std::size_t NUM_FD = 1;
    const std::size_t TIMED_OUT_MS = 2000;
    const std::size_t BUFFER_SIZE = 1024 * (sizeof(inotify_event) + 16);

    char buf[BUFFER_SIZE]
        __attribute__((aligned(__alignof__(struct inotify_event)))) = {0};
    int monitor_fd = inotify_init1(IN_NONBLOCK);
    pollfd polledFd = {monitor_fd, POLLIN, 0};
    inotify_add_watch(monitor_fd, m_dirPath.c_str(),
                      IN_DELETE | IN_CLOSE_WRITE | IN_MOVED_FROM | IN_MOVED_TO);

    while (m_threadRun)
    {
        errno = 0;
        int res = ::poll(&polledFd, NUM_FD, TIMED_OUT_MS);

        if (-1 == res)
        {
            if (errno != EINTR)
            {
                LOG("Failed to poll on inotify fd!", Logger::LogLevel::ERROR);
                throw(std::system_error(errno, std::system_category(),
                                        "Failed to poll on inotifyfd"));
                return;
            }
        }
        else if (0 == res)
        {
            DEBUG_INFO("got timedout");
        }
        else
        {
            ssize_t bytesRead = ::read(monitor_fd, buf, BUFFER_SIZE);

            for (char* runner = buf; runner < buf + bytesRead;)
            {
                struct inotify_event* event =
                    reinterpret_cast<inotify_event*>(runner);

                DEBUG_INFO("Dir Monitor detected dir event");

                DispatcherType eventType = GetEventType(event);
                try
                {
                    m_dispatchers[eventType].NotifyAll(
                        m_dirPath + '/' + std::string(event->name));
                }
                catch (...)
                {
                    LOG("Subscriber threw exception", Logger::LogLevel::ERROR);
                }

                runner += sizeof(inotify_event) + event->len;
            }
        }
    }
}

} // namespace ilrd