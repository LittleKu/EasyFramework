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

#if BUILDFLAG(IS_WIN)
int WINAPI wWinMain(HINSTANCE instance,
                    HINSTANCE prev,
                    wchar_t* cmd,
                    int show) {
  base::CommandLine::Init(0, nullptr);
#elif BUILDFLAG(IS_LINUX)
int main(int argc, char* argv[]) {
  base::CommandLine::Init(argc, argv);
#endif
  base::AtExitManager exit_manager;
  return 0;
}
