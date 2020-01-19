//
// Created by phirasit on 1/5/20.
//
#include "run.hpp"
#include "run_list.hpp"

#include "../utility.hpp"
#include "logger.hpp"

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

std::ostream& operator << (std::ostream& out, GRADE_STATUS status) {
  switch (status) {
    case GRADE_STATUS_OK: return out << "OK";
    case GRADE_STATUS_FAILED: return out << "FAILED";
    case GRADE_STATUS_SKIP: return out << "SKIP";
    case GRADE_STATUS_TIMEOUT: return out << "TIMEOUT";
    case GRADE_STATUS_MEMORY_ERROR: return out << "MEM-ERROR";
    case GRADE_STATUS_RUNTIME_ERROR: return out << "RUN-ERROR";
    case GRADE_STATUS_SYSTEM_ERROR: return out << "SYS-ERROR";
  }
  return out << "INVALID";
}

void Constraint::update(const YAML::Node& config) {
  if (!config) return;
  update_if_exists<int, int>(this->time_ms, config["time"], [] (int time_ms) -> int {
      return time_ms * 1000;
  });
  update_if_exists(this->time_ms, config["time-ms"]);
  update_if_exists(this->memory_mb, config["memory"]);
}

void RunConfig::update(const YAML::Node& config) {
  update_if_exists(this->script, config["script"]);
  this->constraint.update(config);
}

RunConfig* RunConfig::get_config(const std::string &type) {
  return RunConfigList::get_run_config(type);
}

GRADE_STATUS RunConfig::interpret_wstatus(int wstatus) {
  if (WIFSIGNALED(wstatus)) {
    switch (WTERMSIG(wstatus)) {
      case SIGSEGV: return GRADE_STATUS_MEMORY_ERROR;
      case SIGXCPU: return GRADE_STATUS_TIMEOUT;
    }
    return GRADE_STATUS_RUNTIME_ERROR;
  }
  // if exit normally
  return GRADE_STATUS_OK;
}

int RunConfig::execute_script() const {
  static const Logger logger ("execute-script");
  logger("running: ", this->script);
  
  // enter sandbox mode
  if (this->enter_sandbox()) {
    return 1;
  }
  
  // convert this->script from std::string to char*[]
  std::stringstream stream (this->script);
  if (stream.eof()) {
    return 1;
  }
  
  std::string path, arg;
  std::vector<std::string> args_string;
  stream >> path;
  while (stream >> arg) {
    args_string.push_back(arg);
  }
  std::vector<const char*> args (args_string.size());
  std::transform(args_string.begin(), args_string.end(), args.begin(), [] (const std::string& s) {
    return s.c_str();
  });
  
  return execvp(path.c_str(), (char**) args.data());
}

