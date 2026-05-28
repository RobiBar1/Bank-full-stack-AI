/*
Writer:  Robi
Checker: Shahar
Date:    28.05.2026
*/

#ifndef __ILRD_RD176_SHARED_POINTER__
#define __ILRD_RD176_SHARED_POINTER__

#include <stdexcept>

#if cplusplus >= 201103L
// noexcept is defined, do nothing
#else
#define noexcept throw()
#endif // #if cplusplus >= 201103L

namespace ilrd
{
template <typename T> class SharedPointer
{
  public:
    explicit SharedPointer(T* ptr = 0);
    SharedPointer(const SharedPointer<T>& other) noexcept;
    ~SharedPointer() noexcept;

    SharedPointer& operator=(const SharedPointer& other);
    T& operator*() const;
    T* operator->() const;

    template <typename U> SharedPointer(const SharedPointer<U>& other) noexcept;

  private:
    template <typename U> friend class SharedPointer;
    void* operator new(std::size_t count);
    void IncreaseCounter() const;
    void DecreaseCounter();

    T* m_ptr;
    std::size_t* m_counter;
};

template <typename T>
SharedPointer<T>::SharedPointer(T* ptr)
    : m_ptr(ptr), m_counter(new std::size_t(1))
{ // empty on purpose
}

template <typename T>
SharedPointer<T>::SharedPointer(const SharedPointer<T>& other) noexcept
    : m_ptr(other.m_ptr), m_counter(other.m_counter)
{
    IncreaseCounter();
}

template <typename T> SharedPointer<T>::~SharedPointer() noexcept
{
    DecreaseCounter();
}

template <typename T>
SharedPointer<T>& SharedPointer<T>::operator=(const SharedPointer<T>& other)
{
    other.IncreaseCounter();
    DecreaseCounter();
    m_counter = other.m_counter;
    m_ptr = other.m_ptr;

    return *this;
}

template <typename T> T& SharedPointer<T>::operator*() const { return *m_ptr; }

template <typename T> T* SharedPointer<T>::operator->() const { return m_ptr; }

template <typename T>
template <typename U>
SharedPointer<T>::SharedPointer(const SharedPointer<U>& other) noexcept
    : m_ptr(other.m_ptr), m_counter(other.m_counter)
{
    IncreaseCounter();
}

template <typename T> void SharedPointer<T>::IncreaseCounter() const
{
    ++(*m_counter);
}

template <typename T> void SharedPointer<T>::DecreaseCounter()
{
    if (0 == (--(*m_counter)))
    {
        delete m_ptr;
        delete m_counter;
    }
}

} // namespace ilrd
  // namespace ilrd
#endif // __ILRD_RD176_SHARED_POINTER__