/*
Writer: Robi
Checker: Michael
Date: 02.05.2026
*/

#include <assert.h> // assert
#include <cstring>  // strlen, strcpy
#include <iostream> // ostream, istream

#include "rcstring.hpp" // this file hpp

namespace ilrd
{
// ======================== Start StringValue implement =======================

static char* CreateTheString(const char* startVal) throw(std::invalid_argument,
                                                         std::bad_alloc)
{
    if (NULL == startVal)
    {
        throw std::invalid_argument("enter null pointer to RCString");
    }

    size_t len = strlen(startVal) + 1;
    char* ch = NULL;
    ch = new char[len];
    memccpy(ch, startVal, sizeof(char), len);

    return ch;
}

RCString::StringValue::StringValue(const char* startVal) throw(
    std::invalid_argument, std::bad_alloc)
    : m_ref_count(1), m_data(CreateTheString(startVal))
{
}

RCString::StringValue::~StringValue() { delete[] m_data; }
// ======================== End StringValue implement ========================

// ======================== Start RCString implement ========================
RCString::RCString(const char* str) throw(std::invalid_argument, std::bad_alloc)
    : m_value(new StringValue(str))
{
}

RCString::RCString(const RCString& other) noexcept : m_value(other.m_value)
{
    IncreaseRefCount();
}

RCString::~RCString() noexcept { DecreaseRefCount(); }

RCString& RCString::operator=(const RCString& other) noexcept
{
    if (this != &other)
    {
        DecreaseRefCount();
        m_value = other.m_value;
        IncreaseRefCount();
    }

    return *this;
}

std::size_t RCString::Length() const noexcept { return strlen(Cstr()); }

char RCString::operator[](std::size_t index) const throw(std::out_of_range)
{
    if (index >= Length())
    {
        throw std::out_of_range("index out of range in RCString operator []");
    }

    return m_value->m_data[index];
}

RCString::CharProxy
RCString::operator[](std::size_t index) throw(std::out_of_range, std::bad_alloc)
{
    if (index >= Length())
    {
        throw std::out_of_range("index out of range in RCString operator []");
    }

    return CharProxy(*this, index);
}

std::ostream& operator<<(std::ostream& os, const RCString& str) noexcept
{
    return (os << str.Cstr());
}

std::istream& operator>>(std::istream& in, RCString& str) noexcept
{
    char buffer[SIZE_OF_BUFFER_IN_BYTES] = {'\0'};

    in >> buffer;
    str = buffer;

    return in;
}

bool operator==(const RCString& left, const RCString& right) noexcept
{
    return (0 == std::strcmp(left.Cstr(), right.Cstr()));
}

bool operator!=(const RCString& left, const RCString& right) noexcept
{
    return !(left == right);
}

bool operator<(const RCString& left, const RCString& right) noexcept
{
    return (0 > std::strcmp(left.Cstr(), right.Cstr()));
}

bool operator>(const RCString& left, const RCString& right) noexcept
{
    return (right < left);
}

// ====================== End RCString implement ============================

// ===================== Start CharProxy implement ============================
RCString::CharProxy::CharProxy(RCString& str,
                               std::size_t idx) throw(std::out_of_range)
    : m_str(str), m_idx(idx)
{
}

RCString::CharProxy&
RCString::CharProxy::operator=(char newChar) throw(std::bad_alloc)
{
    if (1 < m_str.m_value->m_ref_count)
    {
        StringValue* newValue = new StringValue(m_str.m_value->m_data);

        m_str.DecreaseRefCount();
        m_str.m_value = newValue;
    }

    m_str.m_value->m_data[m_idx] = newChar;

    return *this;
}

RCString::CharProxy&
RCString::CharProxy::operator=(CharProxy& other) throw(std::bad_alloc)
{
    *this = static_cast<char>(other);

    return *this;
}

const char* RCString::CharProxy::operator&() const
{
    return &(m_str.m_value->m_data[m_idx]);
}

// ===================== End CharProxy implement ============================
} // namespace ilrd