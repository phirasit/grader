//
// Created by phirasit on 1/5/20.
//

#ifndef GRADER_SANDBOX_RUN_HPP
#define GRADER_SANDBOX_RUN_HPP

#include "script.hpp"

#include "yaml-cpp/yaml.h"

enum GRADE_STATUS {
    // this list has to be ordered by precedent
    // from least to most
    GRADE_STATUS_SKIP,
    GRADE_STATUS_OK,
    GRADE_STATUS_FAILED,
    GRADE_STATUS_TIMEOUT,
    GRADE_STATUS_MEMORY_ERROR,
    GRADE_STATUS_RUNTIME_ERROR,
    GRADE_STATUS_SYSTEM_ERROR,
};

std::ostream& operator << (std::ostream& out, GRADE_STATUS status);

class Constraint {
private:
    int time_ms; // millisecond
    int memory_mb; // megabyte

public:
    Constraint() = default;
    ~Constraint() = default;
    
    [[nodiscard]] int get_time_ms() const { return this->time_ms; }
    [[nodiscard]] int get_memory_mb() const { return this->memory_mb; }
    
    void update(const YAML::Node& config);
};

class RunConfig {
private:
    std::string script;
    Constraint constraint;
    
protected:
    [[nodiscard]] int enter_sandbox() const;
    [[nodiscard]] int execute_script() const;
    static GRADE_STATUS interpret_wstatus(int wstatus);

public:
    explicit RunConfig() : script(), constraint() {}
    virtual ~RunConfig() = default;
    
    // these functions should be overridden
    virtual void update(const YAML::Node& config);
    virtual GRADE_STATUS grade(int test_id) const = 0;
    
    static RunConfig* get_config(const std::string& type = "");
    
};


#endif //GRADER_SANDBOX_RUN_HPP
