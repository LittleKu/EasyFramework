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

#elif defined(__linux__)
#if defined(FRAMEWORK_IMPLEMENTATION)
#define FRAMEWORK_EXPORT __attribute__((visibility("default")))
#else
#define FRAMEWORK_EXPORT
#endif
#else
#define FRAMEWORK_EXPORT
#endif
extern "C" FRAMEWORK_EXPORT bool QueryInterface(
    /*[in]*/ const char* interface_name,
    /*[out]*/ void** interface);

class BaseRef {
 public:
  virtual ~BaseRef() {}
  virtual void AddRef() const = 0;
  virtual bool Release() const = 0;
};

class BaseInterface : public BaseRef {
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

class Task : public BaseRef {
 public:
  virtual void Run() = 0;
};

#define TASK_RUNNER_NAME "ITaskRunnerV1"
class TaskRunner : public BaseRef {
 public:
  virtual bool IsInCurrentThread() const = 0;
  virtual void PostTask(Task* task) = 0;
  virtual void PostDelayedTask(Task* task, unsigned long long delay_ms) = 0;
};

#define MESSAGE_LOOP_NAME "IMessageLoopV1"
#define MESSAGE_LOOP_VERSION 1

enum class MessageLoopType {
  kMessageLoopTypeUI = 0,
  kMessageLoopTypeIO,
  kMessageLoopTypeDefault,
};

class MessageLoop : public BaseInterface {
 public:
  virtual int Run() = 0;
  virtual void Quit() = 0;
  virtual MessageLoopType GetType() = 0;
  virtual bool IsNestable() const = 0;
};

#define FRAMEWORK_NAME "IFrameworkV1"
#define FRAMEWORK_VERSION 1
class Framework : public BaseInterface {
 public:
  virtual bool Initialize(void* instance) = 0;
  virtual void UnInitialize() = 0;
  virtual bool CreateMessageLoop(bool nestable,
                                 MessageLoopType type,
                                 MessageLoop** loop) = 0;
};

#endif /* __LIBEF_H__ */
