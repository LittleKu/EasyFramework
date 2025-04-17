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
#include <wtypes.h> // for DWORD
#elif defined(__linux__)
#if defined(FRAMEWORK_IMPLEMENTATION)
#define FRAMEWORK_EXPORT __attribute__((visibility("default")))
#else
#define FRAMEWORK_EXPORT
#endif
#include <pthread.h>  //for pid_t
#else
#define FRAMEWORK_EXPORT
#endif
extern "C" FRAMEWORK_EXPORT bool QueryInterface(
    /*[in]*/ const char* interface_name,
    /*[out]*/ void** interface);

class IBaseRef {
 public:
  virtual ~IBaseRef() {}
  virtual void AddRef() const = 0;
  virtual bool Release() const = 0;
};

class IBaseInterface : public IBaseRef {
 public:
  /**
   * \brief 通过接口名称获取指定接口实例对象
   *
   * \param interface_name 接口名称
   * \param interface 接口实例对象
   * \return true 获取成功,false 获取失败
   */
  virtual bool QueryInterface(const char* interface_name, void** interface) = 0;
  /**
   * \brief 获取对应接口版本号.
   *
   * \return 接口版本号
   */
  virtual unsigned int GetVersion() const = 0;
};

class ITask : public IBaseRef {
 public:
  virtual void Run() = 0;
};

#define TASK_RUNNER_NAME "ITaskRunnerV1"
class ITaskRunner : public IBaseRef {
 public:
  virtual bool IsInCurrentThread() const = 0;
  virtual void PostTask(ITask* task) = 0;
  virtual void PostDelayedTask(ITask* task, unsigned long long delay_ms) = 0;
};

#define MESSAGE_LOOP_NAME "IMessageLoopV1"
#define MESSAGE_LOOP_VERSION 1

enum class MessageLoopType {
  kMessageLoopTypeUI = 0,
  kMessageLoopTypeIO,
  kMessageLoopTypeDefault,
};

class IMessageLoop : public IBaseInterface {
 public:
  virtual int Run() = 0;
  virtual void Quit() = 0;
  virtual MessageLoopType GetType() = 0;
  virtual bool IsNestable() const = 0;
};

#if defined(_WIN32)
using ThreadId = DWORD;
#elif defined(__linux__)
using ThreadId = pid_t;
#endif

class IThread : public IBaseRef {
 public:
  virtual void Start() = 0;
  virtual void Stop() = 0;
  virtual bool IsRunning() const = 0;
  virtual const char* GetName() const = 0;
  /**
   * \brief Get thread id.
   * Note: If the thread is not started,
   * it will wait until the thread is run and then return the ID of this thread
   *
   * \return thread id
   */
  virtual ThreadId GetThreadId() const = 0;
  virtual bool GetTaskRunner(ITaskRunner** runner) const = 0;
};

#define FRAMEWORK_NAME "IFrameworkV1"
#define FRAMEWORK_VERSION 1
class IFramework : public IBaseInterface {
 public:
  virtual bool Initialize(void* instance) = 0;
  virtual void UnInitialize() = 0;
  virtual bool CreateMessageLoop(bool nestable,
                                 MessageLoopType type,
                                 IMessageLoop** loop) = 0;
  virtual bool CreateThread(MessageLoopType type,
                            const char* name,
                            IThread** thread) = 0;
};

#endif /* __LIBEF_H__ */
