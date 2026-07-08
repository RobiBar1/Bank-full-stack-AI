#ifndef __ILRD_RD176_SCHEDULER__
#define __ILRD_RD176_SCHEDULER__

#include <chrono>  //chrono
#include <ctime>   //timer_t
#include <mutex>   //mutex
#include <utility> //pair

#include "handleton.hpp"      // handleton
#include "priority_queue.hpp" // PriorityQueue
#include "waitable_queue.hpp" // waitable_queue

namespace ilrd
{

class Scheduler
{
  public:
    class ISchedulerTask
    {
      public:
        virtual void Execute() const = 0;
        virtual ~ISchedulerTask() noexcept = default;
    };

    ~Scheduler() noexcept;

    Scheduler(const Scheduler&) = delete;
    Scheduler& operator=(const Scheduler&) = delete;
    Scheduler(Scheduler&&) = delete;
    Scheduler& operator=(Scheduler&&) = delete;

    void Add(std::shared_ptr<ISchedulerTask> task,
             std::chrono::milliseconds timeOfStart);

  private:
    Scheduler();
    friend class Handleton<Scheduler>;

    using SchedTaskPair =
        std::pair<std::shared_ptr<ISchedulerTask>,
                  std::chrono::steady_clock::time_point>; // add wrapper
    class ComparePriority
    {
      public:
        bool operator()(const SchedTaskPair& l, const SchedTaskPair& r);
    };
    WaitableQueue<SchedTaskPair,
                  PriorityQueue<SchedTaskPair, std::vector<SchedTaskPair>,
                                ComparePriority>>
        m_queue;
    std::shared_ptr<SchedTaskPair> m_currentTopTask;
    timer_t m_timer;
    std::mutex m_mutex; // handle m_current_top_task

    // private functions
    static void TimerCallback(union sigval sv); // insert func in timer_create
                                                // signal - used in start timer;
    void StartTimer(std::chrono::steady_clock::time_point);
    void OnTimerEnd(); // run the m_current_top_task called by timercallback

}; // class scheduler

} // namespace ilrd

#endif
