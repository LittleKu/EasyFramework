#ifndef EASY_FRAMEWORK_INCLUDE_WIN_H_
#define EASY_FRAMEWORK_INCLUDE_WIN_H_

#include <Windows.h>

struct IBaseInterface;

typedef HRESULT(__stdcall* FN_QUERY_INTERFACE)(const char*, IBaseInterface**);
typedef HRESULT(__stdcall* FN_CAN_UNLOAD_PLUGIN)();

typedef HMODULE PlatformLibrary;

typedef DWORD ThreadId;

STDAPI QueryInterface(const char*, IBaseInterface**);
STDAPI CanUnloadPlugin();

#endif  // EASY_FRAMEWORK_INCLUDE_WIN_H_
