#define IMPLEMENTER_MODE

#include "handleton.hpp" //API
#include "Tester.hpp"

/*
1) create a libary for handleton and the included classes
g++ -fPIC -c src/handleton.cpp  -I include/ -o handleton.o
g++ -shared handleton.o -o libhandleton.so

*/

/*

g++ -fPIC -c src/handleton.cpp -I include/ -o handleton.o
g++ -fPIC -c  src/logger.cpp -I include/ -o logger.o
g++ -shared handleton.o logger.o -o libhandleton.so

*/
namespace ilrd
{
template Tester* Handleton<Tester>::GetInstance<int>(int&&);
}
// template Logger* Handleton<Logger>::GetInstance<>();
// template Logger*
// Handleton<Logger>::GetInstance<const std::string&, Logger::LogLevel&&>(
//     const std::string&, Logger::LogLevel&&);
