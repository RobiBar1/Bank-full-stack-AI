#ifndef __ILRD_RD176_COMPLEX_HPP__
#define __ILRD_RD176_COMPLEX_HPP__

#include <cmath>  // std::abs
#include <iosfwd> // ostream forward declaration only

namespace ilrd
{
class Complex
{
  public:
    Complex(double real = 0, double imaginary = 0); // no explicit on purpose

    inline Complex& operator+=(const Complex& other);
    inline Complex& operator-=(const Complex& other);
    inline Complex& operator*=(const Complex& other);
    inline Complex& operator/=(const Complex& other);

    inline double GetReal() const;
    inline double GetImg() const;

    inline void SetReal(double new_real);
    inline void SetImg(double new_img);

  private:
    double m_real;
    double m_imaginary;
};

inline bool operator!=(const Complex& left, const Complex& right);
inline bool operator==(const Complex& left, const Complex& right);
inline const Complex operator+(const Complex& left, const Complex& right);
inline const Complex operator-(const Complex& left, const Complex& right);
inline const Complex operator*(const Complex& left, const Complex& right);
inline const Complex operator/(const Complex& left, const Complex& right);

std::ostream& operator<<(std::ostream& out, const Complex& num);
std::istream& operator>>(std::istream& in, Complex& num);
} // namespace ilrd

inline double ilrd::Complex::GetReal() const { return m_real; }

inline double ilrd::Complex::GetImg() const { return m_imaginary; }

inline void ilrd::Complex::SetReal(double new_real) { m_real = new_real; }
inline void ilrd::Complex::SetImg(double new_img) { m_imaginary = new_img; }

inline bool ilrd::operator!=(const Complex& left, const Complex& right)
{
    return !(left == right);
}

inline bool ilrd::operator==(const Complex& left, const Complex& right)
{
    return (std::abs(left.GetImg() - right.GetImg()) < 1e-9 &&
            std::abs(left.GetReal() - right.GetReal()) < 1e-9);
}

inline const ilrd::Complex ilrd::operator+(const Complex& left,
                                           const Complex& right)
{
    return (ilrd::Complex(left.GetReal() + right.GetReal(),
                          left.GetImg() + right.GetImg()));
}

inline const ilrd::Complex ilrd::operator-(const Complex& left,
                                           const Complex& right)
{
    return (ilrd::Complex(left.GetReal() - right.GetReal(),
                          left.GetImg() - right.GetImg()));
}

inline const ilrd::Complex ilrd::operator*(const Complex& left,
                                           const Complex& right)
{
    double a = left.GetReal();
    double b = left.GetImg();
    double c = right.GetReal();
    double d = right.GetImg();

    // https://en.wikipedia.org/wiki/Complex_number
    return (ilrd::Complex(a * c - (b * d), a * d + b * c));
}

inline const ilrd::Complex ilrd::operator/(const Complex& left,
                                           const Complex& right)
{
    double a = left.GetReal();
    double b = left.GetImg();
    double c = right.GetReal();
    double d = right.GetImg();
    double img = 0;
    double real = 0;

    /*https://www.geeksforgeeks.org/maths/complex-number-formula/*/
    real = (a * c + b * d) / (c * c + d * d);
    img = (b * c - a * d) / (c * c + d * d);

    return (ilrd::Complex(real, img));
}

inline ilrd::Complex& ilrd::Complex::operator+=(const ilrd::Complex& other)
{
    this->SetReal(this->GetReal() + other.GetReal());
    this->SetImg(this->GetImg() + other.GetImg());

    return *this;
}

inline ilrd::Complex& ilrd::Complex::operator-=(const ilrd::Complex& other)
{
    this->SetReal(this->GetReal() - other.GetReal());
    this->SetImg(this->GetImg() - other.GetImg());

    return *this;
}

inline ilrd::Complex& ilrd::Complex::operator*=(const ilrd::Complex& other)
{
    *this = *this * other;

    return *this;
}

inline ilrd::Complex& ilrd::Complex::operator/=(const ilrd::Complex& other)
{
    *this = *this / other;

    return *this;
}

#endif // __ILRD_RD176_COMPLEX_HPP__
