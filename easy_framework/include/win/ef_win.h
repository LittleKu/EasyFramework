#ifndef EASY_FRAMEWORK_INCLUDE_WIN_H_
#define EASY_FRAMEWORK_INCLUDE_WIN_H_

#include <Windows.h>

struct IBaseInterface;

typedef HRESULT(__stdcall* FN_QUERY_INTERFACE)(const char*, IBaseInterface**);

typedef HMODULE PlatformLibrary;

typedef DWORD ThreadId;

STDAPI QueryInterface(const char*, IBaseInterface**);

#endif  // EASY_FRAMEWORK_INCLUDE_WIN_H_
