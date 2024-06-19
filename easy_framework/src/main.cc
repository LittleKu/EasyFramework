/**
 * @file      : main.cc
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-07 14:28:19
 * @brief     : 
 */
#include "base/check_op.h"
#include "build/build_config.h"

#if defined(OS_WIN)

#include "easy_framework/include/win/ef_win.h"

BOOL WINAPI DllMain(PVOID h, DWORD reason, PVOID reserved) {
  return TRUE;
}
#else
#endif // OS_WIN

#include "easy_framework/src/ef_system_impl.h"

#include <mutex> // call_once
#include <string>

#if defined(OS_WIN)
STDAPI QueryInterface(const char* interface_unique, IBaseInterface** out_ptr) {
  if (!out_ptr || !interface_unique ||
      !std::char_traits<char>::length(interface_unique)) {
    return E_INVALIDARG;
  }

  if (std::char_traits<char>::compare(
          interface_unique, INTERFACE_UNIQUE(IEFSystem),
          std::char_traits<char>::length(interface_unique)) == 0) {
    static ef::EFSystemImpl* system = nullptr;
    static std::once_flag flag;
    std::call_once(
        flag,
        [](ef::EFSystemImpl** pp) {
          DCHECK(pp);
          *pp = new ef::EFSystemImpl();
        },
        &system);
    system->AddRef();
    *out_ptr = system;
    return S_OK;
  }
  return E_NOINTERFACE;
}

#else //TODO support Mac OS

#endif
