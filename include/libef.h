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
#include <wtypes.h>  // for DWORD
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
    /*[out]*/ void** iface);

class IBaseRef {
 public:
  virtual ~IBaseRef() {}
  virtual void AddRef() const = 0;
  virtual bool Release() const = 0;
};

class IBaseInterface : public IBaseRef {
 public:
  /**
   * @brief Query interface with name
   *
   * @param interface_name 
   * @param interface 
   * @return true on success,false on failure.
   */
  virtual bool QueryInterface(const char* interface_name, void** iface) = 0;
  /**
   * @brief Get interface version
   *
   * @return version
   */
  virtual unsigned int GetVersion() const = 0;
};

class ITask : public IBaseRef {
 public:
 /**
  * @brief Execute task
  * 
  */
  virtual void Run() = 0;
};

enum class ThreadPoolTaskRunnerMode {
  Shared,
  Dedicated,
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
 /**
  * @brief starting a message loop
  * 
  * @return 
  */
  virtual int Run() = 0;

  /**
   * @brief quit message loop
   * 
   */
  virtual void Quit() = 0;
  /**
   * @brief Get message loop type
   * 
   * @return UI, IO or default
   */
  virtual MessageLoopType GetType() = 0;

  /**
   * @brief 
   * 
   * @return true on can post nestable task
   */
  virtual bool IsNestable() const = 0;
};

#if defined(_WIN32)
using ThreadId = DWORD;
#elif defined(__linux__)
using ThreadId = pid_t;
#endif

class IThread : public IBaseRef {
 public:
 /**
  * @brief Start thread
  * 
  */
  virtual void Start() = 0;

  /**
   * @brief Stop thread
   * 
   */
  virtual void Stop() = 0;

  /**
   * @brief 
   * 
   * @return true if thread is running, otherwise false
   */
  virtual bool IsRunning() const = 0;

  /**
   * @brief Get thread name
   * 
   */
  virtual const char* GetName() const = 0;
  /**
   * @brief Get thread id.
   * Note: If the thread is not started,
   * it will wait until the thread is run and then return the ID of this thread
   *
   * @return thread id
   */
  virtual ThreadId GetThreadId() const = 0;

  /**
   * @brief Get current thread's task runner
   * 
   * @param runner 
   * @return true on success, otherwise failure
   */
  virtual bool GetTaskRunner(ITaskRunner** runner) const = 0;
};

#define FRAMEWORK_NAME "IFrameworkV1"
#define FRAMEWORK_VERSION 1
class IFramework : public IBaseInterface {
 public:
  /**
   * @brief
   *
   * @param instance Process instance
   * @param create_thread_pool support thread pool. if true, than call
   *            CreateThreadPoolTaskRunner will be success, otherwise failed.
   * @return true on success, false on failure.
   */
  virtual bool Initialize(void* instance, bool create_thread_pool) = 0;

  /**
   * @brief
   *
   */
  virtual void UnInitialize() = 0;

  /**
   * @brief Create a Message Loop object
   *
   * @param nestable
   * @param type
   * @param loop
   * @return true on success, otherwise failure
   */
  virtual bool CreateMessageLoop(bool nestable,
                                 MessageLoopType type,
                                 IMessageLoop** loop) = 0;

  /**
   * @brief Create a Thread object
   *
   * @param type
   * @param name
   * @param thread
   * @return true on success, false on failure
   */
  virtual bool CreateThread(MessageLoopType type,
                            const char* name,
                            IThread** thread) = 0;

  /**
   * @brief Create a Thread Pool Task Runner object
   *
   * @param mode
   * @param task_runner
   * @return true on success, false on failure
   */
  virtual bool CreateThreadPoolTaskRunner(ThreadPoolTaskRunnerMode mode,
                                          ITaskRunner** task_runner) = 0;
};

#endif /* __LIBEF_H__ */
