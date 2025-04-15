/**
 * @file      : libef.h
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2025-04-15 17:26:42
 * @brief     :
 */
#ifndef __LIBEF_H__
#define __LIBEF_H__

#if defined(_WIN32)

#if defined(FRAMEWORK_IMPLEMENTATION)
#define FRAMEWORK_EXPORT __declspec(dllexport)
#else
#define FRAMEWORK_EXPORT __declspec(dllimport)
#endif

#elif defined(__linux__)

#if defined(FRAMEWORK_IMPLEMENTATION)
#define FRAMEWORK_EXPORT __attribute__((visibility("default")))
#else
#define FRAMEWORK_EXPORT
#endif

#else
#define FRAMEWORK_EXPORT
#endif

#if defined(__cplusplus)
extern "C" {
#endif

FRAMEWORK_EXPORT bool QueryInterface(/*[in]*/ const char* interface_name,
                                     /*[out]*/ void** interface);

#if defined(__cplusplus)
}
#endif

#endif /* __LIBEF_H__ */
