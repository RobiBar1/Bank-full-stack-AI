#include <iostream>

#include "shred_pointer.hpp"

using namespace ilrd;

class x
{
  public:
    int m_x;
    int m_y;
};

class y : public x
{
    float m_w;
};

int main()
{
    SharedPointer<y> p(new y());

    (*p).m_x = 6;
    std::cout << (*p).m_x << std::endl;

    std::cout << p->m_x << std::endl;
    std::cout << p->m_y << std::endl;

    SharedPointer<x> h(p);

    std::cout << p->m_x << std::endl;

    p = p;

    std::cout << p->m_x << std::endl;

    return 0;
}