/*
Writer:  Robi
Checker: ?
date:    09.07.2026
*/

#include <assert.h>     // assert
#include <cerrno>       // errno
#include <signal.h>     // SIGEV_THREAD, struct sigevent
#include <system_error> // system_error
#include <time.h>       // timer_create

#include "scheduler.hpp" // my_api

namespace ilrd
{

bool Scheduler::ComparePriority::operator()(const SchedTaskPair& l,
                                            const SchedTaskPair& r)
{
    return (l.second > r.second);
}

Scheduler::Scheduler() : m_currentTopTask(nullptr)
{
    struct sigevent sev = {};

    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = &Scheduler::TimerCallback;
    sev.sigev_value.sival_ptr = this;
    sev.sigev_notify_attributes = nullptr;

    if (-1 == timer_create(CLOCK_MONOTONIC, &sev, &m_timer))
    {
        throw std::system_error(errno, std::generic_category(), "timer_create");
    }
}

Scheduler::~Scheduler() noexcept(true)
{
    struct itimerspec disarm = {};

    try
    {
        timer_settime(m_timer, 0, &disarm, nullptr);
        timer_delete(m_timer);
    }
    catch (...)
    {
    }
}

void Scheduler::Add(std::shared_ptr<ISchedulerTask> task,
                    std::chrono::milliseconds timeOfStart)
{
    assert(nullptr != task);
    assert(std::chrono::milliseconds::zero() <= timeOfStart);

    const auto startTime = std::chrono::steady_clock::now() + timeOfStart;
    SchedTaskPair tmp(std::move(task), startTime);

    std::unique_lock<std::mutex> mtx(m_mutex);
    if (nullptr == m_currentTopTask)
    {
        m_currentTopTask = std::make_shared<SchedTaskPair>(std::move(tmp));
        StartTimer(startTime);
    }
    else if (m_currentTopTask->second > startTime)
    {
        m_queue.Push(*m_currentTopTask);
        *m_currentTopTask = std::move(tmp);
        mtx.unlock();
        StartTimer(startTime);
    }
    else
    {
        mtx.unlock();
        m_queue.Push(tmp);
    }
}

void Scheduler::TimerCallback(union sigval sv)
{
    try
    {
        static_cast<Scheduler*>(sv.sival_ptr)->OnTimerEnd();
    }
    catch (...)
    { // empty on purpse
    }
}

void Scheduler::StartTimer(std::chrono::steady_clock::time_point startTime)
{
    static const long NSEC_IN_SEC = 1000000000L;

    struct itimerspec timerSpec = {};

    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
                  startTime - std::chrono::steady_clock::now())
                  .count();

    if (ns <= 0)
    {
        ns = 1;
    }

    timerSpec.it_value.tv_sec = static_cast<time_t>(ns / NSEC_IN_SEC);
    timerSpec.it_value.tv_nsec = static_cast<long>(ns % NSEC_IN_SEC);

    if (-1 == timer_settime(m_timer, 0, &timerSpec, nullptr))
    {
        throw std::system_error(errno, std::generic_category(),
                                "timer_settime");
    }
}

void Scheduler::OnTimerEnd()
{
    std::shared_ptr<ISchedulerTask> task;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (!m_currentTopTask)
        {
            return;
        }

        if (std::chrono::steady_clock::now() < m_currentTopTask->second)
        {
            StartTimer(m_currentTopTask->second);

            return;
        }

        task = m_currentTopTask->first;
        if (m_queue.IsEmpty())
        {
            m_currentTopTask.reset();
        }
        else
        {
            SchedTaskPair nextTask;
            m_queue.Pop(&nextTask);
            try
            {
                StartTimer(nextTask.second);
            }
            catch (...)
            {
                m_queue.Push(nextTask);
                throw;
            }

            *m_currentTopTask = std::move(nextTask);
        }
    }

    try
    {
        task->Execute();
    }
    catch (...)
    {
        // empty on purpse
    }
}

} // namespace ilrd