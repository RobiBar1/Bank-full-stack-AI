#ifndef ILRD_RD176_STRING_HPP
#define ILRD_RD176_STRING_HPP

#include <iosfwd> // ostream forward declaration only

namespace ilrd
{
class String
{
  public:
    String(const char* str = ""); // ctor - not explict on purpose
    String(const String& other);  // copy ctor
    ~String();

    std::size_t Length() const;
    const char* Cstr() const;

    String& operator=(const String& other);

  private:
    static char* Strdup(const char* str);
    char* m_cstr;
};

std::ostream& operator<<(std::ostream& os, const String& str);
bool operator==(const String& left, const String& right);
bool operator>(const String& left, const String& right);
bool operator<(const String& left, const String& right);
} // namespace ilrd

#endif // ILRD_RD176_STRING_HPP