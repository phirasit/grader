/*
 * Run a command inside a seccomp process
 */
#include "yaml-cpp/yaml.h"
#include "submission_config.hpp"
#include "logger.hpp"

#include <iostream>
#include <unistd.h>

int main(int argc, char* argv[]) {
  const Logger logger ("sandbox");
  
  if (argc < 2) {
    std::cerr << "usage: ./sandbox config-file.yaml ..." << std::endl;
    return 1;
  }
  
  // create a submission config
  SubmissionConfig config;
  for (int i = 1; i < argc; ++i) {
    const YAML::Node& yaml = YAML::LoadFile(argv[i]);
    if (yaml) {
      config.update(yaml);
    } else {
      logger("[warning] omitting ", argv[i], ": cannot open the file");
    }
  }
  
  config.grade()
    .report(std::cout);
  
  return 0;
}
