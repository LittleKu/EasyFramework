/**
 * @file      : eframework.cc
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2025-04-15 16:52:29
 * @brief     : 
 */
#include "libef/eframework/src/eframework.h"

#include "build/build_config.h"
#include "libef/include/exports.h"


#if BUILDFLAG(IS_WIN)
#include "base/win/windows_full.h"

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
