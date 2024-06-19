/**
 * @file      : ef_system_impl.h
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-07 14:28:29
 * @brief     :
 */
#ifndef EASY_FRAMEWORK_SYSTEM_IMPL_H_
#define EASY_FRAMEWORK_SYSTEM_IMPL_H_

#include "base/at_exit.h"
#include "base/files/file_path.h"
#include "base/native_library.h"
#include "base/sequence_checker.h"
#include "base/synchronization/lock.h"
#include "base/task/sequence_manager/sequence_manager_impl.h"

#include "easy_framework/common/wrapper/ef_base_interface_wrapper.h"
#include "easy_framework/include/ef_interface_factory.h"
#include "easy_framework/include/ef_plugin.h"
#include "easy_framework/include/ef_system.h"
#include "easy_framework/src/wrapper/ef_interface_factory_wrapper.h"

#include <memory>
#include <unordered_map>

struct IEFMessageLoop;

namespace ef {

class Plugin;

class EFSystemImpl final
    : public ef::common::wrapper::BaseInterfaceImpl<IEFSystem> {
  using Base = ef::common::wrapper::BaseInterfaceImpl<IEFSystem>;

 public:
  EFSystemImpl();
  ~EFSystemImpl() override;

  DECLARE_INTERFACE_UNIQUE(IEFSystem);

 public:  // override IBaseInterface method
  unsigned int Version() const final;
  bool QueryInterface(const char* interface_unique,
                      IBaseInterface** out_interface) final;
  bool ConnectInterface(IBaseInterface* host) final;

 public:  // override IEFSystem methods
  bool Initialize(void* instance, const char* plugin_path) override;
  bool Uninitialize() override;
  bool CreateThreadPoolTaskRunner(IEFTaskRunner** out_task_runner) override;
  bool GetCurrentCommandLine(IEFCommandLine** out_command_line) override;
  bool RegisterInterfaceFactory(const char* unique,
                                IEFInterfaceFactory* factory) override;
  bool UnRegisterInterfaceFactory(const char* unique) override;
  bool GetMainMessageLoop(IEFMessageLoop** out_message_loop) override;
  bool LoadPluginManual(const char* file_path) override;

 private:
  template <typename Interface, typename Impl>
  bool RegisterInterfaceFactoryInternal(const char* unique) {
    ef::common::EFRefPtr<IEFInterfaceFactory> factory = nullptr;
    if (ef::wrapper::EFInterfaceFactoryT<Impl>().Clone(factory.addressof())) {
      return RegisterInterfaceFactory(unique, factory.get());
    }
    return false;
  }

  bool LoadPlugins();
  bool LoadPluginInternal(const base::FilePath& file_path);

 private:
  HINSTANCE instance_{nullptr};
  SEQUENCE_CHECKER(sequence_checker_);

  std::unique_ptr<base::AtExitManager> at_exit_manager_{nullptr};
  ef::common::EFRefPtr<IEFMessageLoop> main_message_loop_{nullptr};

  base::Lock interface_factory_map_lock_;
  std::unordered_map<std::string, ef::common::EFRefPtr<IEFInterfaceFactory>>
      interface_factory_map_;
  base::FilePath plugin_path_;

  struct Plugin {
    base::FilePath file_path;
    decltype(&::CanUnloadPlugin) fnCanUnloadPlugin = nullptr;
    decltype(&::QueryInterface) fnQueryInterface = nullptr;
    base::NativeLibrary plugin = nullptr;
  };
  base::Lock plugin_lock_;
  std::unordered_map<base::FilePath, std::unique_ptr<Plugin>> plugins_;
};

}  // namespace ef

#endif  // !EASY_FRAMEWORK_SYSTEM_IMPL_H_
