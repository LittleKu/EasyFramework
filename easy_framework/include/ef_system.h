/**
 * @file      : ef_system.h
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-07 14:14:57
 * @brief     :
 */
#ifndef EASY_FRAMEWORK_SYSTEM_H_
#define EASY_FRAMEWORK_SYSTEM_H_

#include "easy_framework/include/ef_base.h"
#include "easy_framework/include/ef_command_line.h"
#include "easy_framework/include/ef_interface_factory.h"
#include "easy_framework/include/ef_message_loop.h"
#include "easy_framework/include/ef_task_runner.h"

struct IEFSystem : public IBaseInterface {
  /**
   * @brief not thread safe, must be called on the main thread
   *
   * @param instance process instance
   * @param plugin_path plugin path
   * @return true on success, false on failure
   */
  virtual bool Initialize(void* instance, const char* plugin_path) = 0;

  /**
   * @brief not thread safe, must be called on the main thread
   *
   * @return true on success, false on failure
   */
  virtual bool Uninitialize() = 0;

  /**
   * @brief Create a task runner base on thread pool due to the
   * {create_thread_pool} is true
   *
   * @param out_task_runner
   * @return true on success, false on failure
   */
  virtual bool CreateThreadPoolTaskRunner(IEFTaskRunner** out_task_runner) = 0;

  /**
   * @brief Get the current process command line object
   *
   * @param out_command_line
   * @return true on success, false on failure
   */
  virtual bool GetCurrentCommandLine(IEFCommandLine** out_command_line) = 0;

  /**
   * @brief
   *
   * @param unique
   * @param factory
   * @return true on success, false on failure
   */
  virtual bool RegisterInterfaceFactory(const char* unique,
                                        IEFInterfaceFactory* factory) = 0;
  /**
   * @brief
   *
   * @param unique
   * @return true on success, false on failure
   */
  virtual bool UnRegisterInterfaceFactory(const char* unique) = 0;

  /**
   * @brief Get the Main Message Loop object
   *
   * @param out_message_loop
   * @return true on success, false on failure
   */
  virtual bool GetMainMessageLoop(IEFMessageLoop** out_message_loop) = 0;

  /**
   * @brief
   *
   * @param file_path
   * @return true
   * @return false
   */
  virtual bool LoadPluginManual(const char* file_path) = 0;
};

#endif  // !EASY_FRAMEWORK_SYSTEM_H_
