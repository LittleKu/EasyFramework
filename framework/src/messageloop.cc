/**
 * @file      : messageloop.cc
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2025-04-15 21:56:00
 * @brief     : 
 */
#include "libef/framework/src/messageloop.h"
#include "libef/framework/src/taskrunner.h"

#include "base/logging.h"
#include "base/task/sequence_manager/sequence_manager.h"
#include "base/task/sequence_manager/sequence_manager_impl.h"

namespace libef {

MessageLoopImpl::MessageLoopImpl(
    base::MessagePumpType pump_type /* =base::MessagePumpType::DEFAULT */,
    base::RunLoop::Type type /* =base::RunLoop::Type::kDefault */)
    : sequence_manager_(base::sequence_manager::CreateUnboundSequenceManager(
          base::sequence_manager::SequenceManager::Settings::Builder()
              .SetMessagePumpType(pump_type)
              .Build())),
      pump_type_(pump_type),
      run_loop_type_(type),
      task_queue_(sequence_manager_->CreateTaskQueue(
          base::sequence_manager::TaskQueue::Spec(
              base::sequence_manager::QueueName::DEFAULT_TQ))) {}

MessageLoopImpl::~MessageLoopImpl() = default;

bool MessageLoopImpl::QueryInterface(const char* interface_name,
                                     void** interface) {
  if (std::char_traits<char>::compare(
          interface_name, TASK_RUNNER_NAME,
          std::char_traits<char>::length(interface_name)) == 0) {
    auto* task_runner = new TaskRunnerImpl(task_queue_->task_runner());
    task_runner->AddRef();
    *interface = task_runner;
    return true;
  }
  return false;
}

unsigned int MessageLoopImpl::GetVersion() const {
  return MESSAGE_LOOP_VERSION;
}

int MessageLoopImpl::Run() {
  base::RunLoop run_loop(run_loop_type_);
  if (!IsNestable()) {
    no_nested_quit_closure_ = run_loop.QuitWhenIdleClosure();
  } else {
    nested_quit_closure_stack_.push(run_loop.QuitWhenIdleClosure());
  }
  run_loop.Run();
  return 0;
}

void MessageLoopImpl::Quit() {
  if (IsNestable()) {
    if (nested_quit_closure_stack_.empty()) {
      return;
    }
    base::OnceClosure closure = std::move(nested_quit_closure_stack_.top());
    nested_quit_closure_stack_.pop();
    if (closure.is_null()) {
      return;
    }
    std::move(closure).Run();
  } else {
    if (no_nested_quit_closure_.is_null()) {
      return;
    }
    std::move(no_nested_quit_closure_).Run();
  }
}

MessageLoopType MessageLoopImpl::GetType() {
  switch (pump_type_) {
    case base::MessagePumpType::DEFAULT:
      return MessageLoopType::kMessageLoopTypeDefault;
    case base::MessagePumpType::IO:
      return MessageLoopType::kMessageLoopTypeIO;
    case base::MessagePumpType::UI:
      return MessageLoopType::kMessageLoopTypeUI;
    default:
      break;
  }
  DLOG(ERROR) << "Unknown MessagePumpType";
  return MessageLoopType::kMessageLoopTypeDefault;
}

bool MessageLoopImpl::IsNestable() const {
  return run_loop_type_ == base::RunLoop::Type::kNestableTasksAllowed;
}

}  // namespace libef
