#include <dlfcn.h> // dlopen
#include <string>  // std::string
#include <utility> // std::pair

#include "dir_monitor.hpp" // DirMonitor::DispatcherType::ADDED
#include "dll_loader.hpp"  // my_h

namespace ilrd
{

DLLLoader::~DLLLoader() noexcept
{
    auto item = m_handles.begin();
    while (item != m_handles.end())
    {
        dlclose(item->second);
        m_handles.erase(item);
        item = m_handles.begin();
    }
}

void* DLLLoader::Load(const std::string& path)
{
    void* handle = dlopen(path.c_str(), RTLD_LAZY);

    m_handles.emplace(path, handle);

    return handle;
}

void DLLLoader::UnLoad(const std::string& path)
{
    auto handle = m_handles.find(path.c_str());

    if (handle != m_handles.end())
    {
        dlclose(handle->second);
    }

    m_handles.erase(path.c_str());
}

void* DLLLoader::Modify(const std::string& path)
{
    this->UnLoad(path);
    return (this->Load(path));
}

void DLLLoader::CallbackLoad(const std::string& path) { this->Load(path); }

void DLLLoader::CallbackModify(const std::string& path) { this->Modify(path); }

void DLLLoader::CallbackUnload(const std::string& path) { this->UnLoad(path); }

} // namespace ilrd