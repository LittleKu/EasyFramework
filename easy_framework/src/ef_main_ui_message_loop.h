/**
 * @file      : ef_main_ui_message_loop.h
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-18 20:56:44
 * @brief     :
 */
#ifndef EASY_FRAMEWORK_MAIN_UI_MESSAGE_LOOP_H_
#define EASY_FRAMEWORK_MAIN_UI_MESSAGE_LOOP_H_

#include "base/functional/callback.h"
#include "base/task/sequence_manager/sequence_manager.h"

#include "easy_framework/common/wrapper/ef_base_interface_wrapper.h"
#include "easy_framework/include/ef_message_loop.h"

namespace ef {


class EFMainUIMessageLoop final
    : public ef::common::wrapper::BaseInterfaceImpl<IEFMessageLoop> {
 public:
  EFMainUIMessageLoop();
  ~EFMainUIMessageLoop() override;

  DECLARE_INTERFACE_UNIQUE(IEFMessageLoop)

 public:  // override IMessageLoop methods
  int Run() final;
  void Quit() final;
  bool CreateTaskRunner(IEFTaskRunner** out_task_runner) final;
  int Type() const final;
  bool IsRunning() const final;

 private:
  std::unique_ptr<base::sequence_manager::SequenceManager>
      owned_sequence_manager_{nullptr};
  base::sequence_manager::TaskQueue::Handle default_task_queue_;
  base::OnceClosure quit_when_idle_closure_;
};

}  // namespace ef

#endif  // !EASY_FRAMEWORK_MAIN_UI_MESSAGE_LOOP_H_
