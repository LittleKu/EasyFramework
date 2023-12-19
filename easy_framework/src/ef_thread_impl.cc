/**
 * @file      : ef_thread_impl.cc
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-19 20:14:24
 * @brief     :
 */
#include "easy_framework/src/ef_thread_impl.h"
#include "easy_framework/src/ef_default_task_runner.h"

#include "base/check_op.h"
#include "base/message_loop/message_pump.h"
#include "base/notreached.h"

#include <functional>
#include <string>

namespace ef {

namespace {

base::MessagePumpType ToMessagePumpType(int message_type) {
  switch (message_type) {
    case MessageLoopType::Default:
      return base::MessagePumpType::DEFAULT;
    case MessageLoopType::IO:
      return base::MessagePumpType::IO;
    case MessageLoopType::UI:
      return base::MessagePumpType::UI;
    default:
      NOTREACHED();
      break;
  }
  return base::MessagePumpType::DEFAULT;
}

}  // namespace

EFThreadImpl::EFThreadImpl() = default;

EFThreadImpl::~EFThreadImpl() = default;

bool EFThreadImpl::Start(const char* name, int message_type) {
  if (name == nullptr || !std::char_traits<char>::length(name)) {
    return false;
  }

  if (IsRunning()) {
    return true;
  }

  std::call_once(create_flag_,
                 [thread_name = std::string(name),
                  lifetime = ef::common::EFRefPtr<IEFThread>(this), this] {
                   thread_ = std::make_unique<base::Thread>(thread_name);
                 });
  DCHECK(thread_ != nullptr);
  base::Thread::Options options;
  options.message_pump_type =
      ToMessagePumpType(static_cast<MessageLoopType>(message_type));
  return thread_->StartWithOptions(std::move(options));
}

void EFThreadImpl::Stop() {
  if (thread_ != nullptr) {
    thread_->Stop();
  }
}

bool EFThreadImpl::IsRunning() const {
  return thread_ != nullptr && thread_->IsRunning();
}

bool EFThreadImpl::GetTaskRunner(IEFTaskRunner** out_task_runner) {
  if (IsRunning()) {
    scoped_refptr<base::SingleThreadTaskRunner> task_runner =
        thread_->task_runner();
    auto* task_runner_wrapper = new ef::DefaultTaskRunner(task_runner);
    task_runner_wrapper->ConnectInterface(this);
    task_runner_wrapper->AddRef();
    *out_task_runner = task_runner_wrapper;
    return true;
  }
  return false;
}

}  // namespace ef
