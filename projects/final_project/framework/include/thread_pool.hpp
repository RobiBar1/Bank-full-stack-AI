#ifndef __ILRD_RD176_THREAD_POOL__
#define __ILRD_RD176_THREAD_POOL__

#include <functional>    // function
#include <future>        // future
#include <thread>        // thread
#include <unordered_map> // unordered_map
#include <utility>       // pair

#include "priority_queue.hpp"
#include "waitable_queue.hpp"

#define noexcept throw()

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
    void Add(std::function<void(void)> task_func, Priority priority = MEDIUM);

    void Run();
    void Stop();  // finish all queued tasks and stop
    void Pause(); // finish all running tasks and stop (dont empty queue)
    void SetNumOfThread(std::size_t numThreads);

  private:
    static const Priority VERY_LOW = static_cast<Priority>(LOW - 1);
    static const Priority VERY_HIGH = static_cast<Priority>(HIGH + 1);
    using TaskPair = std::pair<std::shared_ptr<IThreadPoolTask>, Priority>;

    class WorkerThread
    {
      public:
        WorkerThread(ThreadPool* tp);
        WorkerThread(WorkerThread&& other);
        ~WorkerThread();

        std::thread::id Getid() const;

      private:
        static void ThreadJob(ThreadPool* tp);
        std::thread m_thread;
    }; // end WorkerThread

    class BadAppleTask : public ThreadPool::IThreadPoolTask
    {
      public:
        BadAppleTask(ThreadPool& tp);
        ~BadAppleTask() = default;

        void Execute() const;

      private:
        ThreadPool& m_tp;
    }; // end BadAppleTask

    void CreateThreads(std::size_t numThreads);
    void AddBadApples(std::size_t numBadApples, Priority prio);

    enum class Status
    {
        RUNNING,
        PAUSED,
        STOPPED,
    }; // end Status

    class ComparePriority
    {
      public:
        bool operator()(const TaskPair& left, const TaskPair& right) const;
    }; // end ComparePriority

    WaitableQueue<TaskPair, PriorityQueue<TaskPair, std::vector<TaskPair>,
                                          ComparePriority>>
        m_queue;
    std::unordered_map<std::thread::id, WorkerThread> m_workerThreads;
    std::condition_variable_any m_condRun;
    std::mutex m_mtx;
    enum Status m_status = Status::PAUSED;
    std::size_t m_numThreadOn = 0;

}; // end ThreadPool

class FuncTask : public ThreadPool ::IThreadPoolTask
{
  public:
    FuncTask(std::function<void(void)> taskFunc);
    ~FuncTask() = default;
    void Execute() const;

  private:
    std::function<void(void)> m_taskFunc;
}; // end FuncTask

// class FutureFunc : public ThreadPool::IThreadPoolTask
// {
//   public:
//     std::future<std::size_t> Get(); // returns future obj
// };

} // namespace ilrd

#endif // __ILRD_RD176_THREAD_POOL__j