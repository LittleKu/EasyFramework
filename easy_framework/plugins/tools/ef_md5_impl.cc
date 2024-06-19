/**
 * @file      : ef_md5_impl.cc
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-26 11:07:41
 * @brief     : 
 */
#include "easy_framework/plugins/tools/ef_md5_impl.h"

#include "easy_framework/plugins/tools/ef_string_impl.h"

#include "base/check_op.h"

namespace ef {

namespace plugin {

namespace tools {

IMPL_INTERFACE_UNIQUE(EFMessageDigestImpl, IEFMessageDigest)

EFMessageDigestImpl::EFMessageDigestImpl() = default;

EFMessageDigestImpl::~EFMessageDigestImpl() = default;

unsigned int EFMessageDigestImpl::Version() const {
  return MD_VERSION;
}

bool EFMessageDigestImpl::Initialize() {
  DCHECK(!initialized_);
  base::MD5Init(&ctx_);

  initialized_ = true;
  return true;
}

bool EFMessageDigestImpl::Update(const void* data, size_t length) {
  if (data == nullptr || !length) {
    return false;
  }

  DCHECK(initialized_);
  base::MD5Update(
      &ctx_, base::StringPiece(reinterpret_cast<const char*>(data), length));
  return true;
}

bool EFMessageDigestImpl::Final(IEFStringUtf8** out_digest_string) {
  if (out_digest_string == nullptr) {
    return false;
  }

  DCHECK(initialized_);
  base::MD5Digest digest;
  base::MD5Final(&digest, &ctx_);

  ef::common::EFRefPtr<IEFStringUtf8> out =
      EFStringUtf8::FromString(base::MD5DigestToBase16(digest));
  *out_digest_string = out.release();
  return true;
}

}  // namespace tools

}  // namespace plugin

}  // namespace ef
