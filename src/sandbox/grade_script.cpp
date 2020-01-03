//
// Created by phirasit on 12/31/19.
//
#include "grade_script.hpp"

#include <linux/seccomp.h>
#include <sys/prctl.h>
#include <errno.h>

#include <iostream>
#include <fstream>

int grade_script(const YAML::Node& config) {
  if (!config.IsMap()) {
    return 1;
  }
  
  // change output stream
  /*
  if (!config["output-file"]) {
    std::ofstream output_buf (config["output-file"].as<std::string>());
    std::cout.rdbuf(output_buf.rdbuf());
  }
  */
  
  // set mode to seccomp
  if (prctl(PR_SET_SECCOMP, SECCOMP_MODE_STRICT) == -1) {
    std::cerr << "failed to set seccomp" << std::endl;
    std::cerr << "errno code: " << errno << std::endl;
    return 1;
  }

  // TODO limit processing time

  // TODO limit amount of process memory

  // execute the command
  // execvp(argv[1], argv + 1);
  
  return 0;
}
