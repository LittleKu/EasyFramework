/**
 * @file      : ef_command_line.h
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-15 09:40:19
 * @brief     :
 */
#ifndef EASY_FRAMEWORK_COMMAND_LINE_H_
#define EASY_FRAMEWORK_COMMAND_LINE_H_

#include "easy_framework/include/ef_base.h"

#define COMMAND_LINE_VERSION MAKE_VERSION(1, 0, 1)

struct IEFCommandLine : public IBaseInterface {
  /**
   * @brief
   *
   * @param out_copy
   * @return true on success, false on failure
   */
  virtual bool CopyTo(IEFCommandLine** out_copy) = 0;

  /**
   * @brief
   *
   * @param argc
   * @param argv
   * @return true on success, false on failure
   */
  virtual bool InitFromArgv(int argc, const char* const* argv) = 0;

  /**
   * @brief
   *
   * @param command_line_string
   * @param length
   * @return true on success, false on failure
   */
  virtual bool InitFromString(const char* command_line_string,
                              unsigned int length) = 0;

  /**
   * @brief Get the Command Line String object
   *
   * @param out_command_line_string
   * @param out_command_line_string_length optional for command line string
   * length
   * @return true on success, false on failure
   */
  virtual bool GetCommandLineString(
      const char** out_command_line_string,
      unsigned int* out_command_line_string_length) const = 0;

  /**
   * @brief
   *
   * @param switch_key
   * @return true on success, false on failure
   */
  virtual bool HasSwitch(const char* switch_key) const = 0;

  /**
   * @brief Get the Switch Value object
   *
   * @param switch_key
   * @param out_value
   * @param out_value_length optional for switch value length
   * @return true on success, false on failure
   */
  virtual bool GetSwitchValue(const char* switch_key,
                              const char** out_value,
                              unsigned int* out_value_length) const = 0;

  /**
   * @brief
   *
   * @param switch_key
   * @return true on success, false on failure
   */
  virtual bool AppendSwitch(const char* switch_key) = 0;

  /**
   * @brief
   *
   * @param switch_key
   * @param switch_value
   * @return true
   * @return false on success, false on failure
   */
  virtual bool AppendSwitchWithValue(const char* switch_key,
                                     const char* switch_value) = 0;

  /**
   * @brief Set the Program object
   * 
   * @param program 
   * @return true on success, false on failure
   */
  virtual bool SetProgram(const char* program) = 0;
};

#endif  // !EASY_FRAMEWORK_COMMAND_LINE_H_
