/**
 * @file      : libef.cc
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2025-04-15 16:52:29
 * @brief     : 
 */
#include "base/functional/bind.h"
#include "build/build_config.h"
#include "libef/include/libef.h"
#include "libef/framework/src/framework.h"

#if BUILDFLAG(IS_WIN)
#include "base/win/windows_full.h"
#elif BUILDFLAG(IS_LINUX)
#endif

#include <mutex>
#include <utility>

#if BUILDFLAG(IS_WIN)
BOOL APIENTRY DllMain(HMODULE module,
                      DWORD reason,
                      LPVOID reserved) {
  if (reason== DLL_PROCESS_ATTACH) {
    DisableThreadLibraryCalls(module);
  }
  return TRUE;
}
#elif BUILDFLAG(IS_LINUX)

#endif

