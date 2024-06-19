/**
 * @file      : string_impl.cc
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-26 14:17:39
 * @brief     :
 */
#include "easy_framework/plugins/tools/ef_string_impl.h"

#include "base/strings/utf_string_conversions.h"

namespace ef {

namespace plugin {

namespace tools {

/************* IEFStringUtf8 *************/

// static
ef::common::EFRefPtr<IEFStringUtf8> EFStringUtf8::FromString(
    const base::StringPiece& str) {
  EFStringUtf8* s = new EFStringUtf8(str);
  return ef::common::EFRefPtr<IEFStringUtf8>(s);
}

// static
ef::common::EFRefPtr<IEFStringUtf8> EFStringUtf8::FromWString(
    const base::WStringPiece& str) {
  EFStringUtf8* s = new EFStringUtf8(base::WideToUTF8(str));
  return ef::common::EFRefPtr<IEFStringUtf8>(s);
}

// static
ef::common::EFRefPtr<IEFStringUtf8> EFStringUtf8::FromString16(
    const base::StringPiece16& str) {
  EFStringUtf8* s = new EFStringUtf8(base::UTF16ToUTF8(str));
  return ef::common::EFRefPtr<IEFStringUtf8>(s);
}

EFStringUtf8::EFStringUtf8(const base::StringPiece& str) : string_(str) {}

EFStringUtf8::~EFStringUtf8() = default;

const utf8* EFStringUtf8::String() const {
  return string_.c_str();
}

size_t EFStringUtf8::Size() const {
  return string_.size();
}

void EFStringUtf8::Reset(const utf8* s, size_t length) {
  if (s && std::char_traits<utf8>::length(s)) {
    size_t size = std::max(std::char_traits<utf8>::length(s), length);
    string_.assign(s, size);
    return;
  }
  string_.resize(length);
}

void EFStringUtf8::Append(const utf8* s, size_t length) {
  if (!s) {
    return;
  }

  string_.append(s, std::min(std::char_traits<utf8>::length(s), length));
}

bool EFStringUtf8::ToWide(IEFStringWide** out_wide) {
  if (out_wide == nullptr) {
    return false;
  }

  *out_wide = EFStringWide::FromString(string_).release();
  return true;
}

/************* IEFStringWide *************/

//static
ef::common::EFRefPtr<IEFStringWide> EFStringWide::FromString(
    const base::StringPiece& str) {
  EFStringWide* s = new EFStringWide(base::UTF8ToWide(str));
  return ef::common::EFRefPtr<IEFStringWide>(s);
}

//static
ef::common::EFRefPtr<IEFStringWide> EFStringWide::FromWString(
    const base::WStringPiece& str) {
  EFStringWide* s = new EFStringWide(str);
  return ef::common::EFRefPtr<IEFStringWide>(s);
}

ef::common::EFRefPtr<IEFStringWide> EFStringWide::FromString16(
    const base::StringPiece16& str) {
  EFStringWide* s = new EFStringWide(base::UTF16ToWide(str));
  return ef::common::EFRefPtr<IEFStringWide>(s);
}

EFStringWide::EFStringWide(const base::WStringPiece& str) : string_(str) {}

EFStringWide::~EFStringWide() = default;

const wchar_t* ef::plugin::tools::EFStringWide::String() const {
  return string_.c_str();
}

size_t EFStringWide::Size() const {
  return string_.size();
}

void EFStringWide::Reset(const wchar_t* s, size_t length) {
  if (s && std::char_traits<wchar_t>::length(s)) {
    size_t size = std::max(std::char_traits<wchar_t>::length(s), length);
    string_.assign(s, size);
    return;
  }
  string_.resize(length);
}

void EFStringWide::Append(const wchar_t* s, size_t length) {
  if (!s) {
    return;
  }

  string_.append(s, std::min(std::char_traits<wchar_t>::length(s), length));
}

bool EFStringWide::ToUtf8(IEFStringUtf8** out_utf8) {
  if (out_utf8 == nullptr) {
    return false;
  }

  *out_utf8 = EFStringUtf8::FromWString(string_).release();
  return true;
}

/************* IEFStringFactory *************/

IMPL_INTERFACE_UNIQUE(EFStringFactoryImpl, IEFStringFactory)

EFStringFactoryImpl::EFStringFactoryImpl() = default;

EFStringFactoryImpl::~EFStringFactoryImpl() = default;

unsigned int EFStringFactoryImpl::Version() const {
  return STRING_FACTORY_VERSION;
}

bool EFStringFactoryImpl::AllocateUtf8(const utf8* str,
                                       size_t length,
                                       IEFStringUtf8** out_string) {
  if (out_string == nullptr) {
    return false;
  }

  if (!str && !length) {
    return false;
  }

  size_t size =
      str ? std::max(std::char_traits<utf8>::length(str), length) : length;

  ef::common::EFRefPtr<IEFStringUtf8> out = EFStringUtf8::FromString(
      str ? std::string(str, size) : std::string(size, 0));
  *out_string = out.release();
  return true;
}

bool EFStringFactoryImpl::AllocateWide(const wchar_t* str,
                                       size_t length,
                                       IEFStringWide** out_string) {
  if (out_string == nullptr) {
    return false;
  }

  if (!str && !length) {
    return false;
  }

  size_t size =
      str ? std::max(std::char_traits<wchar_t>::length(str), length) : length;

  ef::common::EFRefPtr<IEFStringWide> out = EFStringWide::FromWString(
      str ? std::wstring(str, size) : std::wstring(size, 0));
  *out_string = out.release();
  return true;
}

}  // namespace tools

}  // namespace plugin

}  // namespace ef
