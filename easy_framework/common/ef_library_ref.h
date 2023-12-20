/**
 * @file      : ef_library_ref.h
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-20 09:47:25
 * @brief     :
 */
#ifndef EASY_FRAMEWORK_COMMON_LIBRARY_REF_H_
#define EASY_FRAMEWORK_COMMON_LIBRARY_REF_H_

#include "base/atomic_ref_count.h"

namespace ef {

namespace common {

class EFLibraryRef final {
  mutable base::AtomicRefCount ref_{0};
  EFLibraryRef() = default;

 public:
  ~EFLibraryRef() = default;

  static EFLibraryRef* Get() {
    static EFLibraryRef _;
    return &_;
  }

  static EFLibraryRef& Ref() { return *Get(); }

  EFLibraryRef(const EFLibraryRef&) = delete;
  EFLibraryRef& operator=(const EFLibraryRef&) = delete;

  void AddRef() const { ref_.Increment(); }
  bool Release() const { return ref_.Decrement(); }
  bool IsZero() const { return ref_.IsZero(); }
};

}  // namespace common

}  // namespace ef

#endif  // !EASY_FRAMEWORK_COMMON_LIBRARY_REF_H_
