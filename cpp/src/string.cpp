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

    return count + 1;
}

char* String::Strdup(const char* str)
{
    assert(NULL != str);

    char* tmp = new char[StrLen(str)];
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

std::size_t String::Length() const
{
    const char* tmp = Cstr();
    size_t count = 0;
    while ('\0' != *tmp++)
    {
        ++count;
    }

    return count;
}

const char* String::Cstr() const { return m_cstr; }

String& String::operator=(const String& other)
{
    char* tmp = Strdup(other.Cstr());
    delete[] m_cstr;
    m_cstr = tmp;

    return *this;
}

static Operat CmpFun(const char* str1, const char* str2)
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

namespace ilrd
{
std::ostream& operator<<(std::ostream& os, const String& str)
{
    return os << "The String is: " << str.Cstr() << "\n";
}

bool operator==(const String& left, const String& right)
{
    return (EQUAL == CmpFun(left.Cstr(), right.Cstr()));
}

bool operator>(const String& left, const String& right)
{
    return (BIG == CmpFun(left.Cstr(), right.Cstr()));
}

bool operator<(const String& left, const String& right)
{
    return (LITTLE == CmpFun(left.Cstr(), right.Cstr()));
}
} // namespace ilrd