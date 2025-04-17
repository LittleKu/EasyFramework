/**
 * @file      : taskrunner.cc
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2025-04-16 15:55:00
 * @brief     : 
 */
#include "libef/framework/src/taskrunner.h"

namespace libef {

TaskRunnerImpl::TaskRunnerImpl(
    scoped_refptr<base::SingleThreadTaskRunner> task_runner)
    : task_runner_(task_runner) {}

TaskRunnerImpl::~TaskRunnerImpl() = default;

bool TaskRunnerImpl::IsInCurrentThread() const {
  return task_runner_->BelongsToCurrentThread();
}

void TaskRunnerImpl::PostTask(ITask* task) {
  task_runner_->PostTask(
      FROM_HERE, base::BindOnce(&ITask::Run, scoped_refptr<ITask>(task)));
}

void TaskRunnerImpl::PostDelayedTask(ITask* task, unsigned long long delay_ms) {
  task_runner_->PostDelayedTask(
      FROM_HERE, base::BindOnce(&ITask::Run, scoped_refptr<ITask>(task)),
      base::Milliseconds(delay_ms));
}

}  // namespace libef
