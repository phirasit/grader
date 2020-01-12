//
// Created by phirasit on 1/5/20.
//
#include "run.hpp"

#include "utility.hpp"
#include "logger.hpp"

void Constraint::update(const YAML::Node& config) {
  if (!config) return;
  update_if_exists<int, int>(this->time_ms, config["time"], [] (int time_ms) -> int {
      return time_ms * 1000;
  });
  update_if_exists(this->time_ms, config["time-ms"]);
  update_if_exists(this->memory_mb, config["memory"]);
}

void RunConfig::update(const YAML::Node& config) {
  this->constraint.update(config);
}
