/**
 * @file      : libef.cc
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2025-04-15 16:52:29
 * @brief     : 
 */
#include "build/build_config.h"
#include "libef/include/libef.h"
#include "libef/framework/src/framework.h"

#if BUILDFLAG(IS_WIN)
#include "base/win/windows_full.h"
#elif BUILDFLAG(IS_LINUX)
#endif

#include <mutex>

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

static libef::FrameworkImpl* framework = nullptr;
FRAMEWORK_EXPORT bool QueryInterface(/*[in]*/ const char* interface_name,
                                     /*[out]*/ void** interface) {
  static std::once_flag flag;
  std::call_once(flag, []() { framework = new libef::FrameworkImpl(); });
  return framework->QueryInterface(interface_name, interface);
}
