/**
 * @file      : ef_thread_pool_task_runner_impl.h
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-12 14:41:30
 * @brief     :
 */
#ifndef EASY_FRAMEWORK_THREAD_POOL_TASK_RUNNER_H_
#define EASY_FRAMEWORK_THREAD_POOL_TASK_RUNNER_H_

#include "easy_framework/common/wrapper/ef_base_interface_wrapper.h"
#include "easy_framework/include/ef_task_runner.h"

#include "base/memory/scoped_refptr.h"
#include "base/task/single_thread_task_runner.h"

namespace ef {

class ThreadPoolTaskRunnerImpl
    : public ef::common::wrapper::BaseInterfaceImpl<ITaskRunner> {
 public:
  explicit ThreadPoolTaskRunnerImpl(
      scoped_refptr<base::SingleThreadTaskRunner> task_runner);
  ~ThreadPoolTaskRunnerImpl() override;

 public:  // override IBaseInterface method
  const char* Unique() const override;

 public:  // override ITaskRunner methods
  void PostTask(ITask* task) override;
  void PostDelayedTask(ITask* task, long long ms) override;
  void PostTaskAndReply(ITask* task, ITask* reply) override;
  bool IsRunOnCurrentThread() const override;

 private:
  scoped_refptr<base::SingleThreadTaskRunner> task_runner_{nullptr};
};

}  // namespace ef
#endif  // !EASY_FRAMEWORK_THREAD_POOL_TASK_RUNNER_H_
