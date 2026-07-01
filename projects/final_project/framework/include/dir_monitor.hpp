#ifndef __ILRD_RD176_DIR_MONITOR__
#define __ILRD_RD176_DIR_MONITOR__

#include <string>
#include <thread> // std::thread

#include "dispatcher.hpp"

namespace ilrd
{

class DirMonitor
{
  public:
    DirMonitor(const std::string& dirPath);
    ~DirMonitor() noexcept;

    DirMonitor(const DirMonitor&) = delete;
    DirMonitor& operator=(const DirMonitor&) = delete;
    DirMonitor(DirMonitor&&) = delete;
    DirMonitor& operator=(DirMonitor&&) = delete;

    enum DispatcherType : unsigned long
    {
        ADDED,
        CHANGED,
        REMOVED,
        NUM_TYPES
    };

    void Subscribe(DispatcherType dispatcher,
                   BaseCallback<const std::string&>* callback);
    void Unsubscribe(DispatcherType dispatcher,
                     BaseCallback<const std::string&>* callback);
    void Run(); // starts thread, m_threadRun -> true

  private:
    Dispatcher<const std::string&> m_dispatchers[DispatcherType::NUM_TYPES];
    std::string m_dirPath;
    bool m_threadRun = false;
    std::thread m_thread;

    void Work();

}; // class DirMonitor

} // namespace ilrd

#endif //__ILRD_RD176_DIR_MONITOR__