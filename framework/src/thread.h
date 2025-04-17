/**
 * @file      : thread.h
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2025-04-16 17:27:45
 * @brief     : 
 */
#ifndef LIBEF_FRAMEWORK_SRC_THREAD_H
#define LIBEF_FRAMEWORK_SRC_THREAD_H

#include "libef/include/libef.h"
#include "libef/wrapper/ref_impl.h"

#include "base/memory/scoped_refptr.h"
#include "base/message_loop/message_pump_type.h"

#include <memory>
#include <string>

namespace base {
class Thread;
class SingleThreadTaskRunner;
}  // namespace base

namespace libef {

class ThreadImpl : public BaseRefImpl<IThread> {
 public:
  explicit ThreadImpl(base::MessagePumpType pump_type, const std::string& name);
  ~ThreadImpl() override;

  ThreadImpl() = delete;
  ThreadImpl(const ThreadImpl&) = delete;
  ThreadImpl& operator=(const ThreadImpl&) = delete;

 public:
  void Start() override;
  void Stop() override;
  bool IsRunning() const override;
  const char* GetName() const override;
  ThreadId GetThreadId() const override;
  bool GetTaskRunner(ITaskRunner** task_runner) const override;

 private:
  base::MessagePumpType pump_type_{base::MessagePumpType::DEFAULT};
  std::unique_ptr<base::Thread> thread_{nullptr};
  scoped_refptr<base::SingleThreadTaskRunner> task_runner_{nullptr};
};

}  // namespace libef
#endif  // LIBEF_FRAMEWORK_SRC_THREAD_H
