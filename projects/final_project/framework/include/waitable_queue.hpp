/*
Writer:  Robi
Checker: Omer
Date:    11.06.2026
*/

#ifndef __ILRD_RD176_WAITABLE_QUEUE__
#define __ILRD_RD176_WAITABLE_QUEUE__

#include <chrono>             // milliseconds
#include <condition_variable> // std::condition_variable_any
#include <mutex>              // mutex, unique_lock, defer_lock_t

#include "priority_queue.hpp"

namespace ilrd
{

/* concepts:

    'T' must be copyable and assignable

    The 'Container' must satisfy the
    requirement SequenceContainer,same
    constraint as std::queue:

    * push()
    * pop()
    * front()
    * empty()
*/

template <class T, class Container = std::queue<T>> class WaitableQueue
{
  public:
    WaitableQueue() = default;
    ~WaitableQueue() = default;
    WaitableQueue(const WaitableQueue& other) = delete;
    void operator=(const WaitableQueue& other) = delete;

    void Push(const T& newElem);
    void Pop(T* outParam);
    bool Pop(T* outParam, std::chrono::milliseconds timeout);
    bool IsEmpty() const;

  private:
    Container m_container;
    mutable std::timed_mutex m_mtx;
    std::condition_variable_any m_condNotEmpty;
}; // class WaitableQueue

template <class T, class Container>
void WaitableQueue<T, Container>::Push(const T& newElem)
{
    std::unique_lock<std::timed_mutex> lock(m_mtx);
    m_container.push(newElem);
    lock.unlock();
    m_condNotEmpty.notify_one();
}

template <class T, class Container>
void WaitableQueue<T, Container>::Pop(T* outParam)
{
    std::unique_lock<std::timed_mutex> lock(m_mtx);

    m_condNotEmpty.wait(lock, [this] { return !m_container.empty(); });

    *outParam = m_container.front();

    m_container.pop();
}

template <class T, class Container>
bool WaitableQueue<T, Container>::Pop(T* outParam,
                                      std::chrono::milliseconds timeout)
{
    std::chrono::time_point<std::chrono::system_clock> timeout_time =
        std::chrono::system_clock::now() + timeout;
    std::unique_lock<std::timed_mutex> lock(m_mtx, std::defer_lock_t());

    if (!lock.try_lock_until(timeout_time) ||
        !m_condNotEmpty.wait_until(lock, timeout_time,
                                   [this] { return !m_container.empty(); }))
    {
        return false;
    }

    *outParam = m_container.front();
    m_container.pop();

    return true;
}

template <class T, class Container>
bool WaitableQueue<T, Container>::IsEmpty() const
{
    std::unique_lock<std::timed_mutex> lock(m_mtx);

    return m_container.empty();
}
} // namespace ilrd

#endif // __ILRD_RD176_WAITABLE_QUEUE__
