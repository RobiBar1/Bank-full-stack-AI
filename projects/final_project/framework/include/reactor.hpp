#ifndef __ILRD_RD176_REACTOR__
#define __ILRD_RD176_REACTOR__

#include <functional>    // std::function
#include <memory>        // memory
#include <unordered_map> // unordered_map
#include <utility>       // pair
#include <vector>        // vector

namespace ilrd
{

class Reactor
{
  public:
    enum class Mode
    {
        READ,
        WRITE
    };

    using CallbackPair = std::pair<int, Mode>;
    using Callback = std::function<void(int, Mode)>;

    class IListener
    {
      public:
        virtual std::vector<CallbackPair>
        Listen(const std::vector<CallbackPair>&) = 0;
        virtual ~IListener();
    };

    explicit Reactor(std::unique_ptr<IListener> server);
    ~Reactor() = default;

    Reactor(const Reactor&) = delete;
    Reactor& operator=(const Reactor&) = delete;
    Reactor(Reactor&&) = delete;
    Reactor& operator=(Reactor&&) = delete;

    void Add(int fd, Mode mode,
             Callback handler); // should the handler be const?
    void Remove(int fd, Mode mode);
    void Run();
    void Stop();

  private:
    const std::unique_ptr<IListener> m_listener;

    struct HashPair
    {
        uint64_t operator()(const CallbackPair p) const
        {
            return std::hash<int>{}(
                p.first *
                (1 +
                 (10 * static_cast<int>(p.second)))); // not concrete may change
        }
    };

    std::unordered_map<CallbackPair, Callback, HashPair> m_handlers;
    bool m_isRunning = false;

}; // class reactor

} // namespace ilrd

#endif
