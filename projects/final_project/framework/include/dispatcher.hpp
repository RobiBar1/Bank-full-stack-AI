/*
Writer:  Robi
Checker: Omer
Date:    28.06.2026

comments: non
*/

#ifndef __ILRD_RD176_DISPATCHER__
#define __ILRD_RD176_DISPATCHER__

#include <algorithm>     // for_each
#include <logger.hpp>    // DEBUG_INFO
#include <unordered_set> // unordered_set

namespace ilrd
{

template <class Event> class BaseCallback;

template <class Event> class Dispatcher
{
  public:
    Dispatcher() = default;
    ~Dispatcher() noexcept; // notify death to all
    Dispatcher(const Dispatcher& other) = delete;
    Dispatcher(Dispatcher&& other) = delete;
    Dispatcher& operator=(const Dispatcher& other) = delete;
    Dispatcher& operator=(Dispatcher&& other) = delete;

    void Subscribe(BaseCallback<Event>* callback);
    void Unsubscribe(BaseCallback<Event>* callback);
    void NotifyAll(const Event& event);

  private:
    std::unordered_set<BaseCallback<Event>*> m_callbacks;

}; // Dispatcher

template <class Event> class BaseCallback
{
  public:
    virtual ~BaseCallback() noexcept; // unsubscribes
    virtual void Notify(const Event& event) = 0;
    virtual void NotifyDeathEx() {}; // user can override for personalization
    void SetDispatcher(Dispatcher<Event>* dispatcher);

  private:
    Dispatcher<Event>* m_dispatcher = nullptr;

    friend Dispatcher<Event>;
    void NotifyDeath(); // sets dispatcher to null and calls notifydeathex

}; // BaseCallback

template <class Event, class Observer>
class Callback : public BaseCallback<Event>
{
  public:
    using NotifyFunc = void (Observer::*)(
        const Event& event); // explore syntax - func that belongs to namespace
    using NotifyDeathFunc = void (Observer::*)();

    explicit Callback(Observer& observer, NotifyFunc notifFunc,
                      NotifyDeathFunc notifDeathFunc = nullptr);
    virtual void Notify(const Event& event);
    virtual void NotifyDeathEx();

  private:
    Observer& m_observer;
    NotifyFunc m_notifyFunc;
    NotifyDeathFunc m_notifyDeathFunc;

}; // Callback

/*****************************************************************/
/******************* DISPATCHER IMPLEMENTATION *******************/
/*****************************************************************/

template <class Event> Dispatcher<Event>::~Dispatcher() noexcept
{
    DEBUG_INFO("Dispatcher destructs");
    std::unordered_set<BaseCallback<Event>*> copy = m_callbacks;
    std::for_each(copy.begin(), copy.end(), [](BaseCallback<Event>* callback)
                  { callback->NotifyDeath(); });
}

template <class Event>
void Dispatcher<Event>::Subscribe(BaseCallback<Event>* callback)
{
    DEBUG_INFO("Dispatcher subscribes");
    m_callbacks.insert(callback);
    callback->SetDispatcher(this);
}

template <class Event>
void Dispatcher<Event>::Unsubscribe(BaseCallback<Event>* callback)
{
    DEBUG_INFO("Dispatcher unsubscribes");
    m_callbacks.erase(callback);
    callback->SetDispatcher(nullptr);
}

template <class Event> void Dispatcher<Event>::NotifyAll(const Event& event)
{
    DEBUG_INFO("Dispatcher broadcasts");
    std::unordered_set<BaseCallback<Event>*> copy = m_callbacks;
    std::for_each(copy.begin(), copy.end(),
                  [event](BaseCallback<Event>* callback)
                  { callback->Notify(event); });
}

/*****************************************************************/
/******************* BASE CALLBACK IMPLEMENTATION ****************/
/*****************************************************************/

template <class Event> BaseCallback<Event>::~BaseCallback() noexcept
{
    if (nullptr != m_dispatcher)
    {
        m_dispatcher->Unsubscribe(this);
        DEBUG_INFO("Callback died and unsubscribed");
    }
}

template <class Event>
void BaseCallback<Event>::SetDispatcher(Dispatcher<Event>* dispatcher)
{
    m_dispatcher = dispatcher;
}

template <class Event> void BaseCallback<Event>::NotifyDeath()
{
    DEBUG_INFO("Callback notified of death");
    m_dispatcher = nullptr;
    NotifyDeathEx();
}

/*****************************************************************/
/******************* CALLBACK IMPLEMENTATION *********************/
/*****************************************************************/

template <class Event, class Observer>
Callback<Event, Observer>::Callback(Observer& observer, NotifyFunc notifFunc,
                                    NotifyDeathFunc notifDeathFunc)
    : m_observer(observer), m_notifyFunc(notifFunc),
      m_notifyDeathFunc(notifDeathFunc)
{
    DEBUG_INFO("New callback");
}

template <class Event, class Observer>
void Callback<Event, Observer>::Notify(const Event& event)
{
    if (nullptr != m_notifyFunc)
    {
        (m_observer.*m_notifyFunc)(event);
    }
}

template <class Event, class Observer>
void Callback<Event, Observer>::NotifyDeathEx()
{
    if (nullptr != m_notifyDeathFunc)
    {
        (m_observer.*m_notifyDeathFunc)();
    }
}

} // namespace ilrd

#endif