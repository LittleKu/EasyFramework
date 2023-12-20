/**
 * @file      : ef_thread_impl.h
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-19 20:14:21
 * @brief     : 
 */
#ifndef EASY_FRAMEWORK_THREAD_IMPL_H_
#define EASY_FRAMEWORK_THREAD_IMPL_H_

#include "easy_framework/include/ef_thread.h"
#include "easy_framework/common/wrapper/ef_base_interface_wrapper.h"

#include "base/threading/thread.h"

#include <memory>

namespace ef{

class EFThreadImpl : public ef::common::wrapper::BaseInterfaceImpl<IEFThread> {
 public:
  EFThreadImpl();
  ~EFThreadImpl() override;

  DECLARE_INTERFACE_UNIQUE(IEFThread)

 public:  // override IEFThread methods
  bool Start(const char* name, int message_type) override;
  void Stop() override;
  bool IsRunning() const override;
  bool GetTaskRunner(IEFTaskRunner** out_task_runner) override;
  ThreadId GetThreadId() const override;

 private:
  std::unique_ptr<base::Thread> thread_{nullptr};
  std::once_flag create_flag_ = {};
};

}  // namespace ef
#endif  // !EASY_FRAMEWORK_THREAD_IMPL_H_
