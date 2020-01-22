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

class CaseResult {
private:
    GRADE_STATUS result;
    int time_ms;
    int memory_kb;

public:
    [[nodiscard]] GRADE_STATUS get_result() const { return this->result; }
    [[nodiscard]] int get_time_ms() const { return this->time_ms; }
    [[nodiscard]] int get_memory_kb() const { return this->memory_kb; }
    
    CaseResult() = default;
    CaseResult (GRADE_STATUS result, int time_ms, int memory_kb) :
      result(result), time_ms(time_ms), memory_kb(memory_kb) {}
    ~CaseResult() = default;
    
    void add(const CaseResult& result);
    
    static const CaseResult Ok;
    static const CaseResult SkipResult;
    static const CaseResult SystemError;
};

std::ostream& operator << (std::ostream& out, const CaseResult& case_result);

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
    virtual CaseResult grade(int test_id) const = 0;
    
    static RunConfig* get_config(const std::string& type = "");
    static CaseResult create_result_from_process(GRADE_STATUS status);
    
};

CaseResult grade_script(const RunConfig* config, int test_id);

#endif //GRADER_SANDBOX_RUN_HPP
