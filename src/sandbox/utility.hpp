//
// Created by phirasit on 1/5/20.
//

#ifndef GRADER_UTILITY_HPP
#define GRADER_UTILITY_HPP

#include "yaml-cpp/yaml.h"
#include <functional>

template<typename T1, typename T2>
using TransformFn = std::function<T2(T1)>;

template<typename T>
static const TransformFn<T, T> Identity = [] (const T& t) -> T { return t; };

template<typename T1, typename T2 = T1>
static void update_if_exists(T1& value, const YAML::Node& data, TransformFn<T2, T1> apply = Identity<T1>) {
  if (!data) return;
  value = apply(data.as<T2>());
}

#endif //GRADER_UTILITY_HPP
