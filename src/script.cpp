//
// Created by phirasit on 12/24/19.
//
#include "script.hpp"
#include "logger.hpp"

#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <wait.h>

static int execute_script(const file::File& path, const std::vector<std::string>& scripts) {
  static const Logger logger("script-execution");
  
  // TODO change to chroot
  file::change_folder(path);
  
  for (const std::string& command : scripts) {
    logger("running... ", command);
    if (system(command.c_str())) {
      return 1;
    }
  }
  return 0;
}

int Script::execute(const file::File &path) const {
  if (this->empty()) return 0; // no code to execute
  
  switch (pid_t child_pid = fork()) {
    case -1: return 1;
    case 0: {
      execute_script(path, this->scripts);
      exit(0);
    }
    default: {
      int wstatus;
      waitpid(child_pid, &wstatus, 0);
      return 0;
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
  
  return std::nullopt;
}