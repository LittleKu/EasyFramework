/**
 * @file      : ef_base.h
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-07 14:14:23
 * @brief     :
 */
#ifndef EASY_FRAMEWORK_INCLUDE_BASE_H_
#define EASY_FRAMEWORK_INCLUDE_BASE_H_

struct IWeakRef;

struct IRefBase {
  /**
   * @brief
   *
   */
  virtual void AddRef() const = 0;

  /**
   * @brief
   *
   * @return true
   * @return false
   */
  virtual bool Release() const = 0;

  /**
   * @brief Get the Weak Ref object
   *
   * @param out_weak_ref
   * @return true on success, false on failure
   */
  virtual bool GetWeakRef(IWeakRef** out_weak_ref) const = 0;
};

struct IBaseInterface : public IRefBase {
  /**
   * @brief Get Interface unique identifier
   *
   * @return
   */
  virtual const char* Unique() const = 0;

  /**
   * @brief return the interface version
   *
   * @return unsigned int
   */
  virtual unsigned int Version() const = 0;

  /**
   * @brief Query interface
   *
   * @param interface_unique interface unique identifier
   * @param out_interface a pointer to the interface pointer
   * @return true on success,false on failure
   */
  virtual bool QueryInterface(const char* interface_unique,
                              IBaseInterface** out_interface) = 0;

  /**
   * @brief Connect to the host interface
   *
   * @param host interface that can query this interface
   * @return true on success, false on failure
   */
  virtual bool ConnectInterface(IBaseInterface* host) = 0;
};

struct IWeakRef : public IRefBase {
  /**
   * @brief Override and disable this method on Weak reference object
   *
   */
  bool GetWeakRef(IWeakRef**) const final {
    // disable
    return false;
  }

  /**
   * @brief Detecting the validity of the host pointer.
   *
   * @return true on invalid, otherwise valid.
   */
  virtual bool Expired() const = 0;

  /**
   * @brief Get the host pointer as a strong reference
   *
   * @param strong_ptr
   * @return true success, otherwise failure
   */
  virtual bool Lock(IRefBase** strong_ptr) = 0;
};

#define DECLARE_INTERFACE_UNIQUE(ClassName) \
 public:                                    \
  const char* Unique() const override

#define IMPL_INTERFACE_UNIQUE(ClassName, UniqueName) \
  const char* ClassName::Unique() const {            \
    return #UniqueName;                              \
  }

#define INTERFACE_UNIQUE(UniqueType) #UniqueType

#define MAKE_16BIT(a, b) \
  ((unsigned short)(((unsigned char)(((unsigned int)(a)) & 0xff)) | \
  ((unsigned short)((unsigned char)(((unsigned int)(b)) & 0xff))) << 8))

#define MAKE_32BIT(a, b) \
((unsigned int)(((unsigned short)(((unsigned int)(a)) & 0xffff)) |  \
((unsigned int)((unsigned short)(((unsigned int)(b)) & 0xffff))) << 16))

#define MAKE_VERSION(major /* byte */, minor /* byte */, build /* short */) \
  MAKE_32BIT(MAKE_16BIT(major, minor), build)

#if defined(_WIN32)
#include "easy_framework/include/win/ef_win.h"
#else
// TODO support for Mac OS X
#endif  // cross platform support

#endif  // !EASY_FRAMEWORK_INCLUDE_BASE_H_
