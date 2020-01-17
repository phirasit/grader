//
// Created by phirasit on 1/5/20.
//

#ifndef GRADER_SANDBOX_SUBMISSION_CONFIG_HPP
#define GRADER_SANDBOX_SUBMISSION_CONFIG_HPP

#include "file.hpp"
#include "group.hpp"
#include "script.hpp"
#include "run/run.hpp"
#include "test_result.hpp"

#include "yaml-cpp/yaml.h"

#include <map>
#include <optional>
#include <ostream>
#include <vector>

enum GRADE_POLICY {
    GRADE_POLICY_GRADE_ALL = 0x00,
    GRADE_POLICY_SKIP_TEST_IF_PREVIOUS_FAILED = 0x01,
    GRADE_POLICY_SKIP_GROUP_IF_DEPENDENCY_FAILED = 0x02,
    GRADE_POLICY_SKIP_IF_CANNOT_BE_CORRECT = 0x03, // SKIP_TEST | SKIP_GROUP
};

static const std::map<std::string, GRADE_POLICY> GRADE_POLICY_MAPPING = {
  {"GRADE_ALL", GRADE_POLICY_GRADE_ALL},
  {"SKIP_TEST", GRADE_POLICY_SKIP_TEST_IF_PREVIOUS_FAILED},
  {"SKIP_GROUP", GRADE_POLICY_SKIP_GROUP_IF_DEPENDENCY_FAILED},
  {"SKIP", GRADE_POLICY_SKIP_IF_CANNOT_BE_CORRECT},
};


class SubmissionConfig {
private:
    int number_cases;
    RunConfig* run_config;
    // std::map<int, RunConfig> special_cases;
    GRADE_POLICY grade_policy;
    std::map<std::string, Group> groups;
    std::vector<std::string> group_order;
    std::optional<file::File> output;
    
public:
    SubmissionConfig() : number_cases(0), run_config(nullptr), grade_policy(GRADE_POLICY_GRADE_ALL) {}
    ~SubmissionConfig() = default;
    
    // std::optional<RunConfig> get_case(int case_num);
    void update(const YAML::Node& config);
    bool validate();
    
    [[nodiscard]] const std::optional<file::File>& get_output() const { return this->output; }
    
    [[nodiscard]] TestResult grade() const;
};

#endif //GRADER_SANDBOX_SUBMISSION_CONFIG_HPP
