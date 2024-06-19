/**
 * @file      : ef_command_line_impl.h
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-15 10:20:52
 * @brief     :
 */
#ifndef EASY_FRAMEWORK_COMMAND_LINE_IMPL_H_
#define EASY_FRAMEWORK_COMMAND_LINE_IMPL_H_

#include "easy_framework/common/wrapper/ef_base_interface_wrapper.h"
#include "easy_framework/include/ef_command_line.h"

#include "base/command_line.h"

namespace ef {

class EFCommandLineImpl
    : public ef::common::wrapper::BaseInterfaceImpl<IEFCommandLine> {
 public:
  explicit EFCommandLineImpl(base::CommandLine* command_line = nullptr);
  ~EFCommandLineImpl() override;

  DECLARE_INTERFACE_UNIQUE(IEFCommandLine);

  unsigned int Version() const final;

 public:
  bool CopyTo(IEFCommandLine** out_copy) override;
  bool InitFromArgv(int argc, const char* const* argv) override;
  bool InitFromString(const char* command_line_string,
                      unsigned int length) override;
  bool GetCommandLineString(
      const char** out_command_line_string,
      unsigned int* out_command_line_string_length) const override;
  bool HasSwitch(const char* switch_key) const override;
  bool GetSwitchValue(const char* switch_key,
                      const char** out_value,
                      unsigned int* out_value_length) const override;
  bool AppendSwitch(const char* switch_key) override;
  bool AppendSwitchWithValue(const char* switch_key,
                             const char* switch_value) override;
  bool SetProgram(const char* program) override;

 private:
  base::CommandLine command_line_;
};

}  // namespace ef
#endif  // !EASY_FRAMEWORK_COMMAND_LINE_IMPL_H_
