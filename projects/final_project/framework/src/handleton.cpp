#define IMPLEMENTER_MODE

#include "handleton.hpp"  //API
#include "Tester.hpp"     //tester
#include "dll_loader.hpp" // DLLLoader
#include "logger.hpp"     //Logger, LogLevel

// #include ".hpp" //

namespace ilrd
{
template Tester* Handleton<Tester>::GetInstance<int>(int&&);
template DLLLoader* Handleton<DLLLoader>::GetInstance<>();
template Logger*
Handleton<Logger>::GetInstance<const std::string&, Logger::LogLevel&&>(
    const std::string&, Logger::LogLevel&& logLevel);
template Logger* Handleton<Logger>::GetInstance<>();
} // namespace ilrd
