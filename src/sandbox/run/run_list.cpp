//
// Created by phirasit on 1/16/20.
//

#include "run_list.hpp"

#include <logger.hpp>

// additional headers
#include "equal_string.hpp"

RunConfig* RunConfigList::get_run_config(const std::string &type) {
  if (type == RunConfigCompareStringEqual::get_name()) {
    return new RunConfigCompareStringEqual();
  }
  // add more configs here
  
  
  // default options
  return new RunConfigCompareStringEqual();
}
