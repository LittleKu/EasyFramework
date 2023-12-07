#ifndef EASY_FRAMEWORK_SYSTEM_H_
#define EASY_FRAMEWORK_SYSTEM_H_

#include "easy_framework/include/ef_base.h"

struct IEFSystem : public IBaseInterface {
  /**
   * @brief 
   * 
   * @param instance process instance
   * @return true on success, false on failure
   */
  virtual bool Initialize(void* instance) = 0;

  /**
   * @brief 
   * 
   * @return true on success, false on failure
   */
  virtual bool Uninitialize() = 0;
};

#endif  // !EASY_FRAMEWORK_SYSTEM_H_