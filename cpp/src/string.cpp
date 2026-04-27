/*
 * Writer: Robi
 * Checker: Ehud
 * Date: 20.04.2026
 */
#include <cassert> // assert
#include <cstring>
#include <ostream> // ostream

#include "string.hpp" // my_api

namespace ilrd
{
String::String(const char* str) : m_cstr(Strdup(str))
{
/*empty on purpose*/ }

String::String(const String& other)
    : m_cstr(Strdup((other.Cstr()))) { /*empty on purpose*/ }

String::~String() { delete[] Cstr(); }

char* String::Strdup(const char* str)
{
    assert(str);

    size_t size = strlen(str) + 1;
    char* tmp = new char[size];

    memccpy(tmp, str, sizeof(char), size);

    return tmp;
}

std::size_t String::Length() const { return (strlen(Cstr())); }

const char* String::Cstr() const { return m_cstr; }

String& String::operator=(const String& other)
{
    char* tmp = Strdup(other.Cstr());
    delete[] m_cstr;
    m_cstr = tmp;

    return *this;
}

std::ostream& operator<<(std::ostream& os, const String& str)
{
    return os << "The String is: " << str.Cstr() << "\n";
}

bool operator==(const String& left, const String& right)
{
    return (0 == strcmp(left.Cstr(), right.Cstr()));
}

bool operator>(const String& left, const String& right)
{
    return (0 < strcmp(left.Cstr(), right.Cstr()));
}

bool operator<(const String& left, const String& right)
{
    return (0 > strcmp(left.Cstr(), right.Cstr()));
}
} // namespace ilrd