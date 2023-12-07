#include "easy_framework/src/ef_system_impl.h"

namespace ef {

EFSystemImpl::EFSystemImpl() = default;

EFSystemImpl::~EFSystemImpl() = default;

bool EFSystemImpl::QueryInterface(const char* interface_unique,
                                  IBaseInterface** out_interface) const {
  return false;
}

bool EFSystemImpl::Initialize(void* instance) {
  return false;
}

bool EFSystemImpl::Uninitialize() {
  return false;
}
}  // namespace ef