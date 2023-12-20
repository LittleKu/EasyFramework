/**
 * @file      : ef_thread.h
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-19 20:06:37
 * @brief     :
 */
#ifndef EASY_FRAMEWORK_THREAD_H_
#define EASY_FRAMEWORK_THREAD_H_

#include "easy_framework/include/ef_base.h"
#include "easy_framework/include/ef_message_loop.h"

struct IEFThread : public IBaseInterface {
  /**
   * @brief
   *
   * @param name
   * @param message_type MessageLoopType
   * @return true on success, false on failure
   */
  virtual bool Start(const char* name, int message_type) = 0;

  /**
   * @brief
   *
   */
  virtual void Stop() = 0;

  /**
   * @brief
   *
   * @return true on running, otherwise false
   */
  virtual bool IsRunning() const = 0;

  /**
   * @brief Get the Task Runner object
   *
   * @param out_task_runner
   * @return true on success, false on failure
   */
  virtual bool GetTaskRunner(IEFTaskRunner** out_task_runner) = 0;

  /**
   * @brief Get current thread id
   *
   * @return return the current thread id
   */
  virtual ThreadId GetThreadId() const = 0;
};

#endif  // !EASY_FRAMEWORK_THREAD_H_
