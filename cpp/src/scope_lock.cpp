#include <chrono>
#include <iostream>
#include <mutex>

#include "scope_lock.hpp"

int main()
{
    std::mutex mut;

    ilrd::ScopeLock<std::mutex> hey(mut);

    std::cout << "hey\n";

    return 0;
}