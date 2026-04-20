#include "template.hpp"
#include <iostream>

using namespace std;

void NotFoo()
{
    int x = Max<int>(3, 7);
    cout << x << endl;
}