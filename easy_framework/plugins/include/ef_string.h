/**
 * @file      : ef_string.h
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-26 11:37:38
 * @brief     :
 */
#ifndef EASY_FRAMEWORK_PLUGIN_INCLUDE_STRING_H_
#define EASY_FRAMEWORK_PLUGIN_INCLUDE_STRING_H_

#include "easy_framework/include/ef_base.h"

typedef char utf8;

struct IEFStringWide;
struct IEFStringUtf8 : public IRefBase {
  /**
   * @brief
   *
   * @return const utf8*
   */
  virtual const utf8* String() const = 0;

  /**
   * @brief
   *
   * @return size_t
   */
  virtual size_t Size() const = 0;

  /**
   * @brief
   *
   * @param s
   * @param length
   */
  virtual void Reset(const utf8* s, size_t length) = 0;

  /**
   * @brief
   *
   * @param s
   * @param length
   */
  virtual void Append(const utf8* s, size_t length) = 0;

  /**
   * @brief
   *
   * @param out_wide
   * @return true on success, false on failure
   */
  virtual bool ToWide(IEFStringWide** out_wide) = 0;
};

struct IEFStringWide : public IRefBase {
  /**
   * @brief
   *
   * @return const wchar_t*
   */
  virtual const wchar_t* String() const = 0;

  /**
   * @brief
   *
   * @return size_t
   */
  virtual size_t Size() const = 0;

  /**
   * @brief
   *
   * @param s
   * @param length
   */
  virtual void Reset(const wchar_t* s, size_t length) = 0;

  /**
   * @brief
   *
   * @param s
   * @param length
   */
  virtual void Append(const wchar_t* s, size_t length) = 0;

  /**
   * @brief
   *
   * @param out_utf8
   * @return true on success, false on failure
   */
  virtual bool ToUtf8(IEFStringUtf8** out_utf8) = 0;
};

struct IEFStringFactory : public IBaseInterface {
  /**
   * @brief
   *
   * @param str
   * @param length
   * @param out_string
   * @return true on success, false on failure
   */
  virtual bool AllocateUtf8(const utf8* str,
                            size_t length,
                            IEFStringUtf8** out_string) = 0;

  /**
   * @brief
   *
   * @param str
   * @param length
   * @param out_string
   * @return true on success, false on failure
   */
  virtual bool AllocateWide(const wchar_t* str,
                            size_t length,
                            IEFStringWide** out_string) = 0;
};

#endif  // !EASY_FRAMEWORK_PLUGIN_INCLUDE_STRING_H_