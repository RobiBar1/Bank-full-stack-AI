#ifndef __ILRD_RD176_RCString_HPP__
#define __ILRD_RD176_RCString_HPP__

#include <iosfwd> // ostream forward declaration only
#include <stdexcept>

#if cplusplus >= 201103L
// noexcept is defined, do nothing
#else
#define noexcept throw()
#endif // #if cplusplus >= 201103L

namespace ilrd
{

class RCString
{
  private:
    class CharProxy;

  public:
    RCString(const char* str = "") throw(
        std::invalid_argument, std::bad_alloc); // ctor - not explict on purpose
    RCString(const RCString& other) noexcept;   // copy ctor
    ~RCString() noexcept;
    RCString& operator=(const RCString& str) noexcept;
    char operator[](std::size_t idx) const throw(std::out_of_range);
    CharProxy operator[](std::size_t idx) throw(std::out_of_range,
                                                std::bad_alloc);

    std::size_t Length() const noexcept;
    inline const char* Cstr() const noexcept;

  private:
    inline void DecreaseRefCount() const;
    inline void IncreaseRefCount() const;

    struct StringValue
    {
        StringValue(const char* startVal);
        ~StringValue();

        std::size_t m_ref_count;
        char* m_data;
    };

    StringValue* m_value;
}; // class RCString

class RCString::CharProxy
{
  public:
    CharProxy(RCString& str, std::size_t idx) throw(std::out_of_range);
    CharProxy& operator=(char newChar) throw(std::bad_alloc);
    CharProxy& operator=(CharProxy& newCharProxy) throw(std::bad_alloc);
    const char* operator&() const;
    inline operator char() const;

  private:
    RCString& m_str;
    const std::size_t m_idx;
}; // class CharProxy

std::ostream& operator<<(std::ostream& os, const RCString& str) noexcept;
const size_t SIZE_OF_BUFFER_IN_BYTES = 80;
std::istream& operator>>(std::istream& in, RCString& str) noexcept;
bool operator>(const RCString& left, const RCString& right) noexcept;
bool operator<(const RCString& left, const RCString& right) noexcept;
bool operator!=(const RCString& left, const RCString& right) noexcept;
bool operator==(const RCString& left, const RCString& right) noexcept;

inline const char* RCString::Cstr() const noexcept { return m_value->m_data; }

inline void RCString::IncreaseRefCount() const { ++(m_value->m_ref_count); }
inline void RCString::DecreaseRefCount() const
{
    if (0 == (--(m_value->m_ref_count)))
    {
        delete m_value;
    }
}

inline RCString::CharProxy::operator char() const
{
    return m_str.m_value->m_data[m_idx];
}
} // namespace ilrd

#endif // _ILRD_RD176_RCString_HPP