//
// Created by phirasit on 1/5/20.
//

#include "group.hpp"
#include "utility.hpp"
#include "logger.hpp"

#include <stdexcept>
#include <utility>

static void add_item(std::vector<int>& tests, const std::string& id) {
  static const std::string SEPARATOR = "-";
  const size_t pos = id.find(SEPARATOR);
  if (pos != std::string::npos) {
    int s = std::stoi(id.substr(0, pos));
    int t = std::stoi(id.substr(pos + 1, std::string::npos));
    while (s <= t) {
      tests.push_back(s++);
    }
  } else {
    tests.push_back(std::stoi(id));
  }
}

static void add_item(std::vector<std::string>& tests, const std::string& id) {
  tests.push_back(id);
}

template<typename T>
static const std::vector<T> convert_item_list(const std::string& test_id_string) {
  static const Logger logger("convert-test-id");
  static const std::string DELIMITER = ",";
  std::vector<T> tests;
  const auto add_item_inline = [&tests] (const std::string& id) -> void {
    add_item(tests, id);
  };
  
  try {
    size_t previous = 0, current = test_id_string.find(DELIMITER);
    while (current != std::string::npos) {
      add_item_inline(test_id_string.substr(previous, current - previous));
      previous = current + 1;
      current = test_id_string.find(DELIMITER, previous);
    }
    add_item_inline(test_id_string.substr(previous, current - previous));
  } catch (const std::invalid_argument& exp) {
    logger("cannot convert: ", test_id_string);
    return {};
  }
  return tests;
};

void Group::update(const YAML::Node &config) {
  static const Logger logger("group");
  
  update_if_exists<std::vector<int>, std::string>
    (this->tests, config["tests"], convert_item_list<int>);
  
  update_if_exists<std::vector<std::string>, std::string>
    (this->dependency, config["deps"], convert_item_list<std::string>);
}
