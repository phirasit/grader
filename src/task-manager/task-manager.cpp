//
// Created by phirasit on 12/24/19.
//
#include "logger.hpp"
#include "task-manager.hpp"
#include "yaml-cpp/yaml.h"

static const Logger logger ("task-manager");

static void help() {
  // TODO
}

static void help_install() {
  // TODO
  // task install task-config-file task-location
}

static int load_install_script(const file::File& task_config, const file::File& task_location) {
  // load config file
  const YAML::Node& config = YAML::LoadFile(task_config);
  if (!config) {
    logger("cannot read file: ", task_config);
    return 1;
  } else {
    logger("reading config file from: ", task_config);
  }
  
  const std::optional<Task>& task_obj = Task::from_yaml(config, task_location);
  
  if (!task_obj.has_value()) {
    logger("task config is invalid");
    return 1;
  }
  
  return install(task_obj.value());
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    logger("Arguments are missing");
    help();
    return 1;
  }
  
  const std::string option (argv[1]);
  
  if (option == "install") {
    if (argc < 4) {
      help_install();
      return 1;
    }
    return load_install_script(file::File(argv[2]), file::File(argv[3]));
  }

  logger("invalid command: ", argv[1]);
  return 1;
}
