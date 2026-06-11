#ifndef __ILRD_RD176_THREAD_POOL__
#define __ILRD_RD176_THREAD_POOL__

#include <functional>    // function
#include <future>        // future
#include <thread>        // thread
#include <unordered_map> // unordered_map
#include <utility>       // pair

#include "priority_queue.hpp"
#include "waitable_queue.hpp"

#if __cplusplus >= 201103L
// noexcept is defined, do nothing
#else
#define noexcept throw()
#endif // #if cplusplus >= 201103L

namespace ilrd
{

enum Priority
{
    LOW = 1,
    MEDIUM = 2,
    HIGH = 3
};

class ThreadPool
{
  public:
    class IThreadPoolTask
    {
      public:
        virtual void Execute() const = 0;
        virtual ~IThreadPoolTask() = default;

      private:
        Priority m_priority;
    };

    ThreadPool(
        unsigned int num_of_threads = std::thread::hardware_concurrency());
    ~ThreadPool() noexcept;
    ThreadPool(const ThreadPool& other) = delete;
    ThreadPool& operator=(const ThreadPool& other) = delete;
    ThreadPool(ThreadPool&& other) = delete;
    ThreadPool& operator=(ThreadPool&& other) = delete;

    void Add(const std::shared_ptr<IThreadPoolTask>& task,
             Priority priority = MEDIUM);
    void Run();
    void Stop();  // finish all queued tasks and stop
    void Pause(); // finish all running tasks and stop (dont empty queue)
    void SetNumOfThread(std::size_t numThreads);

  private:
    static const Priority VERY_LOW = static_cast<Priority>(LOW - 1);
    static const Priority VERY_HIGH = static_cast<Priority>(HIGH + 1);

    class WorkerThread
    {
      public:
        WorkerThread(ThreadPool& thread_pool);
        ~WorkerThread();
        size_t GetTid();

      private:
        ThreadPool& m_thread_pool;
        std::thread m_thread;
        bool m_isTerminated = false:
        // TODO: add private members
    };

    enum class Status
    {
        RUNNING,
        PAUSED,
        STOPPED,
    };

    // class StopTask : public IThreadPoolTask
    // class KillTask : public IThreadPoolTask
    using TaskPair = std::pair<std::shared_ptr<IThreadPoolTask>, Priority>;
    WaitableQueue<TaskPair, PriorityQueue<TaskPair>> m_queue;
    std::unordered_map<std::size_t, WorkerThread> m_workerThreads;
    std::condition_variable_any m_condRun;
    std::mutex m_mtx;
    enum class Status m_status = PAUSED;

}; // end ThreadPool

class FuncTask : public ThreadPool::IThreadPoolTask
{
  public:
    FuncTask(std::function<void(void)> taskFunc);
    ~FuncTask() = default;
    void Execute() const;
    inline operator IThreadPoolTask*();
    inline operator std::shared_ptr<IThreadPoolTask>(); // new conversion
  private:
    std::function<void(void)> m_task_func;
};

class FutureFunc : public ThreadPool::IThreadPoolTask
{
  public:
    std::future<std::size_t> Get(); // returns future obj
};

} // namespace ilrd

#endif // __ILRD_RD176_THREAD_POOL__j