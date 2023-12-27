/**
 * @file      : ef_tools.cc
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-27 14:20:26
 * @brief     : 
 */
#include "easy_framework/include/ef_plugin.h"
#include "easy_framework/plugins/tools/ef_string_impl.h"
#include "easy_framework/plugins/tools/ef_md5_impl.h"

EXPORT_PLUGIN_REFERENCES()

BEGIN_EXPORT_PLUGIN_INTERFACE()
EXPORT_INTERFACE(IEFStringFactory, ef::plugin::tools::EFStringFactoryImpl)
EXPORT_INTERFACE(IEFMessageDigest, ef::plugin::tools::EFMessageDigestImpl)
END_EXPORT_PLUGIN_INTERFACE()
