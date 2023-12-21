/**
 * @file      : ef_task_runner_wrapper.cc
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-19 09:20:55
 * @brief     :
 */
#include "easy_framework/src/ef_default_task_runner.h"

#include "base/check_op.h"

namespace ef {

IMPL_INTERFACE_UNIQUE(DefaultTaskRunner, IEFTaskRunner)

DefaultTaskRunner::DefaultTaskRunner(
    scoped_refptr<base::SingleThreadTaskRunner> task_runner)
    : task_runner_{task_runner} {
  DCHECK(task_runner_);
}

DefaultTaskRunner::~DefaultTaskRunner() = default;

void DefaultTaskRunner::PostTask(ITask* task) {
  DCHECK(task_runner_);
  task_runner_->PostTask(
      FROM_HERE, base::BindOnce([](ef::common::EFRefPtr<ITask> t) { t->Run(); },
                                ef::common::EFRefPtr<ITask>(task)));
}

void DefaultTaskRunner::PostDelayedTask(ITask* task, long long ms) {
  DCHECK(task_runner_);
  task_runner_->PostDelayedTask(
      FROM_HERE,
      base::BindOnce([](ef::common::EFRefPtr<ITask> t) { t->Run(); },
                     ef::common::EFRefPtr<ITask>(task)),
      base::Milliseconds(ms));
}

void DefaultTaskRunner::PostTaskAndReply(ITask* task, ITask* reply) {
  DCHECK(task_runner_);
  task_runner_->PostTaskAndReply(
      FROM_HERE,
      base::BindOnce([](ef::common::EFRefPtr<ITask> t) { t->Run(); },
                     ef::common::EFRefPtr<ITask>(task)),
      base::BindOnce([](ef::common::EFRefPtr<ITask> r) { r->Run(); },
                     ef::common::EFRefPtr<ITask>(reply)));
}

bool DefaultTaskRunner::IsRunOnCurrentThread() const {
  DCHECK(task_runner_);
  return task_runner_->BelongsToCurrentThread();
}

}  // namespace ef
