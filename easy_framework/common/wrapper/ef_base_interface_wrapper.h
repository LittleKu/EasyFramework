#ifndef EASY_FRAMEWORK_COMMON_WRAPPER_BASE_INTERFACE_H_
#define EASY_FRAMEWORK_COMMON_WRAPPER_BASE_INTERFACE_H_

#include "easy_framework/common/ef_refptr.h"
#include "easy_framework/common/wrapper/ef_base_wrapper.h"

namespace ef {

namespace common {

namespace wrapper {

template <typename _Ty,
          typename = std::enable_if<
              std::is_convertible<_Ty*, IBaseInterface*>::value>::type>
class BaseInterfaceImpl : public RefBaseImpl<_Ty> {
  using This = BaseInterfaceImpl<_Ty>;
  using Base = RefBaseImpl<_Ty>;

 public:
  BaseInterfaceImpl() : Base() {}
  virtual ~BaseInterfaceImpl() override {}

 public:  // override IBaseInterface methods
  bool QueryInterface(const char* interface_unique,
                      IBaseInterface** out_interface) override {
    if (!interface_unique ||
        !std::char_traits<char>::length(interface_unique) || out_interface) {
      return false;
    }

    if (!host_weak_) {
      return false;
    }

    EFRefPtr<IBaseInterface> host_strong;
    if (!host_weak_->Lock(
            reinterpret_cast<IRefBase**>(host_strong.addressof()))) {
      return false;
    }

    return host_strong->QueryInterface(interface_unique, out_interface);
  }

  bool ConnectInterface(IBaseInterface* host) override {
    if (host == nullptr) {
      return false;
    }

    return host->GetWeakRef(host_weak_.addressof());
  }

 protected:
  EFRefPtr<IWeakRef> host_weak_ = nullptr;
};

}  // namespace wrapper

}  // namespace common
}  // namespace ef
#endif  // !EASY_FRAMEWORK_COMMON_WRAPPER_BASE_INTERFACE_H_
