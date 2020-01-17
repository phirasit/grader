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

class StringFormatter {
private:
    std::string format;
public:
    std::string& get() { return this->format; }
    
    [[nodiscard]] std::string get_string(const std::string& val) const {
      static const std::string placeholder = "{}";
      std::string result = this->format;
      size_t pos;
      while ((pos = result.find(placeholder)) != std::string::npos) {
        result.replace(pos, placeholder.length(), val);
      }
      return result;
    }
};

#endif //GRADER_UTILITY_HPP
