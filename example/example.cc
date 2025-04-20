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
#include "libef/wrapper/ref_ptr.h"

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
      libef::ref_ptr<IFramework> framework = nullptr;
      if (query_interface(FRAMEWORK_NAME, reinterpret_cast<void**>(framework.addressof()))) {
        framework->Initialize(instance, true);
        // Begin main loop
        {
          /* for thread pool task runner */
          //libef::ref_ptr<ITaskRunner> thread_pool_task_runner = nullptr;
          //framework->CreateThreadPoolTaskRunner(
          //    ThreadPoolTaskRunnerMode::Shared,
          //    thread_pool_task_runner.addressof());
          //thread_pool_task_runner->PostDelayedTask(
          //    new TaskImpl(base::BindOnce([]() {
          //      OutputDebugString(
          //          L",kkkkkkkkkkkkkkxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxk");
          //    })),
          //    1000);
        }

        {
          libef::ref_ptr<IMessageLoop> message_loop = nullptr;
          framework->CreateMessageLoop(
              true, MessageLoopType::kMessageLoopTypeUI, message_loop.addressof());

          libef::ref_ptr<ITaskRunner> task_runner = nullptr;
          message_loop->QueryInterface(TASK_RUNNER_NAME,
                                       reinterpret_cast<void**>(task_runner.addressof()));
          task_runner->PostDelayedTask(
              new TaskImpl(base::BindOnce(
                  [](libef::ref_ptr<IMessageLoop> l) { l->Quit(); },
                  libef::ref_ptr<IMessageLoop>(message_loop))),
              5000ULL);

          message_loop->Run();
        }
        framework->UnInitialize();
      }
    }
    base::UnloadNativeLibrary(lib);
  }
  return 0;
}
