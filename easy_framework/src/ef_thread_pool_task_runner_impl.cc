/**
 * @file      : ef_thread_pool_task_runner_impl.cc
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-12 15:15:07
 * @brief     :
 */
#include "easy_framework/src/ef_thread_pool_task_runner_impl.h"

#include "base/check_op.h"
#include "base/time/time.h"

namespace ef {

constexpr char kUnique[] = "ThreadPoolTaskRunner";

ThreadPoolTaskRunnerImpl::ThreadPoolTaskRunnerImpl(
    scoped_refptr<base::SingleThreadTaskRunner> task_runner)
    : task_runner_{task_runner} {}

ThreadPoolTaskRunnerImpl::~ThreadPoolTaskRunnerImpl() = default;

const char* ThreadPoolTaskRunnerImpl::Unique() const {
  return kUnique;
}

void ThreadPoolTaskRunnerImpl::PostTask(ITask* task) {
  DCHECK(task_runner_);

  task_runner_->PostTask(
      FROM_HERE, base::BindOnce([](scoped_refptr<ITask> t) { t->Run(); },
                                std::move(scoped_refptr<ITask>(task))));
}

void ThreadPoolTaskRunnerImpl::PostDelayedTask(ITask* task, long long ms) {
  DCHECK(task_runner_);

  task_runner_->PostDelayedTask(
      FROM_HERE,
      base::BindOnce([](scoped_refptr<ITask> t) { t->Run(); },
                     std::move(scoped_refptr<ITask>(task))),
      base::Milliseconds(ms));
}

void ThreadPoolTaskRunnerImpl::PostTaskAndReply(ITask* task, ITask* reply) {
  DCHECK(task_runner_);

  task_runner_->PostTaskAndReply(
      FROM_HERE,
      base::BindOnce([](scoped_refptr<ITask> t) { t->Run(); },
                     std::move(scoped_refptr<ITask>(task))),
      base::BindOnce([](scoped_refptr<ITask> r) { r->Run(); },
                     std::move(scoped_refptr<ITask>(reply))));
}

bool ThreadPoolTaskRunnerImpl::IsRunOnCurrentThread() const {
  DCHECK(task_runner_);

  return task_runner_->BelongsToCurrentThread();
}

}  // namespace ef
