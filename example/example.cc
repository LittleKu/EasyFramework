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
#include "base/native_library.h"
#include "base/path_service.h"
#include "base/files/file_path.h"
#include "libef/include/libef.h"

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
      Framework* framework = nullptr;
      query_interface(FRAMEWORK_NAME, reinterpret_cast<void**>(&framework));
      if (framework != nullptr) {
        framework->Initialize(instance);
        // Begin main loop
        framework->UnInitialize();
        framework->Release();
      }
    }
    base::UnloadNativeLibrary(lib);

  }
  return 0;
}
