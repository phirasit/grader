//
// Created by phirasit on 1/11/20.
//

#ifndef GRADER_TEST_RESULT_HPP
#define GRADER_TEST_RESULT_HPP

#include "run/run.hpp"
#include "group.hpp"
#include "logger.hpp"

#include <optional>
#include <ostream>
#include <vector>

class TestResult {
private:
    std::map<int, CaseResult> test_result;
    std::map<std::string, CaseResult> group_result;
    std::vector<std::string> group_order;
    Groups groups;

public:
    explicit TestResult(Groups groups, std::vector<std::string> group_order):
      groups(std::move(groups)), group_order(std::move(group_order)) {}
    ~TestResult() = default;
    
    [[nodiscard]] const std::vector<std::string>& get_group_order() const {
      return this->group_order;
    }
    
    [[nodiscard]] const Groups& get_groups() const { return this->groups; }
    
    [[nodiscard]] std::optional<CaseResult> get_test_result(int test_id) const {
      return this->test_result.find(test_id) != this->test_result.end()
             ? std::make_optional(this->test_result.at(test_id))
             : std::nullopt;
    }
    
    [[nodiscard]] std::optional<CaseResult> get_group_result(const std::string& group_id) const {
      return this->group_result.find(group_id) != this->group_result.end()
             ? std::make_optional(this->group_result.at(group_id))
             : std::nullopt;
    }
    
    void set_test_result(int test_id, const CaseResult& status) {
      this->test_result.insert({test_id, status});
    }
    
    void set_group_result(const std::string& group_id, const CaseResult& status) {
      this->group_result.insert({group_id, status});
    }
};

std::ostream& operator << (std::ostream& out, const TestResult& result);

#endif //GRADER_TEST_RESULT_HPP
