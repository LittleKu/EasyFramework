/**
 * @file      : taskrunner.h
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2025-04-16 15:54:03
 * @brief     : 
 */
#ifndef LIBEF_FRAMEWORK_SRC_TASKRUNNER_H
#define LIBEF_FRAMEWORK_SRC_TASKRUNNER_H

#include "libef/include/libef.h"
#include "libef/wrapper/ref_impl.h"

#include "base/task/single_thread_task_runner.h"

namespace libef {

class TaskRunnerImpl : public BaseRefImpl<ITaskRunner> {
 public:
  explicit TaskRunnerImpl(scoped_refptr<base::SingleThreadTaskRunner> task_runner);
  ~TaskRunnerImpl() override;

 public:  // override TaskRunner
  bool IsInCurrentThread() const override;
  void PostTask(ITask* task) override;
  void PostDelayedTask(ITask* task, unsigned long long delay_ms) override;

 private:
  scoped_refptr<base::SingleThreadTaskRunner> task_runner_;
};

}  // namespace libef
#endif // LIBEF_FRAMEWORK_SRC_TASKRUNNER_H