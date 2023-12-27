/**
 * @file      : ef_md5.h
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-26 11:19:18
 * @brief     :
 */
#ifndef EASY_FRAMEWORK_PLUGIN_INCLUDE_MD5_H_
#define EASY_FRAMEWORK_PLUGIN_INCLUDE_MD5_H_

#include "easy_framework/include/ef_base.h"
#include "easy_framework/plugins/include/ef_string.h"

struct IEFMessageDigest : public IBaseInterface {
  /**
   * @brief
   *
   * @return true on success, false on failure
   */
  virtual bool Initialize() = 0;

  /**
   * @brief
   *
   * @param data
   * @param length
   * @return true on success, false on failure
   */
  virtual bool Update(const void* data, size_t length) = 0;

  /**
   * @brief
   *
   * @param out_digest_string
   * @return true on success, false on failure
   */
  virtual bool Final(IEFStringUtf8** out_digest_string) = 0;
};

#endif  // !EASY_FRAMEWORK_PLUGIN_INCLUDE_MD5_H_