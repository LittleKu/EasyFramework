/**
 * @file      : ef_refptr.h
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-11 08:59:44
 * @brief     :
 */
#ifndef EASY_FRAMEWORK_COMMON_REFPTR_H_
#define EASY_FRAMEWORK_COMMON_REFPTR_H_

#include <stddef.h>

#include <type_traits>

#include "easy_framework/include/ef_base.h"

namespace ef {

namespace common {

template <typename Type,
          typename = typename std::enable_if<
              std::is_convertible<Type*, IRefBase*>::value>::type>
class EFRefPtr {
 public:
  constexpr EFRefPtr() = default;

  constexpr EFRefPtr(std::nullptr_t) {}

  EFRefPtr(Type* p) : ptr_(p) {
    if (ptr_) {
      ptr_->AddRef();
    }
  }

  EFRefPtr(const EFRefPtr& other) : EFRefPtr(other.ptr_) {}

  template <typename OtherType,
            typename = typename std::enable_if<
                std::is_convertible<OtherType*, Type*>::value>::type>
  EFRefPtr(const EFRefPtr<OtherType>& other) : EFRefPtr(other.ptr_) {}

  EFRefPtr(EFRefPtr&& r) noexcept : ptr_(r.ptr_) { r.ptr_ = nullptr; }

  template <typename OtherType,
            typename = typename std::enable_if<
                std::is_convertible<OtherType*, Type*>::value>::type>
  EFRefPtr(EFRefPtr<OtherType>&& other) noexcept : ptr_(other.ptr_) {
    other.ptr_ = nullptr;
  }

  ~EFRefPtr() {
    if (ptr_ != nullptr) {
      ptr_->Release();
    }
  }

  Type* get() const { return ptr_; }

  Type& operator*() const { return *ptr_; }

  Type* operator->() const { return ptr_; }

  EFRefPtr& operator=(std::nullptr_t) {
    reset();
    return *this;
  }

  EFRefPtr& operator=(Type* p) { return *this = EFRefPtr(p); }

  EFRefPtr& operator=(EFRefPtr other) noexcept {
    swap(other);
    return *this;
  }

  Type** addressof() const { return &ptr_; }

  Type** addressof() { return &ptr_; }

  void reset() { EFRefPtr().swap(*this); }

  Type* release() {
    Type* ptr = ptr_;
    ptr_ = nullptr;
    return ptr;
  }

  void swap(EFRefPtr& other) noexcept { std::swap(ptr_, other.ptr_); }

  explicit operator bool() const { return ptr_ != nullptr; }

  template <typename OtherType>
  bool operator==(const EFRefPtr<OtherType>& other) const {
    return ptr_ == other.get();
  }

  template <typename OtherType>
  bool operator!=(const EFRefPtr<OtherType>& other) const {
    return !operator==(other);
  }

  template <typename OtherType>
  bool operator<(const EFRefPtr<OtherType>& other) const {
    return ptr_ < other.get();
  }

 protected:
  Type* ptr_ = nullptr;
};

}  // namespace common

}  // namespace ef

#endif  // !EASY_FRAMEWORK_COMMON_REFPTR_H_
