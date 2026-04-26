#include <iostream> // iostream

#include "complex.hpp" // my_api

ilrd::Complex::Complex(double real, double imaginary)
    : m_real(real), m_imaginary(imaginary)
{
}

std::ostream& ilrd::operator<<(std::ostream& out, const ilrd::Complex& num)
{
    return out << "real num: " << num.GetReal()
               << " and img num: " << num.GetImg() << std::endl;
}

std::istream& ilrd::operator>>(std::istream& in, ilrd::Complex& num)
{
    double real = 0;
    double img = 0;

    in >> real >> img;

    num.SetReal(real);
    num.SetImg(img);

    return in;
}