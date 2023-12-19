/**
 * @file      : ef_system_impl.cc
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-07 14:28:36
 * @brief     :
 */
#include "easy_framework/src/ef_system_impl.h"
#include "easy_framework/src/ef_command_line_impl.h"
#include "easy_framework/src/ef_default_task_runner.h"
#include "easy_framework/src/ef_main_ui_message_loop.h"
#include "easy_framework/src/ef_thread_impl.h"

#include "base/check_op.h"
#include "base/command_line.h"
#include "base/task/thread_pool.h"
#include "base/task/thread_pool/initialization_util.h"
#include "base/task/thread_pool/thread_pool_instance.h"

namespace ef {

namespace {

constexpr size_t kThreadPoolDefaultMin = 16;
constexpr size_t kThreadPoolMax = 32;
constexpr double kThreadPoolCoresMultiplier = 0.6;
constexpr size_t kThreadPoolOffset = 0;

void StartThreadPool() {
  base::ThreadPoolInstance::InitParams thread_pool_init_params = {
      base::RecommendedMaxNumberOfThreadsInThreadGroup(
          kThreadPoolDefaultMin, kThreadPoolMax, kThreadPoolCoresMultiplier,
          kThreadPoolOffset)};

  thread_pool_init_params.common_thread_pool_environment = base::
      ThreadPoolInstance::InitParams::CommonThreadPoolEnvironment::COM_MTA;

  base::ThreadPoolInstance::Get()->Start(thread_pool_init_params);
}

}  // namespace

EFSystemImpl::EFSystemImpl()
    : at_exit_manager_(std::make_unique<base::AtExitManager>()),
      main_message_loop_(new ef::EFMainUIMessageLoop()) {
  DCHECK(main_message_loop_);
  main_message_loop_->ConnectInterface(this);

  RegisterInterfaceFactoryInternal<IEFCommandLine, EFCommandLineImpl>(
      INTERFACE_UNIQUE(IEFCommandLine));
  RegisterInterfaceFactoryInternal<IEFThread, EFThreadImpl>(
      INTERFACE_UNIQUE(IEFThread));
}

EFSystemImpl::~EFSystemImpl() {
  DCHECK(at_exit_manager_);
  at_exit_manager_.reset();
}

bool EFSystemImpl::QueryInterface(const char* interface_unique,
                                  IBaseInterface** out_interface) {
  if (!interface_unique || !std::char_traits<char>::length(interface_unique) ||
      out_interface == false) {
    return false;
  }

  {
    base::AutoLock l(interface_factory_map_lock_);
    auto it = interface_factory_map_.find(std::string(interface_unique));
    if (it != interface_factory_map_.end()) {
      IBaseInterface* out = nullptr;
      bool result = it->second->CreateObject(&out);
      if (result) {
        out->ConnectInterface(this);
        *out_interface = out;
        return true;
      }
    }
  }

  if (std::char_traits<char>::compare(
          main_message_loop_->Unique(), interface_unique,
          std::char_traits<char>::length(interface_unique)) == 0) {
    main_message_loop_->AddRef();
    *out_interface = main_message_loop_.get();
    return true;
  }
  return false;
}

bool EFSystemImpl::ConnectInterface(IBaseInterface* host) {
  return false;
}

bool EFSystemImpl::Initialize(void* instance) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  instance_ = reinterpret_cast<HINSTANCE>(instance);

  base::CommandLine::Init(0, nullptr);

  base::ThreadPoolInstance::Create(INTERFACE_UNIQUE(IEFSystem));
  StartThreadPool();

  return true;
}

bool EFSystemImpl::Uninitialize() {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  base::ThreadPoolInstance::Get()->Shutdown();

  DCHECK(!main_message_loop_->IsRunning());
  main_message_loop_.reset();

  return true;
}

bool EFSystemImpl::CreateThreadPoolTaskRunner(IEFTaskRunner** out_task_runner) {
  if (out_task_runner == nullptr) {
    return false;
  }

  if (base::ThreadPoolInstance::Get()) {
    auto* task_runner = new ef::DefaultTaskRunner(
        base::ThreadPool::CreateSingleThreadTaskRunner(
            {base::MayBlock(), base::TaskPriority::USER_VISIBLE}));
    task_runner->AddRef();
    task_runner->ConnectInterface(this);
    *out_task_runner = task_runner;
    return true;
  }
  return false;
}

bool EFSystemImpl::GetCurrentCommandLine(IEFCommandLine** out_command_line) {
  if (out_command_line == nullptr) {
    return false;
  }

  base::CommandLine* current_command_line =
      base::CommandLine::ForCurrentProcess();
  DCHECK(current_command_line);
  auto* c = new ef::EFCommandLineImpl(current_command_line);
  c->ConnectInterface(this);
  c->AddRef();
  *out_command_line = c;

  return true;
}

bool EFSystemImpl::RegisterInterfaceFactory(const char* unique,
                                            IEFInterfaceFactory* factory) {
  if (unique == nullptr || !std::char_traits<char>::length(unique)) {
    return false;
  }

  ef::common::EFRefPtr<IEFInterfaceFactory> factory_copy = nullptr;
  bool result = factory->Clone(factory_copy.addressof());
  if (result == false) {
    return result;
  }
  base::AutoLock l(interface_factory_map_lock_);
  interface_factory_map_.insert(std::make_pair(unique, factory_copy));
  return true;
}

bool EFSystemImpl::UnRegisterInterfaceFactory(const char* unique) {
  if (unique == nullptr || !std::char_traits<char>::length(unique)) {
    return false;
  }

  base::AutoLock l(interface_factory_map_lock_);
  auto it = interface_factory_map_.find(std::string(unique));
  if (it == interface_factory_map_.end()) {
    return false;
  }

  interface_factory_map_.erase(it);
  return true;
}

bool EFSystemImpl::GetMainMessageLoop(IEFMessageLoop** out_message_loop) {
  if (out_message_loop == nullptr) {
    return false;
  }

  DCHECK(main_message_loop_);
  main_message_loop_->AddRef();
  *out_message_loop = main_message_loop_.get();
  return true;
}

}  // namespace ef
