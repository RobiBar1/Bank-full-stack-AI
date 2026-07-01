#include <iostream>

void init() __attribute__((constructor));
void death() __attribute__((destructor));

void init() { std::cout << "Hello from init!" << std::endl; }

void death() { std::cout << "Hello from dtor" << std::endl; }

int main() { std::cout << "Hello from main!" << std::endl; }
