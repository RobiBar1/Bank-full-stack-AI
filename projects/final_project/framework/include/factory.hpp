#ifndef __ILRD_RD176_FACTORY__
#define __ILRD_RD176_FACTORY__

#include <functional>
#include <memory>
#include <unordered_map>

#if __cplusplus >= 201103L
// noexcept is defined, do nothing
#else
#define noexcept throw()
#endif // #if cplusplus >= 201103L

namespace ilrd
{

template <class Key, class Base, class... Types> class Factory
{
  public:
    Factory() = default;
    ~Factory() noexcept = default;
    Factory(const Factory& other) = delete;
    Factory& operator=(const Factory& other) = delete;
    Factory(Factory&& other) = delete;
    Factory& operator=(Factory&& other) = delete;

    void Add(const Key& key,
             std::function<std::shared_ptr<Base>(Types... args)> creator);

    std::shared_ptr<Base> Create(const Key& key, Types... args);

  private:
    std::unordered_map<Key, std::function<std::shared_ptr<Base>(Types... args)>>
        m_creators;

}; // Factory

template <class Key, class Base, class... Types>
inline void Factory<Key, Base, Types...>::Add(
    const Key& key, std::function<std::shared_ptr<Base>(Types... args)> creator)
{
    m_creators[key] = creator;
}

template <class Key, class Base, class... Types>
inline std::shared_ptr<Base>
Factory<Key, Base, Types...>::Create(const Key& key, Types... args)
{
    return m_creators.at(key);
}

} // namespace ilrd

#endif // __ILRD_RD176_FACTORY__
