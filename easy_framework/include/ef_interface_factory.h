/**
 * @file      : ef_interface_factory.h
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-18 09:28:32
 * @brief     :
 */
#ifndef EASY_FRAMEWORK_INTERFACE_FACTORY_H_
#define EASY_FRAMEWORK_INTERFACE_FACTORY_H_

#include "easy_framework/include/ef_base.h"

struct IEFInterfaceFactory : public IRefBase {
  /**
   * @brief Create a Object object
   *
   * @param out_object
   * @return true on success, false on failure
   */
  virtual bool CreateObject(IBaseInterface** out_object) = 0;

  /**
   * @brief
   *
   * @param copy_factory
   * @return true on success, false on failure
   */
  virtual bool Clone(IEFInterfaceFactory** copy_factory) = 0;
};

#endif  // !EASY_FRAMEWORK_INTERFACE_FACTORY_H_
