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
#include <mutex>

static libef::FrameworkImpl* framework = nullptr;
static std::mutex framework_mutex;

namespace libef {

namespace {

// Owned from Initialize until explicit shutdown or final destruction finishes.
// AtExit callbacks can query and release a new instance during either path.
std::atomic_flag at_exit_manager_in_use;

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

void OnBeforeFrameworkDestructLocked() {
  framework = nullptr;
}

}  // namespace

FrameworkImpl::FrameworkImpl(base::OnceCallback<void()> before_destruct)
    : before_destruct_(std::move(before_destruct)) {}

FrameworkImpl::~FrameworkImpl() = default;

bool libef::FrameworkImpl::Release() const {
  {
    std::lock_guard lock(framework_mutex);
    if (ref_count_.fetch_sub(1, std::memory_order_acq_rel) != 1) {
      return false;
    }
    if (!before_destruct_.is_null()) {
      std::move(before_destruct_).Run();
    }
  }
  // Destruction can run AtExit callbacks that query the framework.
  const bool owned_at_exit_manager = at_exit_manager_ != nullptr;
  delete this;
  if (owned_at_exit_manager) {
    at_exit_manager_in_use.clear(std::memory_order_release);
  }
  return true;
}

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
  if (at_exit_manager_in_use.test_and_set(std::memory_order_acquire)) {
    return false;
  }
  if (create_thread_pool && base::ThreadPoolInstance::Get()) {
    at_exit_manager_in_use.clear(std::memory_order_release);
    return false;
  }

  at_exit_manager_ = std::make_unique<base::AtExitManager>();
  instance_ = reinterpret_cast<Instance>(instance);
  if (create_thread_pool) {
    base::ThreadPoolInstance::CreateAndStartWithDefaultParams("Easy_Framework");
  }
  create_thread_pool_.store(create_thread_pool);
  initialized_.store(true);
  return true;
}

void FrameworkImpl::UnInitialize() {
  if (initialized_.exchange(false)) {
    if (create_thread_pool_.exchange(false)) {
      base::ThreadPoolInstance::Get()->Shutdown();
    }
    at_exit_manager_.reset();
    instance_ = nullptr;
    at_exit_manager_in_use.clear(std::memory_order_release);
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

bool FrameworkImpl::CreateThreadPoolTaskRunner(ThreadPoolTaskRunnerMode mode,
                                               ITaskRunner** task_runner) {
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


FRAMEWORK_EXPORT bool QueryInterface(/*[in]*/ const char* interface_name,
                                     /*[out]*/ void** interface) {
    std::lock_guard lock(framework_mutex);
    if (framework == nullptr) {
      framework = new libef::FrameworkImpl(
          base::BindOnce(&libef::OnBeforeFrameworkDestructLocked));
    }
  return framework->QueryInterface(interface_name, interface);
}
