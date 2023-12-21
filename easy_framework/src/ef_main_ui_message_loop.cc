/**
 * @file      : ef_main_ui_message_loop.cc
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-18 20:56:27
 * @brief     :
 */
#include "easy_framework/src/ef_main_ui_message_loop.h"
#include "easy_framework/src/ef_default_task_runner.h"

#include "base/message_loop/message_pump.h"
#include "base/run_loop.h"
#include "ef_main_ui_message_loop.h"


namespace ef {

IMPL_INTERFACE_UNIQUE(EFMainUIMessageLoop, IEFMessageLoop)

EFMainUIMessageLoop::EFMainUIMessageLoop()
    : owned_sequence_manager_(
          base::sequence_manager::CreateUnboundSequenceManager(
              base::sequence_manager::SequenceManager::Settings::Builder()
                  .SetMessagePumpType(base::MessagePumpType::UI)
                  .Build())),
      default_task_queue_(owned_sequence_manager_->CreateTaskQueue(
          base::sequence_manager::TaskQueue::Spec(
              base::sequence_manager::QueueName::DEFAULT_TQ))) {
  owned_sequence_manager_->SetDefaultTaskRunner(
      default_task_queue_->task_runner());
  owned_sequence_manager_->BindToMessagePump(
      base::MessagePump::Create(base::MessagePumpType::UI));
}

EFMainUIMessageLoop::~EFMainUIMessageLoop() = default;

int EFMainUIMessageLoop::Run() {
  base::RunLoop loop;
  quit_when_idle_closure_ = loop.QuitWhenIdleClosure();
  loop.Run();
  return 0;
}

void EFMainUIMessageLoop::Quit() {
  if (!quit_when_idle_closure_.is_null()) {
    std::move(quit_when_idle_closure_).Run();
  }
}

bool EFMainUIMessageLoop::CreateTaskRunner(IEFTaskRunner** out_task_runner) {
  if (out_task_runner == nullptr) {
    return false;
  }

  DCHECK(default_task_queue_);
  scoped_refptr<base::SingleThreadTaskRunner> task_runner =
      default_task_queue_->CreateTaskRunner(
          base::sequence_manager::kTaskTypeNone);

  auto* task_runner_wrapper = new ef::DefaultTaskRunner(task_runner);
  task_runner_wrapper->ConnectInterface(this);
  task_runner_wrapper->AddRef();
  *out_task_runner = task_runner_wrapper;
  return true;
}

int EFMainUIMessageLoop::Type() const {
  return static_cast<int>(MessageLoopType::UI);
}

bool EFMainUIMessageLoop::IsRunning() const {
  return !quit_when_idle_closure_.is_null();
}
}  // namespace ef
