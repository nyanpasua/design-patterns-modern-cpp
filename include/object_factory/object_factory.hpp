//
// Created by liyingmin on 2021/9/11.
//

#ifndef DESIGN_PATTERNS_OBJECT_FACTORY_HPP
#define DESIGN_PATTERNS_OBJECT_FACTORY_HPP

#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>

#include "singleton/singleton.hpp"

#if defined(__cplusplus) && __cplusplus >= 201709L
#include <concepts>
#endif

#include <iostream>
namespace okdp {

/*!
@brief Automatically register type into factory and yield them by identifier
"key".

This class template is inherited from okdp::singleton and it can hence be used
as a "singleton class".

@tparam AbstractProduct The abstract base class of the products, all registered
types are its subclass.

@since version 1.0.0
*/
template <typename AbstractProduct>
class object_factory : public singleton<object_factory<AbstractProduct>> {
  using this_type = object_factory<AbstractProduct>;

 public:
  /*!
  @brief register type into map using functor.

  @tparam T the creating type.
  */
  template <typename T>
#if defined(__cplusplus) && __cplusplus >= 201709L
  requires std::derived_from<T, AbstractProduct>
#endif
  struct register_type {
    // registers T into object factory.
    register_type(const std::string &key) {
      this_type::instance().map_.emplace(key, [] { return new T(); });
    }

    // register T with variadic parameters into object factory
    template <typename... Args>
    register_type(const std::string &key, Args... args) {
      //(std::cout << ... << args) << std::endl;
      this_type::instance().map_.emplace(key, [=] { return new T(args...); });
    }
  };

  // removes object
  void unregister(const std::string &key) {
    this_type::instance().map_.erase(key);
  }

  /// return concrete object by invoking new operator
  /// !!!note!!! if use this method user should delete it to avoid memory leaks.
  AbstractProduct *create(const std::string &key) {
    if (this_type::instance().map_.find(key) ==
        this_type::instance().map_.end())
      throw std::invalid_argument(
          "error: unknown object type passed to factory!");
    return this_type::instance().map_[key]();
  }

  /// return concrete object by invoking shared ptr
  std::shared_ptr<AbstractProduct> create_shared(const std::string &key) {
    return std::shared_ptr<AbstractProduct>(create(key));
  }

  /// return concrete object by invoking unique ptr
  std::unique_ptr<AbstractProduct> create_unique(const std::string &key) {
    return std::unique_ptr<AbstractProduct>(create(key));
  }

 private:
  std::map<std::string, std::function<AbstractProduct *()>> map_;
};

// template<typename T> std::map<std::string, std::function<T*()>>
// object_factory<T>::map_;

#define FACTORY_REGISTER(Base, Derived, ...)                               \
  static okdp::object_factory<Base>::register_type<Derived> reg_##Derived( \
      #Derived, ##__VA_ARGS__);

}  // namespace okdp
#endif  // DESIGN_PATTERNS_OBJECT_FACTORY_HPP
