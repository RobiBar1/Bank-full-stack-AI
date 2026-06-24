/**
Writer:  Robi
Checker: Chen
Date:    24/06/2026
**/
#ifndef __ILRD_RD176_Handleton__
#define __ILRD_RD176_Handleton__

#include <atomic>
#include <iostream>
#include <mutex> // mutex

#if __cplusplus >= 201103L
// noexcept is defined, do nothing
#else
#define noexcept throw()
#endif // #if cplusplus >= 201103L

namespace ilrd
{

template <class T> class Handleton
{
  public:
    Handleton() = delete;
    Handleton(const Handleton& other) = delete;
    Handleton& operator=(const Handleton& other) = delete;
    Handleton(Handleton&& other) = delete;
    Handleton& operator=(Handleton&& other) = delete;
    ~Handleton() = delete;

    template <class... Types> static T* GetInstance(Types&&... args);

  private:
    class HandletonDestroyer
    {
      public:
        HandletonDestroyer() = default;
        ~HandletonDestroyer();
    };

    static std::atomic<T*> m_instance;
    static std::mutex m_mtx;
    static HandletonDestroyer m_destroyer;

}; // Singleton

} // namespace ilrd

#ifdef IMPLEMENTER_MODE

namespace ilrd
{
template <class T> std::atomic<T*> Handleton<T>::m_instance(nullptr);

template <class T> std::mutex Handleton<T>::m_mtx;

template <class T>
typename Handleton<T>::HandletonDestroyer Handleton<T>::m_destroyer;

template <class T>
template <class... Types>
T* Handleton<T>::GetInstance(Types&&... args)
{
    T* sin = m_instance.load(std::memory_order_acquire);
    if (nullptr == sin)
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        sin = m_instance.load(std::memory_order_relaxed);
        if (nullptr == sin)
        {
            m_instance.store(new T(std::forward<Types>(args)...),
                             std::memory_order_release);
        }
    }

    return m_instance;
}

template <class T> Handleton<T>::HandletonDestroyer::~HandletonDestroyer()
{
    delete m_instance;
    m_instance = nullptr;
}

} // namespace ilrd

#endif // IMPLEMENTER_MODE

#endif // __ILRD_RD176_Handleton__
