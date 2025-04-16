/**
 * @file      : messageloop.h
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2025-04-15 21:55:06
 * @brief     : 
 */
#ifndef LIBEF_FRAMEWORK_SRC_MESSAGELOOP_H
#define LIBEF_FRAMEWORK_SRC_MESSAGELOOP_H

#include "libef/include/libef.h"
#include "libef/wrapper/ref_impl.h"

#include "base/functional/callback.h"
#include "base/message_loop/message_pump_type.h"
#include "base/run_loop.h"
#include "base/task/sequence_manager/task_queue.h"

#include <stack>

namespace base {
namespace sequence_manager {
class SequenceManager;
}  // namespace sequence_manager
}  // namespace base

namespace libef {

class MessageLoopImpl : public BaseRefImpl<MessageLoop> {
 public:
  explicit MessageLoopImpl(
      base::MessagePumpType pump_type = base::MessagePumpType::DEFAULT,
      base::RunLoop::Type type = base::RunLoop::Type::kDefault);
  ~MessageLoopImpl() override;

 public:  // override BaseInterface
  bool QueryInterface(const char* interface_name, void** interface) override;
  unsigned int GetVersion() const override;

 public:  // override MessageLoop
  int Run() override;
  void Quit() override;
  MessageLoopType GetType() override;
  bool IsNestable() const override;

 private:
  std::unique_ptr<base::sequence_manager::SequenceManager> sequence_manager_;
  base::MessagePumpType pump_type_;
  const base::RunLoop::Type run_loop_type_;
  base::sequence_manager::TaskQueue::Handle task_queue_;
  base::OnceClosure no_nested_quit_closure_;
  std::stack<base::OnceClosure> nested_quit_closure_stack_;
};

}  // namespace libef

#endif // LIBEF_FRAMEWORK_SRC_MESSAGELOOP_H