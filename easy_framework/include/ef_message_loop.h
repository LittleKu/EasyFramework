/**
 * @file      : ef_message_loop.h
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-12 10:18:57
 * @brief     :
 */
#ifndef EASY_FRAMEWORK_MESSAGE_LOOP_H_
#define EASY_FRAMEWORK_MESSAGE_LOOP_H_

#include "easy_framework/include/ef_base.h"
#include "easy_framework/include/ef_task_runner.h"

enum MessageLoopType {
  Default = 0,
  UI,
  IO,
};

struct IEFMessageLoop : public IBaseInterface {
  /**
   * @brief run message loop
   *
   * @return return message loop exit code
   */
  virtual int Run() = 0;

  /**
   * @brief
   *
   */
  virtual void Quit() = 0;

  /**
   * @brief Create a Task Runner object base on current message loop
   *
   * @param out_task_runner
   * @return truen on success, otherwise failure
   */
  virtual bool CreateTaskRunner(IEFTaskRunner** out_task_runner) = 0;

  /**
   * @brief
   *
   * @return MessageLoopType
   */
  virtual int Type() const = 0;

  /**
   * @brief
   *
   * @return true on running, otherwise false
   */
  virtual bool IsRunning() const = 0;
};

#endif  // !EASY_FRAMEWORK_MESSAGE_LOOP_H_
