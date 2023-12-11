/**
 * @file      : ef_system_impl.h
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-07 14:28:29
 * @brief     :
 */
#ifndef EASY_FRAMEWORK_SYSTEM_IMPL_H_
#define EASY_FRAMEWORK_SYSTEM_IMPL_H_

#include "easy_framework/common/wrapper/ef_base_interface_wrapper.h"
#include "easy_framework/include/ef_system.h"

namespace ef {

class EFSystemImpl final
    : public ef::common::wrapper::BaseInterfaceImpl<IEFSystem> {
  using Base = ef::common::wrapper::BaseInterfaceImpl<IEFSystem>;

 public:
  EFSystemImpl();
  ~EFSystemImpl() override;

  DECLARE_INTERFACE_UNIQUE(IEFSystem);

 public:  // override IBaseInterface method
  bool QueryInterface(const char* interface_unique,
                      IBaseInterface** out_interface) const final;
  bool ConnectInterface(IBaseInterface* host) final;

 public:  // override IEFSystem methods
  bool Initialize(void* instance) override;
  bool Uninitialize() override;

 private:
};

}  // namespace ef

#endif  // !EASY_FRAMEWORK_SYSTEM_IMPL_H_
