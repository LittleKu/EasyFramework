#ifndef EASY_FRAMEWORK_INCLUDE_BASE_H_
#define EASY_FRAMEWORK_INCLUDE_BASE_H__

struct IWeakObject;

struct IBaseInterface {
  /**
   * @brief
   *
   */
  virtual void AddRef() const = 0;

  /**
   * @brief
   *
   * @return true will release object,otherwise decrement reference
   */
  virtual bool Release() const = 0;

  /**
   * @brief Query interface
   *
   * @param interface_unique interface unique identifier
   * @param out_interface a pointer to the interface pointer
   * @return true on success,false on failure
   */
  virtual bool QueryInterface(const char* interface_unique,
                              IBaseInterface** out_interface) const = 0;

  /**
   * @brief Get the Weak Ref object
   *
   * @param out_weak_ref
   * @return true on success,false on failure
   */
  virtual bool GetWeakRef(IWeakObject** out_weak_ref) const = 0;
};

struct IWeakObject {
  /**
   * @brief
   *
   */
  virtual void AddRef() const = 0;

  /**
   * @brief
   *
   * @return true will release object,otherwise decrement reference
   */
  virtual bool Release() const = 0;

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
  virtual bool Lock(IBaseInterface** strong_ptr) = 0;
};

#endif  // !EASY_FRAMEWORK_INCLUDE_BASE_H__