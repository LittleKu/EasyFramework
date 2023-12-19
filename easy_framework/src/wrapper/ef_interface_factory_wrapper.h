/**
 * @file      : ef_interface_factory_wrapper.h
 * @author    : LittleKu<kklvzl@gmail.com>
 * @date      : 2023-12-18 09:32:10
 * @brief     :
 */
#ifndef EASY_FRAMEWORK_INTERFACE_FACTORY_WRAPPER_H_
#define EASY_FRAMEWORK_INTERFACE_FACTORY_WRAPPER_H_

#include "easy_framework/common/wrapper/ef_base_wrapper.h"
#include "easy_framework/include/ef_interface_factory.h"


#include <type_traits>

namespace ef {
namespace wrapper {

template <typename Interface,
          typename = std::enable_if<
              std::is_convertible<Interface*, IBaseInterface*>::value>::type>
class EFInterfaceFactoryT final
    : public ef::common::wrapper::RefBaseImpl<IEFInterfaceFactory> {
 public:
  EFInterfaceFactoryT() = default;
  ~EFInterfaceFactoryT() override = default;

 public:  // override IEFInterfaceFactory methods
  bool CreateObject(IBaseInterface** out_object) final {
    if (out_object == nullptr) {
      return false;
    }

    Interface* obj = new Interface();
    obj->AddRef();

    *out_object = obj;

    return true;
  }

  bool Clone(IEFInterfaceFactory** copy_factory) final {
    if (copy_factory == nullptr) {
      return false;
    }

    auto* copy = new EFInterfaceFactoryT<Interface>();
    copy->AddRef();
    *copy_factory = copy;
    return true;
  }
};

}  // namespace wrapper
}  // namespace ef

#endif  // !EASY_FRAMEWORK_INTERFACE_FACTORY_WRAPPER_H_
