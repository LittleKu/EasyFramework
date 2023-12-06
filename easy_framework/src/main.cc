#include "build/build_config.h"

#if defined(OS_WIN)

#include <Windows.h>

BOOL WINAPI DllMain(PVOID h, DWORD reason, PVOID reserved) {
  return TRUE;
}

#else

#endif // OS_WIN
