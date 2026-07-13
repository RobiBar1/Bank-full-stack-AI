/*
Writer:  Robi
Checker: Ehud
Date:    13.07.2026
*/

#include "async_inject.hpp" // my api
#include "scheduler.hpp"    // GetInstance

namespace ilrd
{

AsyncInjection::AsyncInjection(std::function<bool(void)> asyncFunc,
                               std::chrono::milliseconds interval)
    : m_asyncFunc(asyncFunc), m_interval(interval)
{
    m_task = std::make_shared<AsyncInjectTask>(*this);
    Handleton<Scheduler>::GetInstance()->Add(m_task, m_interval);
}

AsyncInjection::AsyncInjectTask::AsyncInjectTask(
    AsyncInjection& async_injection)
    : m_asyncTask(async_injection)
{ // empty on purpse
}

void AsyncInjection::CleanUp() { delete this; }

void AsyncInjection::AsyncInjectTask::Execute() const
{
    if (m_asyncTask.m_asyncFunc())
    {
        m_asyncTask.CleanUp();
    }
    else
    {
        Handleton<Scheduler>::GetInstance()->Add(m_asyncTask.m_task,
                                                 m_asyncTask.m_interval);
    }
}
} // namespace ilrd