/**
 * @file      : ref_ptr.h
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2025-04-17 11:14:55
 * @brief     : 
 */
#ifndef LIBEF_WRAPPER_REF_PTR_H
#define LIBEF_WRAPPER_REF_PTR_H

#include <type_traits>

namespace libef {

template <typename T>
class ref_ptr {
 protected:
  T* ptr_ = nullptr;

  template <typename U>
  friend class ref_ptr;

  void InternalAddRef() const noexcept {
    if (ptr_) {
      ptr_->AddRef();
    }
  }

  void InternalRelease() noexcept {
    if (ptr_) {
      ptr_->Release();
    }
  }

 public:
  using element_type = T;
  constexpr ref_ptr() noexcept = default;

  constexpr ref_ptr(std::nullptr_t) noexcept : ptr_(nullptr) {}

  template <typename U>
  ref_ptr(U* ptr) noexcept : ptr_(ptr) {
    InternalAddRef();
  }

  ref_ptr(const ref_ptr& other) noexcept : ptr_(other.ptr_) {
    InternalAddRef();
  }

  template <typename U>
    requires std::is_convertible_v<U*, T*>
  ref_ptr(const ref_ptr<U>& other) noexcept : ptr_(other.ptr_) {
    InternalAddRef();
  }

  ref_ptr(ref_ptr&& other) noexcept : ptr_(nullptr) {
    if (this !=
        reinterpret_cast<ref_ptr*>(&reinterpret_cast<unsigned char&>(other))) {
      swap(other);
    }
  }

  template <typename U>
    requires std::is_convertible_v<U*, T*>
  ref_ptr(ref_ptr<U>&& other) noexcept : ptr_(other.ptr_) {
    other.ptr_ = nullptr;
  }

  ~ref_ptr() noexcept { InternalRelease(); }

  ref_ptr& operator=(std::nullopt_t) noexcept {
    InternalRelease();
    return *this;
  }

  ref_ptr& operator=(T* other) noexcept {
    if (ptr_ != other) {
      raw_ref(other).swap(*this);
    }
    return *this;
  }

  template <typename U>
  ref_ptr& operator=(U* other) noexcept {
    ref_ptr(other).swap(*this);
    return *this;
  }

  ref_ptr& operator=(const ref_ptr& other) noexcept {
    if (ptr_ != other.ptr_) {
      ref_ptr(other).swap(*this);
    }
    return *this;
  }

  template <typename U>
  ref_ptr& operator=(const ref_ptr<U>& other) noexcept {
    ref_ptr(other).swap(*this);
    return *this;
  }

  ref_ptr& operator=(ref_ptr&& other) noexcept {
    ref_ptr(static_cast<ref_ptr&&>(other)).swap(*this);
    return *this;
  }

  template <typename U>
  ref_ptr& operator=(ref_ptr<U>&& other) noexcept {
    ref_ptr(static_cast<ref_ptr<U>&&>(other)).swap(*this);
    return *this;
  }

  void swap(ref_ptr&& other) noexcept {
    T* tmp = ptr_;
    ptr_ = other.ptr_;
    other.ptr_ = tmp;
  }

  void swap(ref_ptr& other) noexcept {
    T* tmp = ptr_;
    ptr_ = other.ptr_;
    other.ptr_ = tmp;
  }

  T* get() const noexcept { return ptr_; }

  explicit operator bool() const noexcept { return ptr_ != nullptr; }

  T* operator->() const noexcept { return ptr_; }

  T* const* addressof() const noexcept { return &ptr_; }

  T** addressof() noexcept { return &ptr_; }

  T* detach() noexcept {
    T* ptr = ptr_;
    ptr_ = nullptr;
    return ptr;
  }

  void attach(T* other) noexcept {
    if (ptr_ != nullptr) {
      ptr_->Release();
    }
    ptr_ = other;
  }

  void reset() noexcept { ref_ptr().swap(*this); }
};

}  // namespace libef

#endif // LIBEF_WRAPPER_REF_PTR_H