/**
 * @file      : ref_impl.h
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2025-04-15 20:12:45
 * @brief     : 
 */
#ifndef LIBEF_WRAPPER_REF_IMPL_H
#define LIBEF_WRAPPER_REF_IMPL_H

#include "libef/include/libef.h"

#include <atomic>
#include <type_traits>

namespace libef {

template <typename T>
concept HasRefType = requires(T& t) {
  t.AddRef();
  t.Release();
};

template <typename T>
concept IsBaseOfRefType = std::is_base_of_v<IBaseRef, T>;

template <typename T>
  requires IsBaseOfRefType<T>
class BaseRefImpl : public T {
 public:
  virtual ~BaseRefImpl() = default;

  void AddRef() const override {
    ref_count_.fetch_add(1, std::memory_order_relaxed);
  }

  bool Release() const override {
    if (ref_count_.fetch_sub(1, std::memory_order_acq_rel) != 1) {
      return false;
    }
    delete this;
    return true;
  }

 protected:
  mutable std::atomic_int ref_count_{0};
};

}  // namespace libef
#endif  // LIBEF_WRAPPER_REF_IMPL_H
