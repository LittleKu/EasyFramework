/**
 * @file      : example.cc
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2025-04-15 16:15:30
 * @brief     : 
 */
#include "build/build_config.h"

#if BUILDFLAG(IS_WIN)
#include "base/win/windows_full.h"
#elif BUILDFLAG(IS_LINUX)
#else
#error "Unsupported OS"
#endif

#include "base/at_exit.h"
#include "base/command_line.h"
#include "base/functional/callback.h"
#include "base/native_library.h"
#include "base/path_service.h"
#include "base/files/file_path.h"
#include "libef/include/libef.h"
#include "libef/wrapper/ref_impl.h"

class TaskImpl : public libef::BaseRefImpl<ITask> {
 public:
  explicit TaskImpl(base::OnceClosure task) : task_(std::move(task)) {}

  void Run() override {
    if (!task_.is_null()) {
      std::move(task_).Run();
    }
  }

 private:
  base::OnceClosure task_;
};

#if BUILDFLAG(IS_WIN)

#define EXTENSION ".dll"

int WINAPI wWinMain(HINSTANCE instance,
                    HINSTANCE prev,
                    wchar_t* cmd,
                    int show) {
  base::CommandLine::Init(0, nullptr);
#elif BUILDFLAG(IS_LINUX)

#define EXTENSION ".so"

int main(int argc, char* argv[]) {
  base::CommandLine::Init(argc, argv);
#endif
  base::AtExitManager exit_manager;
  base::FilePath lib_path;
  base::PathService::Get(base::DIR_EXE, &lib_path);
  lib_path = lib_path.AppendASCII("libef").AddExtensionASCII(EXTENSION);
  base::NativeLibrary lib = base::LoadNativeLibrary(lib_path, nullptr);
  if (lib != nullptr) {
    auto query_interface = reinterpret_cast<decltype(&QueryInterface)>(
        base::GetFunctionPointerFromNativeLibrary(lib, "QueryInterface"));
    if (query_interface != nullptr) {
      IFramework* interface = nullptr;
      if (query_interface(FRAMEWORK_NAME, &interface)) {
        IFramework* framework = reinterpret_cast<IFramework*>(interface);
        framework->Initialize(instance);
        // Begin main loop

        {
          IMessageLoop* message_loop = nullptr;
          framework->CreateMessageLoop(
              true, MessageLoopType::kMessageLoopTypeUI, &message_loop);

          ITaskRunner* task_runner = nullptr;
          message_loop->QueryInterface(TASK_RUNNER_NAME,
                                       reinterpret_cast<void**>(&task_runner));
          task_runner->PostDelayedTask(
              new TaskImpl(base::BindOnce(
                  [](scoped_refptr<IMessageLoop> l) { l->Quit(); },
                  scoped_refptr<IMessageLoop>(message_loop))),
              5000ULL);

          message_loop->Run();
          task_runner->Release();
          message_loop->Release();
        }
        framework->UnInitialize();
        framework->Release();
      }
    }
    base::UnloadNativeLibrary(lib);

  }
  return 0;
}
