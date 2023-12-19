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
   * @brief Deteting the validity of the host pointer.
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

#define DECLARE_INTERFACE_UNIQUE(UniqueType) \
 public:                                     \
  const char* Unique() const override {      \
    return #UniqueType;                      \
  }

#define INTERFACE_UNIQUE(UniqueType)  #UniqueType

#endif  // !EASY_FRAMEWORK_INCLUDE_BASE_H_
