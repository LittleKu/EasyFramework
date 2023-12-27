/**
 * @file      : ef_default_task_runner.h
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-19 09:15:48
 * @brief     :
 */
#ifndef EASY_FRAMEWORK_DEFAULT_TASK_RUNNER_H_
#define EASY_FRAMEWORK_DEFAULT_TASK_RUNNER_H_

#include "easy_framework/common/wrapper/ef_base_interface_wrapper.h"
#include "easy_framework/include/ef_task_runner.h"

#include "base/memory/scoped_refptr.h"
#include "base/task/single_thread_task_runner.h"

namespace ef {


class DefaultTaskRunner final
    : public ef::common::wrapper::RefBaseImpl<IEFTaskRunner> {
 public:
  explicit DefaultTaskRunner(
      scoped_refptr<base::SingleThreadTaskRunner> task_runner);
  ~DefaultTaskRunner() override;

 public:  // override ITaskRunner methods
  void PostTask(ITask* task) override;
  void PostDelayedTask(ITask* task, long long ms) override;
  void PostTaskAndReply(ITask* task, ITask* reply) override;
  bool IsRunOnCurrentThread() const override;

 private:
  scoped_refptr<base::SingleThreadTaskRunner> task_runner_{nullptr};
};

}  // namespace ef
#endif  // !EASY_FRAMEWORK_DEFAULT_TASK_RUNNER_H_
