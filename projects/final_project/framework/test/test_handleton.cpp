#include "Tester.hpp"
#include "handleton.hpp"
#include <dlfcn.h>
#include <iostream>

const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";

int main()
{
    using namespace ilrd;
    Tester* t = Handleton<Tester>::GetInstance(10);
    std::cout << "main process singleton return address: " << t << std::endl;

    void* handle = dlopen("bin/debug/libplugin2.so", RTLD_LAZY);

    typedef Tester* (*plugin_t)();
    plugin_t Handleton_plugin =
        reinterpret_cast<plugin_t>(dlsym(handle, "Handleton_plugin"));

    std::cout << "======================================================"
              << std::endl;
    std::cout << "----------- Result: -----------" << std::endl;
    std::string res = Handleton_plugin() == t ? GREEN + "Pass" : RED + "Fail";
    std::cout << res + RESET << std::endl;
    std::cout << "======================================================"
              << std::endl;

    dlclose(handle);
}
