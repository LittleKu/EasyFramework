/**
 * @file      : ef_task_runner.h
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-12 10:19:10
 * @brief     :
 */
#ifndef EASY_FRAMEWORK_TASK_RUNNER_H_
#define EASY_FRAMEWORK_TASK_RUNNER_H_

#include "easy_framework/include/ef_base.h"

struct ITask : public IRefBase {
  /**
   * @brief Run task
   *
   */
  virtual void Run() = 0;
};

struct ITaskRunner : public IBaseInterface {
  /**
   * @brief Post task
   *
   * @param task
   */
  virtual void PostTask(ITask* task) = 0;

  /**
   * @brief Post delayed task
   *
   * @param task
   * @param ms milliseconds
   */
  virtual void PostDelayedTask(ITask* task, long long ms) = 0;

  /**
   * @brief
   *
   * @param task
   * @param reply
   */
  virtual void PostTaskAndReply(ITask* task, ITask* reply) = 0;

  /**
   * @brief
   *
   * @return true/false
   */
  virtual bool IsRunOnCurrentThread() const = 0;
};

#endif  // !EASY_FRAMEWORK_TASK_RUNNER_H_
