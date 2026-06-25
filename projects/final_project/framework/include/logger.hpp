/*
Writer:  Robi
Checker: Max
Date:    25.06.2026
*/

#ifndef __ILRD_RD176_LOGGER__
#define __ILRD_RD176_LOGGER__

#include <atomic>  // atomic
#include <chrono>  // time
#include <fstream> // fstream
#include <string>  // string
#include <thread>  // thread

#include "handleton.hpp"
#include "waitable_queue.hpp"

#ifndef NDEBUG
#define DEBUG_INFO(MSG)                                                        \
    ilrd::Handleton<ilrd::Logger>::GetInstance()->Log(                         \
        MSG, ilrd::Logger::DEBUG, __FILE__, __LINE__)
#else
#define DEBUG_INFO(MSG) ((void)0)
#endif

#define LOG(MSG, LEVEL)                                                        \
    ilrd::Handleton<ilrd::Logger>::GetInstance()->Log(MSG, LEVEL, __FILE__,    \
                                                      __LINE__)

namespace ilrd
{

class Logger
{
  public:
    enum LogLevel
    {
        ERROR,
        INFO,
        DEBUG
    };

    Logger(const Logger& other) = delete;
    Logger(Logger&& other) = delete;
    Logger& operator=(const Logger& other) = delete;
    Logger& operator=(Logger&& other) = delete;

    void Log(const std::string& logMessage, LogLevel msgLevel,
             const std::string& file = __FILE__, int line = __LINE__);
    inline void SetLevel(LogLevel logLevel);

  private:
    Logger(const std::string& filePath = "./logger_Robi.log",
           LogLevel logLevel = ERROR);
    ~Logger();

    friend class Handleton<Logger>;

    static std::string GetTimeString(void);

    // TO DO: insert thread loop func (popps from waitable queue and prints)

    void PopFromQueueLog(void); // (independent thread is here in a loop till
                                // see the stop string in msg)

    std::atomic<LogLevel> m_logLevel;
    std::fstream m_log_file;
    WaitableQueue<std::string> m_queue;
    std::thread m_thread;
}; // Logger

inline void Logger::SetLevel(LogLevel logLevel) { m_logLevel = logLevel; }

} // namespace ilrd

#endif