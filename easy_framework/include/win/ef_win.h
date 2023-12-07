#ifndef EASY_FRAMEWORK_INCLUDE_WIN_H_
#define EASY_FRAMEWORK_INCLUDE_WIN_H_

#include <Windows.h>

#include "easy_framework/include/ef_base.h"

typedef HRESULT(__stdcall* FN_QUERY_INTERFACE)(const char*, IBaseInterface**);

#endif  // EASY_FRAMEWORK_INCLUDE_WIN_H_