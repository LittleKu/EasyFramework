/**
 * @file      : framework.cc
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2025-04-15 20:34:11
 * @brief     : 
 */
#include "libef/framework/src/framework.h"
#include "libef/framework/src/messageloop.h"
#include "libef/framework/src/taskrunner.h"
#include "libef/framework/src/thread.h"

#include "base/check_op.h"
#include "base/task/task_traits.h"
#include "base/task/thread_pool.h"
#include "base/task/thread_pool/thread_pool_instance.h"

#include <string>

namespace libef {

namespace {

base::MessagePumpType MessageLoopTypeToMessagePumpType(MessageLoopType type) {
  switch (type) {
    case MessageLoopType::kMessageLoopTypeDefault:
      return base::MessagePumpType::DEFAULT;
    case MessageLoopType::kMessageLoopTypeIO:
      return base::MessagePumpType::IO;
    case MessageLoopType::kMessageLoopTypeUI:
      return base::MessagePumpType::UI;
    default:
      break;
  }
  return base::MessagePumpType::DEFAULT;
}

}  // namespace

FrameworkImpl::FrameworkImpl() = default;

FrameworkImpl::~FrameworkImpl() = default;

bool FrameworkImpl::QueryInterface(const char* interface_name,
                                   void** interface) {
  if (std::char_traits<char>::compare(
          interface_name, FRAMEWORK_NAME,
          std::char_traits<char>::length(interface_name)) == 0) {
    AddRef();
    *interface = this;
    return true;
  }
  return false;
}

unsigned int FrameworkImpl::GetVersion() const {
  return FRAMEWORK_VERSION;
}

bool FrameworkImpl::Initialize(void* instance, bool create_thread_pool) {
  if (initialized_.load() == false) {
    at_exit_manager_ = std::make_unique<base::AtExitManager>();
    instance_ = reinterpret_cast<Instance>(instance);
    initialized_.store(true);

    if ((create_thread_pool_ = create_thread_pool)) {
        base::ThreadPoolInstance::CreateAndStartWithDefaultParams("Easy_Framework");
    }
    return true;
  }
  return false;
}

void FrameworkImpl::UnInitialize() {
  if (initialized_.load() == true) {
    if (create_thread_pool_) {
      base::ThreadPoolInstance::Get()->Shutdown();
    }
    at_exit_manager_.reset();
    instance_ = nullptr;
    initialized_.store(false);
  }
}

bool FrameworkImpl::CreateMessageLoop(bool nestable,
                                      MessageLoopType type,
                                      IMessageLoop** loop) {
  if (loop == nullptr) {
    return false;
  }

  auto* message_loop =
      new MessageLoopImpl(MessageLoopTypeToMessagePumpType(type),
                          nestable ? base::RunLoop::Type::kNestableTasksAllowed
                                   : base::RunLoop::Type::kDefault);
  if (message_loop == nullptr) {
    return false;
  }
  message_loop->AddRef();
  *loop = message_loop;
  return true;
}

bool FrameworkImpl::CreateThread(MessageLoopType type,
                                 const char* name,
                                 IThread** thread) {
  if (thread == nullptr) {
    return false;
  }
  auto *t = new ThreadImpl(MessageLoopTypeToMessagePumpType(type), name);
  t->AddRef();
  *thread = t;
  return true;
}

bool FrameworkImpl::CreateThreadPoolTaskRunner(ThreadPoolTaskRunnerMode mode,ITaskRunner** task_runner) {
  if (create_thread_pool_) {
    if (task_runner == nullptr) {
      return false;
    }
    scoped_refptr<base::SingleThreadTaskRunner> runner =
        base::ThreadPool::CreateSingleThreadTaskRunner(
            {base::MayBlock()},
            static_cast<base::SingleThreadTaskRunnerThreadMode>(mode));
    if (runner) {
      *task_runner = new TaskRunnerImpl(runner);
      (*task_runner)->AddRef();
      return true;
    }
  }
  return false;
}

}  // namespace libef
