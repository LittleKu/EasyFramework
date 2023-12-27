/**
 * @file      : ef_plugin.h
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-26 08:46:46
 * @brief     :
 */
#ifndef EASY_FRAMEWORK_PLUGIN_H_
#define EASY_FRAMEWORK_PLUGIN_H_

#include "easy_framework/include/ef_base.h"

#define PLUGIN_EXPORT_SYMBOL(x) #x

#if defined(_WIN32)
#define EXPORT_PLUGIN_REFERENCES()                                   \
  STDAPI CanUnloadPlugin() {                                         \
    return ef::common::EFLibraryRef::Ref().IsZero() ? S_OK : E_FAIL; \
  }

#define BEGIN_EXPORT_PLUGIN_INTERFACE()                                 \
  STDAPI QueryInterface(const char* unique, IBaseInterface** out_ptr) { \
    if (out_ptr == nullptr || !unique || !strlen(unique)) {             \
      return E_INVALIDARG;                                              \
    }

#define EXPORT_INTERFACE(Interface, Implement)              \
  do {                                                      \
    if (strcmp(INTERFACE_UNIQUE(Interface), unique) == 0) { \
      Implement* p = new Implement();                       \
      p->AddRef();                                          \
      *out_ptr = p;                                         \
      return S_OK;                                          \
    }                                                       \
  } while (false);

#define END_EXPORT_PLUGIN_INTERFACE() \
  return E_NOINTERFACE;               \
  }

#else
// TODO support for Mac OS X
#define EXPORT_PLUGIN_REFERENCES()
#define BEGIN_EXPORT_PLUGIN_INTERFACE()
#define EXPORT_INTERFACE(Interface, Implement)
#endif

#endif  // !EASY_FRAMEWORK_PLUGIN_H_