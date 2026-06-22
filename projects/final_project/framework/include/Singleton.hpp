/**
Writer:  Robi
Checker: Chen
Date:    15/06/2026
**/
#ifndef __ILRD_RD176_SINGLETON__
#define __ILRD_RD176_SINGLETON__

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

template <class T> class Singleton
{
  public:
    Singleton() = delete;
    Singleton(const Singleton& other) = delete;
    Singleton& operator=(const Singleton& other) = delete;
    Singleton(Singleton&& other) = delete;
    Singleton& operator=(Singleton&& other) = delete;
    ~Singleton() = default;

    template <class... Types> static T* GetInstance(Types... args);

  private:
    class SingletonDestroyer
    {
      public:
        SingletonDestroyer() = default;
        ~SingletonDestroyer() noexcept;
    };

    static std::atomic<T*> m_instance;
    static std::mutex m_mtx;
    static SingletonDestroyer m_destroyer;

}; // Singleton

template <class T> std::atomic<T*> Singleton<T>::m_instance(nullptr);

template <class T> std::mutex Singleton<T>::m_mtx;

template <class T>
typename Singleton<T>::SingletonDestroyer Singleton<T>::m_destroyer;

template <class T>
template <class... Types>
T* Singleton<T>::GetInstance(Types... args)
{
    if (nullptr == m_instance)
    {
        std::lock_guard<std::mutex> lock(m_mtx);

        if (nullptr == m_instance)
        {
            m_instance = new T(args...);
        }
    }

    return m_instance;
}

template <class T> void Singleton<T>::SingletonDestroyer::Init()
{
    // empty on purpose
}

template <class T>
Singleton<T>::SingletonDestroyer::~SingletonDestroyer() noexcept
{
    delete m_instance;
}

} // namespace ilrd

#endif // __ILRD_RD176_SINGLETON__
