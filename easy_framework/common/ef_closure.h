/**
 * @file      : ef_closure.h
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-20 20:28:50
 * @brief     :
 */
#ifndef EASY_FRAMEWORK_COMMON_CLOSURE_H_
#define EASY_FRAMEWORK_COMMON_CLOSURE_H_

#include "base/atomic_ref_count.h"
#include "base/functional/callback.h"

#include "easy_framework/include/ef_task_runner.h"

namespace ef {

namespace common {

class EFTaskOnceClosure final : public ITask {
 public:
  explicit EFTaskOnceClosure(base::OnceClosure closure)
      : closure_(std::move(closure)) {}

 public:  // override IRefBase methods
  void AddRef() const override { ref_count_.Increment(); }
  bool Release() const override {
    if (!ref_count_.Decrement()) {
      delete this;
      return true;
    }
    return false;
  }
  bool GetWeakRef(IWeakRef** out_weak_ref) const override { return false; }

 public:  // override ITask method
  void Run() override {
    if (!closure_.is_null()) {
      std::move(closure_).Run();
    }
  }

 private:
  base::OnceClosure closure_;
  mutable base::AtomicRefCount ref_count_{0};
};

}  // namespace common
}  // namespace ef
#endif  // !EASY_FRAMEWORK_COMMON_CLOSURE_H_
