#ifndef EASY_FRAMEWORK_COMMON_WRAPPER_BASE_H_
#define EASY_FRAMEWORK_COMMON_WRAPPER_BASE_H_

#include "easy_framework/common/ef_ref_count.h"
#include "easy_framework/include/ef_base.h"

#include <string>

namespace ef {

namespace common {

namespace wrapper {

class WeakReference final : public IWeakObject {
  IBaseInterface* host_ptr_{nullptr};
  RefCount* ref_ptr_{nullptr};
  mutable base::AtomicRefCount ref_count_{0};

 public:
  WeakReference(IBaseInterface* host_ptr, RefCount* ref_ptr)
      : host_ptr_(host_ptr), ref_ptr_(ref_ptr) {
    ref_ptr_->AddRefWeak();
  }
  inline virtual ~WeakReference() { ref_ptr_->ReleaseWeak(); }

 public:  // override IWeakObject methods
  void AddRef() const override { ref_count_.Increment(); }

  bool Release() const override {
    if (!ref_count_.Decrement()) {
      delete this;
      return true;
    }

    return false;
  }

  bool Expired() const override { return !ref_ptr_->HasAtLeastOneRef(); }

  bool Lock(IBaseInterface** strong_ptr) override {
    if (strong_ptr == nullptr) {
      return false;
    }

    if (!Expired()) {
      host_ptr_->AddRef();
      *strong_ptr = host_ptr_;
      return true;
    }
    return false;
  }
};

template <typename Interface,
          typename = std::enable_if<
              std::is_convertible<Interface*, IBaseInterface*>::value>::type>
class BaseInterfaceImpl : public Interface {
  using This = BaseInterfaceImpl<Interface>;

 public:
  BaseInterfaceImpl() : ref_ptr_(new RefCount) { ref_ptr_->AddRefWeak(); }
  virtual ~BaseInterfaceImpl() { ref_ptr_->ReleaseWeak(); }

  // override IBaseInterface methods
  void AddRef() const override { ref_ptr_->AddRefImpl(); }

  bool Release() const override {
    if (ref_ptr_->ReleaseImpl()) {
      delete this;
      return true;
    }
    return false;
  }
  bool QueryInterface(const char* interface_unique,
                      IBaseInterface** out_interface) const override {
    if (!out_interface || !interface_unique ||
        !std::char_traits<char>::length(interface_unique)) {
      return false;
    }

    if (std::char_traits<char>::compare(
            interface_unique, Unique(),
            std::char_traits<char>::length(interface_unique)) == 0) {
      AddRef();
      *out_interface = const_cast<This*>(this);
      return true;
    }
    return false;
  }

  bool GetWeakRef(IWeakObject** out_weak_ref) const override {
    if (!out_weak_ref)
      return false;

    auto* weak_ref = new WeakReference(const_cast<This*>(this), ref_ptr_);
    weak_ref->AddRef();
    *out_weak_ref = weak_ref;
    return true;
  }

 protected:
  RefCount* ref_ptr_{nullptr};
};

}  // namespace wrapper

}  // namespace common

}  // namespace ef

#endif  // !EASY_FRAMEWORK_COMMON_WRAPPER_BASE_H_