/**
 * @file      : ef_command_line_impl.cc
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-15 10:20:41
 * @brief     : 
 */
#include "easy_framework/src/ef_command_line_impl.h"

#include "base/files/file_path.h"
#include "base/strings/utf_string_conversions.h"

namespace ef {

EFCommandLineImpl::EFCommandLineImpl(base::CommandLine* command_line)
    : command_line_(command_line == nullptr
                        ? base::CommandLine(base::CommandLine::NO_PROGRAM)
                        : *command_line) {}

EFCommandLineImpl::~EFCommandLineImpl() = default;

bool EFCommandLineImpl::CopyTo(IEFCommandLine** out_copy) {
  if (out_copy == nullptr) {
    return false;
  }

  EFCommandLineImpl* copy = new EFCommandLineImpl(&command_line_);
  copy->AddRef();
  *out_copy = copy;
  return true;
}

bool EFCommandLineImpl::InitFromArgv(int argc, const char* const* argv) {
  base::CommandLine::StringVector args;
  for (int i = 0; i < argc; i++) {
    args.push_back(base::UTF8ToWide(std::string(argv[i])));
  }

  command_line_.InitFromArgv(args);
  return true;
}

bool EFCommandLineImpl::InitFromString(const char* command_line_string,
                                       unsigned int length) {
  if (command_line_string == nullptr ||
      std::char_traits<char>::length(command_line_string) == 0) {
    return false;
  }

  command_line_.ParseFromString(
      base::UTF8ToWide(std::string(command_line_string, length)));
  return true;
}

bool EFCommandLineImpl::GetCommandLineString(
    const char** out_command_line_string,
    unsigned int* out_command_line_string_length) const {
  if (out_command_line_string == nullptr &&
      out_command_line_string_length == nullptr) {
    return false;
  }

  static char command_line_string[4096];
  std::char_traits<char>::assign(command_line_string, 4096, 0);
  std::string cls = base::WideToUTF8(command_line_.GetCommandLineString());
  size_t out_length = std::min(sizeof(command_line_string), cls.length());

  if (out_command_line_string) {
    *out_command_line_string = std::char_traits<char>::copy(
        &command_line_string[0], cls.c_str(), out_length);
  }

  if (out_command_line_string_length) {
    *out_command_line_string_length = cls.length();
  }

  return true;
}

bool EFCommandLineImpl::HasSwitch(const char* switch_key) const {
  if (!switch_key || !std::char_traits<char>::length(switch_key)) {
    return false;
  }

  return command_line_.HasSwitch(switch_key);
}

bool EFCommandLineImpl::GetSwitchValue(const char* switch_key,
                                       const char** out_value,
                                       unsigned int* out_value_length) const {
  static char value[1024];
  std::char_traits<char>::assign(&value[0], sizeof(value), 0);

  if (out_value == nullptr && out_value_length == nullptr) {
    return false;
  }

  if (!HasSwitch(switch_key)) {
    return false;
  }

  std::string v = command_line_.GetSwitchValueASCII(switch_key);
  if (v.empty()) {
    return false;
  }

  size_t out_length = std::min(sizeof(value), v.length());
  if (out_value) {
    *out_value = std::char_traits<char>::copy(&value[0], v.c_str(), out_length);
  }

  if (out_value_length) {
    *out_value_length = out_length;
  }

  return true;
}

bool EFCommandLineImpl::AppendSwitch(const char* switch_key) {
  if (!switch_key || !std::char_traits<char>::length(switch_key)) {
    return false;
  }

  command_line_.AppendSwitch(switch_key);
  return true;
}

bool EFCommandLineImpl::AppendSwitchWithValue(const char* switch_key,
                                              const char* switch_value) {
  if (!switch_key || !std::char_traits<char>::length(switch_key) ||
      !switch_value || !std::char_traits<char>::length(switch_value)) {
    return false;
  }

  command_line_.AppendSwitchASCII(switch_key, switch_value);
  return true;
}

bool EFCommandLineImpl::SetProgram(const char* program) {
  if (!program || !std::char_traits<char>::length(program)) {
    return false;
  }

  command_line_.SetProgram(
      base::FilePath::FromUTF8Unsafe(std::string(program)));
  return true;
}

}  // namespace ef
