//
// Created by phirasit on 1/1/20.
//

#include "task-manager.hpp"
#include "logger.hpp"

#include "yaml-cpp/yaml.h"

int install(const Task& task) {
  static const Logger logger ("install");
  
  // copy task from task location to task folder
  const file::File& source_dir = task.get_task_location();
  const file::File& task_dir = file::get_path("/tasks/") + file::File(task.get_task_id());
  
  if (file::copy_folder(source_dir, task_dir)) {
    logger("cannot copy task to the destination folder");
    return 1;
  }
  
  // execute install script
  if (task.get_install_script().execute(task_dir)) {
    logger("install script return errors");
    return 1;
  }
  
  return 0;
}
