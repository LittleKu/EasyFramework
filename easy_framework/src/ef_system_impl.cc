/**
 * @file      : ef_system_impl.cc
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-07 14:28:36
 * @brief     :
 */
#include "easy_framework/src/ef_system_impl.h"
#include "easy_framework/src/ef_thread_pool_task_runner_impl.h"

#include "base/task/thread_pool.h"
#include "base/task/thread_pool/initialization_util.h"
#include "base/task/thread_pool/thread_pool_instance.h"

namespace ef {

namespace {

constexpr size_t kThreadPoolDefaultMin = 16;
constexpr size_t kThreadPoolMax = 32;
constexpr double kThreadPoolCoresMultiplier = 0.6;
constexpr size_t kThreadPoolOffset = 0;

void StartThreadPool() {
  base::ThreadPoolInstance::InitParams thread_pool_init_params = {
      base::RecommendedMaxNumberOfThreadsInThreadGroup(
          kThreadPoolDefaultMin, kThreadPoolMax, kThreadPoolCoresMultiplier,
          kThreadPoolOffset)};

  thread_pool_init_params.common_thread_pool_environment = base::
      ThreadPoolInstance::InitParams::CommonThreadPoolEnvironment::COM_MTA;

  base::ThreadPoolInstance::Get()->Start(thread_pool_init_params);
}

}  // namespace

INTERFACE_UNIQUE_IMPL(IEFSystem, EFSystemImpl)

EFSystemImpl::EFSystemImpl() = default;

EFSystemImpl::~EFSystemImpl() = default;

bool EFSystemImpl::QueryInterface(const char* interface_unique,
                                  IBaseInterface** out_interface) const {
  return false;
}

bool EFSystemImpl::ConnectInterface(IBaseInterface* host) {
  return false;
}

bool EFSystemImpl::Initialize(void* instance) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  instance_ = reinterpret_cast<HINSTANCE>(instance);

  base::ThreadPoolInstance::Create(INTERFACE_UNIQUE(IEFSystem));
  StartThreadPool();

  return true;
}

bool EFSystemImpl::Uninitialize() {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  base::ThreadPoolInstance::Get()->Shutdown();
  return true;
}

bool EFSystemImpl::CreateThreadPoolTaskRunner(ITaskRunner** out_task_runner) {
  if (out_task_runner == nullptr) {
    return false;
  }

  if (base::ThreadPoolInstance::Get()) {
    auto* task_runner = new ThreadPoolTaskRunnerImpl(
        base::ThreadPool::CreateSingleThreadTaskRunner(
            {base::MayBlock(), base::TaskPriority::USER_VISIBLE}));
    task_runner->AddRef();
    *out_task_runner = task_runner;
    return true;
  }
  return false;
}

}  // namespace ef
