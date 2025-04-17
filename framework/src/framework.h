/**
 * @file      : framework.h
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2025-04-15 20:06:10
 * @brief     : 
 */
#ifndef __FRAMEWORK_H__
#define __FRAMEWORK_H__

#include "build/build_config.h"

#include "libef/wrapper/ref_impl.h"

#include "base/at_exit.h"

#if BUILDFLAG(IS_WIN)
#include "base/win/windows_types.h"
using Instance = HINSTANCE;
#elif BUILDFLAG(IS_LINUX)
//TODO add linux header ?
using Instance = void*;
#endif

#include <atomic>

namespace libef {

class FrameworkImpl : public BaseRefImpl<IFramework> {
 public:
  FrameworkImpl();
  ~FrameworkImpl() override;

 public: /* override BaseInterface */
  bool QueryInterface(const char* interface_name, void** interface) override;
  unsigned int GetVersion() const override;

 public:
  bool Initialize(void* instance) override;
  void UnInitialize() override;
  bool CreateMessageLoop(bool nestable,
                         MessageLoopType type,
                         IMessageLoop** loop) override;
  bool CreateThread(MessageLoopType type,
                    const char* name,
                    IThread** thread) override;

 private:
  std::atomic_bool initialized_{false};
#if BUILDFLAG(IS_WIN)
  Instance instance_{nullptr};
#elif BUILDFLAG(IS_LINUX)

#endif
  std::unique_ptr<base::AtExitManager> at_exit_manager_;
};

}  // namespace libef

#endif /* __FRAMEWORK_H__ */