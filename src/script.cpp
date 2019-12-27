//
// Created by phirasit on 12/24/19.
//

#include "script.hpp"

static int execute_command(const std::string &path, const std::string& command) {
  // TODO execute script
  // TODO limit script permission
  
  return 0; // no error
}

int Script::execute(const std::string &path) const {
  for (const std::string& command : this->scripts) {
    if (execute_command(path, command)) {
      return 1;
    }
  }
}

std::optional<Script> Script::from_yaml(const YAML::Node& yaml) {
  if (yaml.IsNull()) {
    return Script();
  }
  if (yaml.IsScalar()) {
    return Script({yaml.as<std::string>()});
  }
  if (yaml.IsSequence()) {
    std::vector<std::string> scripts;
    for (auto it = yaml.begin(); it != yaml.end(); ++it) {
      scripts.push_back(it->as<std::string>());
    }
    return Script(std::move(scripts));
  }
}