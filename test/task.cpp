//
// Created by phirasit on 1/1/20.
//
#include "catch.hpp"
#include "yaml-cpp/yaml.h"

#include "task.hpp"
#include "setup.hpp"
#include <stdlib.h>
#include "setup.hpp"

TEST_CASE("parse a task from yaml", "[task]") {
  
  REQUIRE(setup_test_system() == 0);
  
  const YAML::Node& task_yaml = YAML::LoadFile("test/sample-task/config.yaml");
  
  const auto task_or_null = Task::from_yaml(task_yaml, "test/sample-task");
  REQUIRE(task_or_null.has_value());
  
  // check detail
  const Task& task = task_or_null.value();
  REQUIRE(task.get_task_id() == "a+b");
  REQUIRE(task.get_task_version() == "1");
  REQUIRE(task.get_result_path() == "result.out");
}

TEST_CASE("install a task", "[task][end-to-end]") {
  REQUIRE(setup_test_system() == 0);
  REQUIRE(system(nullptr));
  REQUIRE(system("bin/task install test/sample-task/config.yaml test/sample-task") == 0);
}
