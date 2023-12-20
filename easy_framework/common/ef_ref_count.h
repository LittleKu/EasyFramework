/**
 * @file      : ef_ref_count.h
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-07 14:27:11
 * @brief     :
 */
#ifndef EASY_FRAMEWORK_COMMON_REF_COUNT_H_
#define EASY_FRAMEWORK_COMMON_REF_COUNT_H_

#include "base/atomic_ref_count.h"
#include "base/check_op.h"

#include "easy_framework/common/ef_library_ref.h"
namespace ef {

namespace common {

class RefCount final {
 public:
  RefCount() { EFLibraryRef::Ref().AddRef(); }
  ~RefCount() { EFLibraryRef::Ref().Release(); }

  RefCount(const RefCount&) = delete;
  RefCount& operator=(const RefCount&) = delete;

  /**
   * @brief
   *
   */
  void AddRefImpl() const {
    CHECK_NE(strong_ref_count_.Increment(), std::numeric_limits<int>::max());
  }

  /**
   * @brief
   *
   * @return
   */
  bool ReleaseImpl() const {
    if (!strong_ref_count_.Decrement()) {
      return true;
    }
    return false;
  }

  /**
   * @brief
   *
   */
  void AddRefWeak() const {
    CHECK_NE(weak_ref_count_.Increment(), std::numeric_limits<int>::max());
  }

  /**
   * @brief
   *
   */
  bool ReleaseWeak() const {
    if (!weak_ref_count_.Decrement()) {
      delete this;
      return true;
    }
    return false;
  }

  bool HasAtLeastOneRef() const { return !strong_ref_count_.IsZero(); }

 private:
  mutable base::AtomicRefCount strong_ref_count_{0};
  mutable base::AtomicRefCount weak_ref_count_{0};
};

}  // namespace common

}  // namespace ef
#endif  // !EASY_FRAMEWORK_COMMON_REF_COUNT_H_
