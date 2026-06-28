#include <iostream>
#include <memory>
#include <string>

#include "dispatcher.hpp"

using namespace ilrd;

const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";

typedef enum
{
    TEST_STATUS_SUCCESS,
    TEST_STATUS_FAILURE
} test_status_t;

static test_status_t Dispatcher_SmokeTest(void);
static test_status_t Dispatcher_NotifyAll_CallsSingleSubscriber_Test(void);
static test_status_t Dispatcher_NotifyAll_CallsMultipleSubscribers_Test(void);
static test_status_t Dispatcher_NotifyAll_PassesCorrectEvent_Test(void);
static test_status_t Dispatcher_Unsubscribe_StopsNotifications_Test(void);
static test_status_t
Dispatcher_Destruction_NotifiesDeathToSubscribers_Test(void);
static test_status_t
Dispatcher_Destruction_WithNoDeathFunc_DoesNotCrash_Test(void);
static test_status_t
Dispatcher_CallbackDestruction_UnsubscribesFromDispatcher_Test(void);
static test_status_t
Dispatcher_SameObserver_MultipleCallbacks_BothNotified_Test(void);
static test_status_t
Dispatcher_MultipleIndependentDispatchers_DontInterfere_Test(void);

static void CheckTest(test_status_t result, std::string name)
{
    if (result == TEST_STATUS_SUCCESS)
    {
        std::cout << "-> " << name << ": " << GREEN << "PASSED\n\n" << RESET;
    }
    else
    {
        std::cout << "-> " << name << ": " << RED << "FAILED\n\n" << RESET;
    }
}

int main()
{
    std::cout << "\n ---- DISPATCHER TESTS ----\n\n";

    CheckTest(Dispatcher_SmokeTest(), "Dispatcher_SmokeTest");
    CheckTest(Dispatcher_NotifyAll_CallsSingleSubscriber_Test(),
              "Dispatcher_NotifyAll_CallsSingleSubscriber_Test");
    CheckTest(Dispatcher_NotifyAll_CallsMultipleSubscribers_Test(),
              "Dispatcher_NotifyAll_CallsMultipleSubscribers_Test");
    CheckTest(Dispatcher_NotifyAll_PassesCorrectEvent_Test(),
              "Dispatcher_NotifyAll_PassesCorrectEvent_Test");
    CheckTest(Dispatcher_Unsubscribe_StopsNotifications_Test(),
              "Dispatcher_Unsubscribe_StopsNotifications_Test");
    CheckTest(Dispatcher_Destruction_NotifiesDeathToSubscribers_Test(),
              "Dispatcher_Destruction_NotifiesDeathToSubscribers_Test");
    CheckTest(Dispatcher_Destruction_WithNoDeathFunc_DoesNotCrash_Test(),
              "Dispatcher_Destruction_WithNoDeathFunc_DoesNotCrash_Test");
    CheckTest(Dispatcher_CallbackDestruction_UnsubscribesFromDispatcher_Test(),
              "Dispatcher_CallbackDestruction_UnsubscribesFromDispatcher_Test");
    CheckTest(Dispatcher_SameObserver_MultipleCallbacks_BothNotified_Test(),
              "Dispatcher_SameObserver_MultipleCallbacks_BothNotified_Test");
    CheckTest(Dispatcher_MultipleIndependentDispatchers_DontInterfere_Test(),
              "Dispatcher_MultipleIndependentDispatchers_DontInterfere_Test");

    return 0;
}

class Event
{
  public:
    explicit Event(int value = 0) : m_value(value) {}

    int GetValue() const { return m_value; }

  private:
    int m_value;
};

class Observer
{
  public:
    explicit Observer(int id = 0)
        : m_id(id), m_notifyCount(0), m_lastValue(0), m_died(false)
    {
    }

    void Notify(const Event& event)
    {
        ++m_notifyCount;
        m_lastValue = event.GetValue();
    }

    void NotifyDeath() { m_died = true; }

    int GetId() const { return m_id; }
    int GetNotifyCount() const { return m_notifyCount; }
    int GetLastValue() const { return m_lastValue; }
    bool HasDied() const { return m_died; }

  private:
    int m_id;
    int m_notifyCount;
    int m_lastValue;
    bool m_died;
};

static test_status_t Dispatcher_SmokeTest(void)
{
    Dispatcher<Event> dispatcher;
    (void)dispatcher;

    return TEST_STATUS_SUCCESS;
}

static test_status_t Dispatcher_NotifyAll_CallsSingleSubscriber_Test(void)
{
    Dispatcher<Event> dispatcher;
    Observer observer;
    Callback<Event, Observer> callback(observer, &Observer::Notify,
                                       &Observer::NotifyDeath);

    dispatcher.Subscribe(&callback);
    dispatcher.NotifyAll(Event(1));

    if (observer.GetNotifyCount() != 1)
    {
        return TEST_STATUS_FAILURE;
    }

    return TEST_STATUS_SUCCESS;
}

static test_status_t Dispatcher_NotifyAll_CallsMultipleSubscribers_Test(void)
{
    Dispatcher<Event> dispatcher;
    Observer observer_a(1);
    Observer observer_b(2);
    Observer observer_c(3);

    Callback<Event, Observer> callback_a(observer_a, &Observer::Notify,
                                         &Observer::NotifyDeath);
    Callback<Event, Observer> callback_b(observer_b, &Observer::Notify,
                                         &Observer::NotifyDeath);
    Callback<Event, Observer> callback_c(observer_c, &Observer::Notify,
                                         &Observer::NotifyDeath);

    dispatcher.Subscribe(&callback_a);
    dispatcher.Subscribe(&callback_b);
    dispatcher.Subscribe(&callback_c);

    dispatcher.NotifyAll(Event(0));

    if (observer_a.GetNotifyCount() != 1 || observer_b.GetNotifyCount() != 1 ||
        observer_c.GetNotifyCount() != 1)
    {
        return TEST_STATUS_FAILURE;
    }

    return TEST_STATUS_SUCCESS;
}

static test_status_t Dispatcher_NotifyAll_PassesCorrectEvent_Test(void)
{
    Dispatcher<Event> dispatcher;
    Observer observer;
    Callback<Event, Observer> callback(observer, &Observer::Notify,
                                       &Observer::NotifyDeath);

    const int EXPECTED_VALUE = 99;

    dispatcher.Subscribe(&callback);
    dispatcher.NotifyAll(Event(EXPECTED_VALUE));

    if (observer.GetLastValue() != EXPECTED_VALUE)
    {
        return TEST_STATUS_FAILURE;
    }

    return TEST_STATUS_SUCCESS;
}

static test_status_t Dispatcher_Unsubscribe_StopsNotifications_Test(void)
{
    Dispatcher<Event> dispatcher;
    Observer observer;
    Callback<Event, Observer> callback(observer, &Observer::Notify,
                                       &Observer::NotifyDeath);

    dispatcher.Subscribe(&callback);
    dispatcher.Unsubscribe(&callback);
    dispatcher.NotifyAll(Event(1));

    if (observer.GetNotifyCount() != 0)
    {
        return TEST_STATUS_FAILURE;
    }

    return TEST_STATUS_SUCCESS;
}

static test_status_t
Dispatcher_Destruction_NotifiesDeathToSubscribers_Test(void)
{
    Observer observer;
    Callback<Event, Observer> callback(observer, &Observer::Notify,
                                       &Observer::NotifyDeath);

    {
        Dispatcher<Event> dispatcher;
        dispatcher.Subscribe(&callback);
    }

    if (!observer.HasDied())
    {
        return TEST_STATUS_FAILURE;
    }

    return TEST_STATUS_SUCCESS;
}

static test_status_t
Dispatcher_Destruction_WithNoDeathFunc_DoesNotCrash_Test(void)
{
    Observer observer;
    Callback<Event, Observer> callback(observer, &Observer::Notify);

    {
        Dispatcher<Event> dispatcher;
        dispatcher.Subscribe(&callback);
    }

    if (observer.HasDied())
    {
        return TEST_STATUS_FAILURE;
    }

    return TEST_STATUS_SUCCESS;
}

static test_status_t
Dispatcher_CallbackDestruction_UnsubscribesFromDispatcher_Test(void)
{
    Dispatcher<Event> dispatcher;
    Observer observer;

    {
        Callback<Event, Observer> callback(observer, &Observer::Notify,
                                           &Observer::NotifyDeath);
        dispatcher.Subscribe(&callback);
        dispatcher.Unsubscribe(&callback);
    }

    dispatcher.NotifyAll(Event(1));

    if (observer.GetNotifyCount() != 0)
    {
        return TEST_STATUS_FAILURE;
    }

    return TEST_STATUS_SUCCESS;
}

static test_status_t
Dispatcher_SameObserver_MultipleCallbacks_BothNotified_Test(void)
{
    Dispatcher<Event> dispatcher;
    Observer observer;

    Callback<Event, Observer> callback_a(observer, &Observer::Notify,
                                         &Observer::NotifyDeath);
    Callback<Event, Observer> callback_b(observer, &Observer::Notify,
                                         &Observer::NotifyDeath);

    dispatcher.Subscribe(&callback_a);
    dispatcher.Subscribe(&callback_b);

    dispatcher.NotifyAll(Event(1));

    if (observer.GetNotifyCount() != 2)
    {
        return TEST_STATUS_FAILURE;
    }

    return TEST_STATUS_SUCCESS;
}

static test_status_t
Dispatcher_MultipleIndependentDispatchers_DontInterfere_Test(void)
{
    Dispatcher<Event> dispatcher_a;
    Dispatcher<Event> dispatcher_b;

    Observer observer_a;
    Observer observer_b;

    Callback<Event, Observer> callback_a(observer_a, &Observer::Notify,
                                         &Observer::NotifyDeath);
    Callback<Event, Observer> callback_b(observer_b, &Observer::Notify,
                                         &Observer::NotifyDeath);

    dispatcher_a.Subscribe(&callback_a);
    dispatcher_b.Subscribe(&callback_b);

    dispatcher_a.NotifyAll(Event(1));

    if (observer_a.GetNotifyCount() != 1 || observer_b.GetNotifyCount() != 0)
    {
        return TEST_STATUS_FAILURE;
    }

    return TEST_STATUS_SUCCESS;
}