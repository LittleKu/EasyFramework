/**
 * @file      : thread.cc
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2025-04-16 17:27:59
 * @brief     : 
 */
#include "libef/framework/src/thread.h"
#include "libef/framework/src/taskrunner.h"

#include "base/threading/thread.h"

namespace libef {

ThreadImpl::ThreadImpl(base::MessagePumpType pump_type, const std::string& name)
    : pump_type_(pump_type) {
  thread_ = std::make_unique<base::Thread>(name);
}

ThreadImpl::~ThreadImpl() {
  Stop();
}

void ThreadImpl::Start() {
  base::Thread::Options options;
  options.message_pump_type = pump_type_;
  thread_->StartWithOptions(std::move(options));
}

void ThreadImpl::Stop() {
    thread_->Stop();
}

bool ThreadImpl::IsRunning() const {
  return thread_->IsRunning();
}

const char* ThreadImpl::GetName() const {
  return thread_->thread_name().c_str();
}

bool ThreadImpl::GetTaskRunner(ITaskRunner** runner) const {
  if (runner == nullptr || thread_->task_runner() == nullptr) {
    return false;
  }

  auto* task_runner = new TaskRunnerImpl(thread_->task_runner());
  task_runner->AddRef();
  *runner = task_runner;
  return true;
}

}  // namespace libef
