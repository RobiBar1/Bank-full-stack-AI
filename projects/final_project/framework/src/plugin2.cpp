#include "Tester.hpp"
#include "handleton.hpp"

/*
2) create a shared dynamic libary for the plugin
 g++ -fPIC -c src/plugin2.cpp -I include/ -o plugin2.o
 g++ -shared plugin2.o -o libplugin2.so

*/
namespace ilrd
{
extern "C"
{
    Tester* Handleton_plugin()
    {
        return ilrd::Handleton<Tester>::GetInstance(5);
    }
}
}