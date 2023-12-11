/**
 * @file      : ef_system_impl.cc
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-07 14:28:36
 * @brief     :
 */
#include "easy_framework/src/ef_system_impl.h"

namespace ef {

INTERFACE_UNIQUE_IMPL(IEFSystem, EFSystemImpl)

EFSystemImpl::EFSystemImpl() = default;

EFSystemImpl::~EFSystemImpl() = default;

bool EFSystemImpl::QueryInterface(const char* interface_unique,
                                  IBaseInterface** out_interface) const {
  return false;
}

bool EFSystemImpl::ConnectInterface(IBaseInterface* host) {
  return false;
}

bool EFSystemImpl::Initialize(void* instance) {
  return false;
}

bool EFSystemImpl::Uninitialize() {
  return false;
}

}  // namespace ef
