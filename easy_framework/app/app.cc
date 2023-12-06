#include "build/build_config.h"

#if defined(OS_WIN)
#include <Windows.h>

#include "base/command_line.h"
#include "base/win/windows_types.h"

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, wchar_t*, int) {
  return 0;
}
#else

#endif  // OS_WIN