#ifndef __ILRD_RD176_ASYNC_INJECTION__
#define __ILRD_RD176_ASYNC_INJECTION__

#include <ctime>      // std::chrono
#include <functional> // std::function

#include "scheduler.hpp" // scheduler

namespace ilrd
{

class AsyncInjection
{
  public:
    AsyncInjection(std::function<bool(void)> asyncFunc,
                   std::chrono::milliseconds interval);

    AsyncInjection(const AsyncInjection&) = delete;
    AsyncInjection& operator=(const AsyncInjection&) = delete;
    AsyncInjection(AsyncInjection&&) = delete;
    AsyncInjection& operator=(AsyncInjection&&) = delete;

  private:
    class AsyncInjectTask : public Scheduler::ISchedulerTask
    {
      public:
        AsyncInjectTask(AsyncInjection& async_injection);
        virtual void Execute() const;

      private:
        AsyncInjection& m_asyncTask;
    };

    void CleanUp();
    ~AsyncInjection() noexcept = default;

    std::function<bool(void)> m_asyncFunc; // true if done to work
    std::chrono::milliseconds m_interval;
    std::shared_ptr<AsyncInjectTask> m_task;
}; // class async_injection

} // namespace ilrd

#endif // __ILRD_RD176_ASYNC_INJECTION__