#ifndef __ILRD_RD176_DLLLOADER__
#define __ILRD_RD176_DLLLOADER__

#include <string>
#include <unordered_map>

#include "handleton.hpp"

namespace ilrd
{

class DLLLoader
{
  public:
    ~DLLLoader() noexcept;
    void* Load(const std::string& path);
    void UnLoad(const std::string& path);
    void* Modify(const std::string& path);
    void CallbackLoad(const std::string& path); // calls Load -- notify
    void CallbackModify(const std::string& path);
    void CallbackUnload(const std::string& path);

    DLLLoader(const DLLLoader& other) = delete;
    DLLLoader(DLLLoader&& other) = delete;
    DLLLoader& operator=(const DLLLoader& other) = delete;
    DLLLoader& operator=(DLLLoader&& other) = delete;

  private:
    DLLLoader() = default;

    std::unordered_map<std::string, void*> m_handles;

    friend class Handleton<DLLLoader>;
};

// read about attribute constructor

} // namespace ilrd
#endif // __ILRD_RD176_DLLLOADER__