//
// Created by phirasit on 12/31/19.
//
#include "single_run.hpp"
#include "logger.hpp"

#include <linux/seccomp.h>
#include <sys/prctl.h>
#include <errno.h>

#include <iostream>
#include <fstream>

std::ostream& operator << (std::ostream& out, GRADE_STATUS status) {
  switch (status) {
    case GRADE_STATUS_OK: return out << "OK";
    case GRADE_STATUS_FAILED: return out << "FAILED";
    case GRADE_STATUS_SKIP: return out << "SKIP";
    case GRADE_STATUS_TIMEOUT: return out << "TIMEOUT";
    case GRADE_STATUS_MEMORY_ERROR: return out << "MEM-ERROR";
    case GRADE_STATUS_RUNTIME_ERROR: return out << "RUN-ERROR";
  }
  return out << "INVALID";
}

GRADE_STATUS run_script(const RunConfig& config, int test_id) {
  const Logger logger("run-script");
  logger("finish running ", test_id);
  
  // TODO finish this
  /*
  // set mode to seccomp
  if (prctl(PR_SET_SECCOMP, SECCOMP_MODE_STRICT) == -1) {
    std::cerr << "failed to set seccomp" << std::endl;
    std::cerr << "errno code: " << errno << std::endl;
    return GRADE_STATUS_FAILED;
  }
  */

  // TODO limit processing time

  // TODO limit amount of process memory

  // execute the command
  // execvp(argv[1], argv + 1);
  
  return GRADE_STATUS_OK;
}
