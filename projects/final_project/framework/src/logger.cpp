#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "logger.hpp"

namespace ilrd
{
Logger::Logger(const std::string& filePath, LogLevel logLevel)
    : m_loggerIsBeingDestroyed(false), m_logLevel(logLevel),
      m_log_file(filePath, std::ios::out),
      m_thread(&Logger::PopFromQueueLog, this)
{
    if (!m_log_file.is_open())
    {
        std::cout << "Error opening log" << std::endl;
        exit(1);
    }
}

std::string Logger::GetTimeString(void)
{
    auto now = std::chrono::system_clock::now();
    std::time_t time_now = std::chrono::system_clock::to_time_t(now);

    std::tm local_tm;
    localtime_r(&time_now, &local_tm);

    std::stringstream strStream;
    strStream << std::put_time(&local_tm, "%Y::%m::%d::%H::%M::%S");

    return strStream.str();
}

void Logger::Log(const std::string& logMessage, // msg
                 LogLevel msgLevel,             // msgLevel
                 const std::string& file,       // file from which command came
                 int line                       // line in the file
)
{
    if (m_logLevel < msgLevel)
    {
        return;
    }

    std::string finalStr;

    finalStr += Logger::GetTimeString() + "\n";
    finalStr += file + ": " + std::to_string(line) + "\n";
    finalStr += logMessage + "\n\n";

    m_queue.Push(finalStr);
}

Logger::~Logger()
{
    m_loggerIsBeingDestroyed = true;
    m_queue.Push("");
    m_thread.join();
    m_log_file.close();
}

void Logger::PopFromQueueLog(void) // from thread
{
    std::string msg = "x";
    while ("" != msg)
    {
        m_queue.Pop(&msg);
        m_log_file << msg;
        m_log_file.flush();
        (void)0;
    }
}

} // namespace ilrd
