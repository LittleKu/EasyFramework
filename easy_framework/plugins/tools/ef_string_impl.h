/**
 * @file      : string_impl.h
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-26 14:17:34
 * @brief     :
 */
#ifndef EASY_FRAMEWORK_PLUGIN_TOOLS_STRING_IMPL_H_
#define EASY_FRAMEWORK_PLUGIN_TOOLS_STRING_IMPL_H_

#include "easy_framework/plugins/include/ef_string.h"

#include "easy_framework/common/ef_refptr.h"
#include "easy_framework/common/wrapper/ef_base_interface_wrapper.h"

#include "base/strings/string_piece.h"

namespace ef {

namespace plugin {

namespace tools {

class EFStringUtf8 : public ef::common::wrapper::RefBaseImpl<IEFStringUtf8> {

  explicit EFStringUtf8(const base::StringPiece& str);
 public:
  static ef::common::EFRefPtr<IEFStringUtf8> FromString(
      const base::StringPiece& str);
  static ef::common::EFRefPtr<IEFStringUtf8> FromWString(
      const base::WStringPiece& str);
  static ef::common::EFRefPtr<IEFStringUtf8> FromString16(
      const base::StringPiece16& str);
  ~EFStringUtf8() override;

  const utf8* String() const override;
  size_t Size() const override;
  void Reset(const utf8* s, size_t length) override;
  void Append(const utf8* s, size_t length) override;
  bool ToWide(IEFStringWide** out_wide) override;

 private:
  std::string string_;
};

class EFStringWide : public ef::common::wrapper::RefBaseImpl<IEFStringWide> {
  explicit EFStringWide(const base::WStringPiece& str);

 public:
  static ef::common::EFRefPtr<IEFStringWide> FromString(
      const base::StringPiece& str);
  static ef::common::EFRefPtr<IEFStringWide> FromWString(
      const base::WStringPiece& str);
  static ef::common::EFRefPtr<IEFStringWide> FromString16(
      const base::StringPiece16& str);
  ~EFStringWide() override;

  const wchar_t* String() const override;
  size_t Size() const override;
  virtual void Reset(const wchar_t* s, size_t length) override;
  void Append(const wchar_t* s, size_t length) override;
  bool ToUtf8(IEFStringUtf8** out_utf8) override;

 private:
  std::wstring string_;
};

class EFStringFactoryImpl
    : public ef::common::wrapper::BaseInterfaceImpl<IEFStringFactory> {
 public:
  EFStringFactoryImpl();
  ~EFStringFactoryImpl() override;

  DECLARE_INTERFACE_UNIQUE(IEFStringFactory);

 public:  // override IEFStringUtil methods
  bool AllocateUtf8(const utf8* str,
                    size_t length,
                    IEFStringUtf8** out_string) override;

  bool AllocateWide(const wchar_t* str,
                    size_t length,
                    IEFStringWide** out_string) override;
};

}  // namespace tools

}  // namespace plugin

}  // namespace ef
#endif  // !EASY_FRAMEWORK_PLUGIN_TOOLS_STRING_IMPL_H_