/**
 * @file      : ef_base_wrapper.h
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-07 14:27:55
 * @brief     :
 */
#ifndef EASY_FRAMEWORK_COMMON_WRAPPER_BASE_H_
#define EASY_FRAMEWORK_COMMON_WRAPPER_BASE_H_

#include "easy_framework/common/ef_ref_count.h"
#include "easy_framework/include/ef_base.h"

#include <string>

namespace ef {

namespace common {

namespace wrapper {

class WeakReference final : public IWeakRef {
  IRefBase* host_ptr_{nullptr};
  RefCount* ref_ptr_{nullptr};
  mutable base::AtomicRefCount ref_count_{0};

 public:
  WeakReference(IRefBase* host_ptr, RefCount* ref_ptr)
      : host_ptr_(host_ptr), ref_ptr_(ref_ptr) {
    ref_ptr_->AddRefWeak();
  }
  virtual ~WeakReference() { ref_ptr_->ReleaseWeak(); }

 public:  // override IWeakReft methods
  void AddRef() const override { ref_count_.Increment(); }

  bool Release() const override {
    if (!ref_count_.Decrement()) {
      delete this;
      return true;
    }

    return false;
  }

  bool Expired() const override { return !ref_ptr_->HasAtLeastOneRef(); }

  bool Lock(IRefBase** strong_ptr) override {
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

template <typename _Ty,
          typename =
              std::enable_if<std::is_convertible<_Ty*, IRefBase*>::value>::type>
class RefBaseImpl : public _Ty {
  using This = RefBaseImpl<_Ty>;

 public:
  RefBaseImpl() : ref_ptr_(new RefCount) { ref_ptr_->AddRefWeak(); }
  virtual ~RefBaseImpl() { ref_ptr_->ReleaseWeak(); }

  // override IRefBase methods
  void AddRef() const override { ref_ptr_->AddRefImpl(); }

  bool Release() const override {
    if (ref_ptr_->ReleaseImpl()) {
      delete this;
      return true;
    }
    return false;
  }

  bool GetWeakRef(IWeakRef** out_weak_ref) const override {
    if (!out_weak_ref) {
      return false;
    }

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
