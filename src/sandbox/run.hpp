//
// Created by phirasit on 1/5/20.
//

#ifndef GRADER_SANBOX_RUN_HPP
#define GRADER_SANBOX_RUN_HPP

#include "script.hpp"

#include "yaml-cpp/yaml.h"

class Constraint {
private:
    int time_ms; // millisecond
    int memory_mb; // megabyte

public:
    Constraint() = default;
    ~Constraint() = default;
    
    int get_time_ms() { return this->time_ms; }
    int get_memory_mb() { return this->memory_mb; }
    
    void update(const YAML::Node& config);
};

class RunConfig {
private:
    Script script;
    Constraint constraint;

public:
    explicit RunConfig() : script(), constraint() {}
    explicit RunConfig(const RunConfig& config)
      : script(config.script), constraint(config.constraint) {}
    explicit RunConfig(const YAML::Node& yaml) : RunConfig() { update(yaml); }
    ~RunConfig() = default;
    
    void update(const YAML::Node& config);
};


#endif //GRADER_RUN_HPP
