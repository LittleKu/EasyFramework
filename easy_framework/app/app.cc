#include "build/build_config.h"

#if defined(OS_WIN)
#include <Windows.h>

#include "base/command_line.h"
#include "base/files/file_path.h"
#include "base/logging.h"
#include "base/native_library.h"
#include "base/path_service.h"
#include "base/win/windows_types.h"

#include "easy_framework/common/ef_refptr.h"
#include "easy_framework/common/wrapper/ef_base_wrapper.h"
#include "easy_framework/include/ef_base.h"
#include "easy_framework/include/ef_system.h"
#include "easy_framework/include/win/ef_win.h"

#include <functional>

class TaskImpl : public ef::common::wrapper::RefBaseImpl<ITask> {
  std::function<void()> m_func;

 public:
  explicit TaskImpl(const std::function<void()>& func) : m_func(func) {}
  ~TaskImpl() override = default;

  void Run() override { m_func(); }
};

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, wchar_t*, int) {
  base::CommandLine::Init(0, nullptr);

  base::FilePath cur;
  base::PathService::Get(base::DIR_EXE, &cur);
  base::FilePath log_file = cur.Append(L"app.log");

  logging::LoggingSettings settings;
  settings.delete_old = logging::DELETE_OLD_LOG_FILE;
  settings.lock_log = logging::DONT_LOCK_LOG_FILE;
  settings.logging_dest = logging::LOG_TO_ALL;
  settings.log_file_path = log_file.value().c_str();
  logging::InitLogging(settings);
  logging::SetMinLogLevel(logging::LOG_INFO);
  logging::SetLogItems(true, true, true, true);

  {
    ef::common::EFRefPtr<IEFSystem> sys = nullptr;
    QueryInterface(INTERFACE_UNIQUE(IEFSystem),
                   reinterpret_cast<IBaseInterface**>(sys.addressof()));

    if (sys) {
      sys->Initialize(instance);

      ef::common::EFRefPtr<IEFMessageLoop> main_loop = nullptr;
      sys->GetMainMessageLoop(main_loop.addressof());

      ef::common::EFRefPtr<IEFTaskRunner> main_task_runner = nullptr;
      main_loop->CreateTaskRunner(main_task_runner.addressof());

      main_task_runner->PostDelayedTask(
          new TaskImpl([main_loop] { main_loop->Quit(); }), 10000);
      main_loop->Run();

      sys->Uninitialize();
    }
  }

  return 0;
}
#else

#endif  // OS_WIN
