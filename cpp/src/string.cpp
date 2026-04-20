/*
 * Writer: Robi
 * Checker: Ehud
 * Date: 20.04.2026
 */
#include <cassert> // assert
#include <ostream> // ostream

#include "string.hpp" // my_api

using namespace ilrd;

enum Operat
{
    LITTLE,
    BIG,
    EQUAL
};

static size_t StrLen(const char* str)
{
    size_t count = 0;

    assert(NULL != str);

    while ('\0' != *str++)
    {
        ++count;
    }

    return count;
}

char* String::Strdup(const char* str)
{
    assert(NULL != str);

    char* tmp = new char[StrLen(str) + 1];
    char* tmp_start = tmp;
    while ('\0' != *str)
    {
        *tmp++ = *str++;
    }

    *tmp = '\0';

    return tmp_start;
}

String::String(const char* str) : m_cstr(Strdup(str)) {}
String::String(const String& other) : m_cstr(Strdup((other.Cstr()))) {}
String::~String() { delete[] Cstr(); }

std::size_t String::Length() const { return (StrLen(Cstr())); }

const char* String::Cstr() const { return m_cstr; }

String& String::operator=(const String& other)
{
    char* tmp = Strdup(other.Cstr());
    delete[] m_cstr;
    m_cstr = tmp;

    return *this;
}

static Operat CmpFunEnum(const char* str1, const char* str2)
{
    assert(NULL != str1);
    assert(NULL != str2);

    while ('\0' != *str1 && '\0' != *str2)
    {
        if (*str1 > *str2)
        {
            return BIG;
        }
        else if (*str1++ < *str2++)
        {
            return LITTLE;
        }
    }

    if ('\0' == *str1 && '\0' == *str2)
    {
        return EQUAL;
    }
    if ('\0' != *str1)
    {
        return BIG;
    }

    return LITTLE;
}

static int CmpFun(const char* str1, const char* str2)
{
    assert(NULL != str1);
    assert(NULL != str2);

    while ('\0' != *str1 && *str1 == *str2)
    {
        ++str1;
        ++str2;
    }

    return (*reinterpret_cast<unsigned char*>(const_cast<char*>(str1)) -
            *reinterpret_cast<unsigned char*>(const_cast<char*>(str2)));
}

namespace ilrd
{
std::ostream& operator<<(std::ostream& os, const String& str)
{
    return os << "The String is: " << str.Cstr() << "\n";
}

bool operator==(const String& left, const String& right)
{
    return (0 == CmpFun(left.Cstr(), right.Cstr()));
}

bool operator>(const String& left, const String& right)
{
    return (0 < CmpFun(left.Cstr(), right.Cstr()));
}

bool operator<(const String& left, const String& right)
{
    return (0 > CmpFun(left.Cstr(), right.Cstr()));
}
} // namespace ilrd