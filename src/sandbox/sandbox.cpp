/*
 * Run a command inside a seccomp process
 */
#include "yaml-cpp/yaml.h"
#include "submission_config.hpp"
#include "logger.hpp"

#include <fstream>
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
  
  if (config.get_output().has_value()) {
    // output to file
    std::ofstream output_file (config.get_output().value());
    output_file << config.grade();
    output_file.close();
  } else {
    // output to std::cout
    std::cout << config.grade();
  }
  
  return 0;
}
