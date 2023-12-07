#ifndef EASY_FRAMEWORK_COMMON_REF_COUNT_H_
#define EASY_FRAMEWORK_COMMON_REF_COUNT_H_

#include "base/atomic_ref_count.h"
#include "base/check_op.h"

namespace ef {

namespace common {

class RefCount final {
 public:
  RefCount() = default;
  ~RefCount() = default;

  RefCount(const RefCount&) = delete;
  RefCount& operator=(const RefCount&) = delete;

  /**
   * @brief
   *
   * @return int
   */
  int AddRefImpl() const {
    int pre_increment_count = strong_ref_count_.Increment();
    CHECK_GT(pre_increment_count, 0);
    CHECK_NE(pre_increment_count, std::numeric_limits<int>::max());

    return pre_increment_count;
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
   * @return int
   */
  int AddRefWeak() const {
    int pre_increment_count = weak_ref_count_.Increment();

    CHECK_GT(pre_increment_count, 0);
    CHECK_NE(pre_increment_count, std::numeric_limits<int>::max());

    return pre_increment_count;
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