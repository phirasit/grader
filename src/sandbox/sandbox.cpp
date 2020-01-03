/*
 * Run a command inside a seccomp process
 */
#include "yaml-cpp/yaml.h"
#include "grade_script.hpp"

#include <iostream>
#include <unistd.h>

int main(int argc, char* argv[]) {
  if (argc <= 2) {
    std::cerr << "usage: ./sandbox config-file.yaml" << std::endl;
    return 1;
  }
  
  const YAML::Node config (argv[1]);

  if (config.IsNull()) {
    std::cerr << "cannot open config file: " << argv[1] << std::endl;
    return 1;
  }
  
  return grade_script(config);
}
