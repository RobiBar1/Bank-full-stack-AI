#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <typeinfo>

using namespace std;

/* solve for g.
extern "C"
{
    int hey()
    {
        printf("hey");

        return 0;
    }
}*/

int hey()
{
    printf("hey");

    return 0;
}

double hey(int a)
{
    printf("%d", a);

    return 0;
}
void hello() { printf("hello"); }

/*
robi@robi-pc:~/git/cpp$ gdpp98 -c shared_librart.cpp
robi@robi-pc:~/git/cpp$ gdpp98 shared_librart.o --shared -o shaerd.so
robi@robi-pc:~/git/cpp$ nm shaerd.so
*/