/*
Writer:  Robi
Checker: Chen
Date:    13.06.2026

comments:
for compile:

gdpp11 -I include/ src/thread_pool.cpp test/test_thread_pool.cpp
*/

#include <cassert> // assert

#include "thread_pool.hpp"

namespace ilrd
{

static thread_local bool g_s_is_cencel = false;

/*============================================================================*/
/*=============================== ThreadPool =================================*/
/*============================================================================*/

ThreadPool::ThreadPool(unsigned int num_of_threads)
{
    CreateThreads(num_of_threads);
}

ThreadPool::~ThreadPool() { Stop(); }

void ThreadPool::Add(const std::shared_ptr<IThreadPoolTask>& task,
                     Priority priority)
{
    m_queue.Push(TaskPair(task, priority));
}

void ThreadPool::Add(std::function<void(void)> task_func, Priority priority)
{
    m_queue.Push(TaskPair(std::make_shared<FuncTask>(task_func), priority));
}

void ThreadPool::AddBadApples(std::size_t numThreads, Priority priority)
{
    for (std::size_t i = 0; i < numThreads; ++i)
    {
        Add(std::make_shared<BadAppleTask>(BadAppleTask(*this)), priority);
    }

    m_numThreadOn -= numThreads;
}

void ThreadPool::Run()
{
    m_status = Status::RUNNING;
    m_condRun.notify_all();
}

void ThreadPool::Stop()
{
    m_status = Status::STOPPED;
    AddBadApples(m_numThreadOn, VERY_LOW);
    m_condRun.notify_all();
}

void ThreadPool::CreateThreads(std::size_t numThreads)
{
    for (std::size_t i = 0; i < numThreads; ++i)
    {
        ThreadPool::WorkerThread newThread(this);
        m_workerThreads.emplace(newThread.Getid(), std::move(newThread));
        /*
        emplace do that:
        // std::pair<std::thread::id, WorkerThread>
        //                                   pair(newThread.Getid(),
        // std::move(newThread));
        // m_workerThreads.insert(std::move(pair));

        but more efficient:
    ** pair+insert: 1 stack alloc, 1 heap alloc, 2 move Ctor, 1 Dtor.
    ** emplace: 0 stack alloc, 1 heap alloc, 1 move Ctor, 0 Dtor.
        */
    }

    m_numThreadOn += numThreads;
}

void ThreadPool::Pause()
{
    assert(m_status != ThreadPool::Status::STOPPED);

    m_status = Status::PAUSED;

    auto mock_func = []() {};
    for (std::size_t i = 0; i < m_numThreadOn; ++i)
    {
        Add(std::make_shared<FuncTask>(mock_func), VERY_HIGH);
    }
}

void ThreadPool::SetNumOfThread(std::size_t numThreads)
{
    assert(m_status != ThreadPool::Status::STOPPED);

    numThreads > m_numThreadOn
        ? CreateThreads(numThreads - m_numThreadOn)
        : AddBadApples(m_numThreadOn - numThreads, VERY_HIGH);
}

/*============================================================================*/
/*=============================== WorkerThread ===============================*/
/*============================================================================*/
ThreadPool::WorkerThread::WorkerThread(ThreadPool* threadPool)
    : m_thread(ThreadJob, threadPool)
{
    // empty on purpose
}

ThreadPool::WorkerThread::WorkerThread(WorkerThread&& other)
    : m_thread(std::move(other.m_thread))
{
    // empty on purpose
}

ThreadPool::WorkerThread::~WorkerThread()
{
    m_thread.joinable() ? m_thread.join() : void();
}

std::thread::id ThreadPool::WorkerThread::Getid() const
{
    return m_thread.get_id();
}

void ThreadPool::WorkerThread::ThreadJob(ThreadPool* threadPool)
{
    assert(NULL != threadPool);

    ThreadPool::Status& status = threadPool->m_status;

    std::unique_lock<std::mutex> lock(threadPool->m_mtx);
    while (!g_s_is_cencel)
    {
        threadPool->m_condRun.wait(
            lock, [&status]() { return ThreadPool::Status::PAUSED != status; });
        lock.unlock();

        TaskPair task;
        threadPool->m_queue.Pop(&task);
        task.first->Execute();
        lock.lock();
    }
}

/*============================================================================*/
/*=============================== FuncTask ===============================*/
/*============================================================================*/
FuncTask::FuncTask(std::function<void()> taskFunc) : m_taskFunc(taskFunc)
{
    // empty on purpose
}

void FuncTask::Execute() const { m_taskFunc(); }

/*============================================================================*/
/*=============================== BadAppleTask ===============================*/
/*============================================================================*/
ThreadPool::BadAppleTask::BadAppleTask(ThreadPool& threadPool)
    : m_tp(threadPool)
{
    // empty on purpose
}

void ThreadPool::BadAppleTask::Execute() const { g_s_is_cencel = true; }

/*============================================================================*/
/*=============================== TaskComparePriority ========================*/
/*============================================================================*/
bool ThreadPool::ComparePriority::operator()(const TaskPair& left,
                                             const TaskPair& right) const
{
    return (left.second < right.second);
}

} // namespace ilrd