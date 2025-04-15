/**
 * @file      : exports.h
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2025-04-15 16:55:55
 * @brief     :
 */
#ifndef __EXPORTS_H__
#define __EXPORTS_H__

#include "build/build_config.h"

#if BUILDFLAG(IS_WIN)

#if defined(FRAMEWORK_IMPLEMENTATION)
#define FRAMEWORK_EXPORT __declspec(dllexport)
#else
#define FRAMEWORK_EXPORT __declspec(dllimport)
#endif

#elif BUILDFLAG(IS_LINUX)

#if defined(FRAMEWORK_IMPLEMENTATION)
#define FRAMEWORK_EXPORT __attribute__((visibility("default")))
#else
#define FRAMEWORK_EXPORT
#endif

#endif

#endif /* __EXPORTS_H__ */
