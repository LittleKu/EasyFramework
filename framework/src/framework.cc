/**
 * @file      : framework.cc
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2025-04-15 20:34:11
 * @brief     : 
 */
#include "libef/framework/src/framework.h"

#include "base/task/sequence_manager/sequence_manager_impl.h"

#include <string>

namespace libef {

FrameworkImpl::FrameworkImpl() = default;

FrameworkImpl::~FrameworkImpl() = default;

bool FrameworkImpl::QueryInterface(const char* interface_name,
                                   void** interface) {
  if (std::char_traits<char>::compare(
          interface_name, FRAMEWORK_NAME,
          std::char_traits<char>::length(interface_name)) == 0) {
    AddRef();
    *interface = this;
    return true;
  }
  return false;
}

unsigned int FrameworkImpl::GetVersion() const {
  return FRAMEWORK_VERSION;
}

bool FrameworkImpl::Initialize(void* instance) {
  if (initialized_.load() == false) {
    instance_ = reinterpret_cast<Instance>(instance);
    initialized_.store(true);
    return true;
  }
  return false;
}

void FrameworkImpl::UnInitialize() {
  if (initialized_.load() == true) {
    instance_ = nullptr;
    initialized_.store(false);
  }
}

bool FrameworkImpl::CreateMessageLoop(bool nestable,
                                      MessageLoopType type,
                                      MessageLoop** loop) {
  return false;
}

}  // namespace libef
