#include "reactor.hpp" // my_api

/*==============================================================*/
/*=========================== Reactor ==========================*/
/*==============================================================*/

ilrd::Reactor::Reactor(std::unique_ptr<IListener> server)
    : m_listener(std::move(server))
{
}

void ilrd::Reactor::Add(int fd, Mode mode, Callback handler)
{
    m_handlers[CallbackPair(fd, mode)] = handler;
}

void ilrd::Reactor::Remove(int fd, Mode mode)
{
    m_handlers.erase(CallbackPair(fd, mode));
}

void ilrd::Reactor::Run()
{
    if (true == m_isRunning) // if already running and handler call Run
    {
        return;
    }

    m_isRunning = true;
    std::vector<CallbackPair> arr_even_on;

    while (m_isRunning)
    {
        std::vector<CallbackPair> arr;
        std::unordered_map<CallbackPair, Callback, HashPair>
            unordered_map_copy = m_handlers;
        for (auto key : unordered_map_copy)
        {
            arr.push_back(key.first);
        }

        arr_even_on = m_listener->Listen(arr);
        for (auto key : arr_even_on)
        {
            try
            {
                unordered_map_copy.at(key)(key.first, key.second);
            }
            catch (...)
            {
                // empty on purpose
            }

            if (!m_isRunning)
            {
                return;
            }
        }
    }
}

void ilrd::Reactor::Stop() { m_isRunning = false; }

/*==============================================================*/
/*========================= IListener ==========================*/
/*==============================================================*/

ilrd::Reactor::IListener::~IListener() {}
