#include <cstring> // strcmp
#include <iostream>
#include <string>
#include <sys/epoll.h>
#include <sys/inotify.h>
#include <unistd.h>

#include <fstream>
#include <sstream>

#include "logger.hpp"

using namespace ilrd;

/*

g++ -fPIC -c ../src/handleton.cpp -I ../include -o handleton.o
g++ -fPIC -c  ../src/logger.cpp -I ../include -o logger.o
g++ -shared handleton.o logger.o -o libhandleton.so

g++ ./test_logger.cpp ../src/logger.cpp -I ../include/ -o test_logger.out -L./
-lhandleton -Wl,-rpath,./

./test_logger.out

valgrind --leak-check=yes --track-origins=yes ./test_logger.out

*/

const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";

typedef enum
{
    TEST_STATUS_SUCCESS,
    TEST_STATUS_FAILURE
} test_status_t;

static test_status_t Logger_BasicTest(void);

static void CheckTest(test_status_t result, std::string name)
{
    if (result == TEST_STATUS_SUCCESS)
    {
        std::cout << "-> " << name << ": " << GREEN << "PASSED\n\n" << RESET;
    }
    else
    {
        std::cout << "-> " << name << ": " << RED << "FAILED\n\n" << RESET;
    }
}

int main()
{
    std::cout << "\n ---- TESTS ----\n\n";

    CheckTest(Logger_BasicTest(), "Logger_BasicTest");

    return 0;
}

Logger* logger_handleton = Handleton<ilrd::Logger>::GetInstance();

static test_status_t Logger_BasicTest(void)
{
    logger_handleton->SetLevel(Logger::LogLevel::DEBUG);

    DEBUG_INFO("hello mars"); // shows

    DEBUG_INFO("setting up radars..."); // shows

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    DEBUG_INFO("radars set"); // shows

    logger_handleton->SetLevel(Logger::LogLevel::INFO);

    DEBUG_INFO("setting up radio from earth"); // doesn't show

    LOG("Error: some radio did static and didn't receive",
        Logger::LogLevel::ERROR); // shows

    LOG("Radio is from earth", Logger::LogLevel::INFO);

    logger_handleton->SetLevel(Logger::LogLevel::DEBUG);

    DEBUG_INFO("fixed earth radio"); // shows

    DEBUG_INFO("finished setting up mars base"); // shows

    logger_handleton->SetLevel(Logger::LogLevel::DEBUG);

    return TEST_STATUS_SUCCESS;
}
