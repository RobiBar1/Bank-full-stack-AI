#ifndef __ILRD_RD176_SCOPE_LOCK__
#define __ILRD_RD176_SCOPE_LOCK__

#if __cplusplus >= 201103L
// noexcept is defined, do nothing
#else
#define noexcept throw()
#endif // #if cplusplus >= 201103L

namespace ilrd
{
template <typename T> class ScopeLock
{
  public:
    ScopeLock(const T& lock);
    ~ScopeLock() throw();

  private:
    ScopeLock(ScopeLock& other);
    void operator=(ScopeLock& other);

    T& m_lock;
};

template <typename T> ScopeLock<T>::ScopeLock(const T& lock) : m_lock(lock)
{
    m_lock.Lock();
}

template <typename T> ScopeLock<T>::~ScopeLock() throw() { m_lock.UnLock(); }

} // namespace ilrd

#endif // __ILRD_RD176_SCOPE_LOCK__ kk///
