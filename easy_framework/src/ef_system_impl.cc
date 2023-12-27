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
#include "base/files/file_enumerator.h"
#include "base/files/file_util.h"
#include "base/path_service.h"
#include "base/task/thread_pool.h"
#include "base/task/thread_pool/initialization_util.h"
#include "base/task/thread_pool/thread_pool_instance.h"
#include "base/threading/thread_restrictions.h"

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

struct LibraryDeleter {
  using pointer = base::NativeLibrary;
  void operator()(base::NativeLibrary h) { base::UnloadNativeLibrary(h); }
};

constexpr wchar_t kPluginSubPath[] = FILE_PATH_LITERAL("plugin");
}  // namespace

IMPL_INTERFACE_UNIQUE(EFSystemImpl, IEFSystem)

EFSystemImpl::EFSystemImpl()
    : at_exit_manager_(std::make_unique<base::AtExitManager>()),
      main_message_loop_(new ef::EFMainUIMessageLoop()) {
  DCHECK(main_message_loop_);
  main_message_loop_->ConnectInterface(this);

  RegisterInterfaceFactoryInternal<IEFCommandLine, EFCommandLineImpl>(
      INTERFACE_UNIQUE(IEFCommandLine));
  RegisterInterfaceFactoryInternal<IEFThread, EFThreadImpl>(
      INTERFACE_UNIQUE(IEFThread));

  // default plugin path
  base::FilePath current_path;
  base::PathService::Get(base::DIR_MODULE, &current_path);
  plugin_path_ = current_path.Append(kPluginSubPath);
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

  if (std::char_traits<char>::compare(
          Unique(), interface_unique,
          std::char_traits<char>::length(interface_unique)) == 0) {
    AddRef();
    *out_interface = const_cast<EFSystemImpl*>(this);
    return true;
  }

  //first query plugins
  {
    base::AutoLock l(plugin_lock_);
    for (const auto& plugin : plugins_) {
      IBaseInterface* out = nullptr;
      HRESULT hr =
          plugin.second.get()->fnQueryInterface(interface_unique, &out);
      if (hr == S_OK) {
        out->ConnectInterface(this);
        *out_interface = out;
        return true;
      }
    }
  }

  //internal interface
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

bool EFSystemImpl::Initialize(void* instance, const char* plugin_path) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  instance_ = reinterpret_cast<HINSTANCE>(instance);

  base::CommandLine::Init(0, nullptr);

  base::ThreadPoolInstance::Create(INTERFACE_UNIQUE(IEFSystem));
  StartThreadPool();

  if (plugin_path && std::char_traits<char>::length(plugin_path)) {
    base::FilePath new_plugin_path =
        base::FilePath::FromUTF8Unsafe(std::string(plugin_path));
    base::ScopedAllowBlockingForTesting allow_io;
    if (base::DirectoryExists(new_plugin_path)) {
      plugin_path_ = new_plugin_path;
    }
  }

  return LoadPlugins();
}

bool EFSystemImpl::Uninitialize() {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  {
    base::AutoLock l(plugin_lock_);
    for (auto& plugin : plugins_) {
      if (0 == plugin.second->fnCanUnloadPlugin()) {
        base::UnloadNativeLibrary(plugin.second.get()->plugin);
      } else {
        DCHECK(false) << "plugin :" << plugin.second->file_path
                      << " could not unload at the time";
      }
    }
    plugins_.clear();
  }

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

bool EFSystemImpl::LoadPluginManual(const char* file_path) {
  if (!file_path || !std::char_traits<char>::length(file_path)) {
    return false;
  }

  base::FilePath plugin_file_path =
      base::FilePath::FromUTF8Unsafe(std::string(file_path));

  return LoadPluginInternal(plugin_file_path);
}

bool EFSystemImpl::LoadPlugins() {
  base::ScopedAllowBlockingForTesting allow_io;
  DCHECK(base::DirectoryExists(plugin_path_));

  base::FileEnumerator e(plugin_path_, false, base::FileEnumerator::FILES,
                         FILE_PATH_LITERAL("*.dll"));
  for (base::FilePath plugin_name = e.Next(); !plugin_name.empty();
       plugin_name = e.Next()) {
    LoadPluginInternal(plugin_name);
  }
  return true;
}

bool EFSystemImpl::LoadPluginInternal(const base::FilePath& file_path) {
  base::ScopedAllowBlockingForTesting allow_io;
  if (!base::PathExists(file_path)) {
    return false;
  }

  base::NativeLibraryLoadError e;
  std::unique_ptr<void, LibraryDeleter> plugin(
      base::LoadNativeLibrary(file_path, &e));
  if (!plugin.get()) {
    return false;
  }

  auto* fnCanUnloadPlugin = reinterpret_cast<decltype(&::CanUnloadPlugin)>(
      base::GetFunctionPointerFromNativeLibrary(
          plugin.get(), PLUGIN_EXPORT_SYMBOL(CanUnloadPlugin)));
  auto* fnQueryInterface = reinterpret_cast<decltype(&::QueryInterface)>(
      base::GetFunctionPointerFromNativeLibrary(
          plugin.get(), PLUGIN_EXPORT_SYMBOL(QueryInterface)));

  if (fnCanUnloadPlugin == nullptr || fnQueryInterface == nullptr) {
    return false;
  }

  {
    base::AutoLock l(plugin_lock_);
    if (plugins_.find(file_path) == plugins_.end()) {
      auto p = std::make_unique<Plugin>();
      p->file_path = file_path;
      p->fnCanUnloadPlugin = fnCanUnloadPlugin;
      p->fnQueryInterface = fnQueryInterface;
      p->plugin = plugin.release();
      plugins_.insert(std::make_pair(file_path, std::move(p)));
    }
  }
  return false;
}

}  // namespace ef
