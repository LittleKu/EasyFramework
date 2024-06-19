/**
 * @file      : ef_md5_impl.h
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-26 11:07:35
 * @brief     :
 */
#ifndef EASY_FRAMEWORK_PLUGIN_TOOLS_MD5_IMPL_H_
#define EASY_FRAMEWORK_PLUGIN_TOOLS_MD5_IMPL_H_

#include "easy_framework/common/wrapper/ef_base_interface_wrapper.h"
#include "easy_framework/plugins/include/ef_md5.h"

#include "base/hash/md5.h"

#include <atomic>

namespace ef {

namespace plugin {

namespace tools {

class EFMessageDigestImpl
    : public ef::common::wrapper::BaseInterfaceImpl<IEFMessageDigest> {
 public:
  EFMessageDigestImpl();
  ~EFMessageDigestImpl() override;

  DECLARE_INTERFACE_UNIQUE(EFMessageDigestImpl);

  unsigned int Version() const final;

 public:  // override IEFMessageDigest methods
  bool Initialize() override;
  bool Update(const void* data, size_t length) override;
  bool Final(IEFStringUtf8** out_digest_string) override;

 private:
  std::atomic_bool initialized_{false};
  base::MD5Context ctx_;
};

}  // namespace tools

}  // namespace plugin

}  // namespace ef

#endif  // !EASY_FRAMEWORK_PLUGIN_TOOLS_MD5_IMPL_H_
