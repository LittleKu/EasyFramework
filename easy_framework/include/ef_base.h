#ifndef EASY_FRAMEWORK_INCLUDE_BASE_H_
#define EASY_FRAMEWORK_INCLUDE_BASE_H__

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
}
#endif  // !EASY_FRAMEWORK_INCLUDE_BASE_H__