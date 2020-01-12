//
// Created by phirasit on 1/11/20.
//

#ifndef GRADER_TEST_RESULT_HPP
#define GRADER_TEST_RESULT_HPP

#include "single_run.hpp"

#include <optional>
#include <ostream>
#include <vector>
#include <logger.hpp>

class TestResult {
private:
    std::map<int, GRADE_STATUS> test_result;
    std::map<std::string, GRADE_STATUS> group_result;
    std::vector<std::string> group_order;

public:
    explicit TestResult(std::vector<std::string> group_order):
      group_order(std::move(group_order)) {}
    ~TestResult() = default;
    
    const decltype(group_result)& get_result() const { return this->group_result; }
    
    std::optional<GRADE_STATUS> get_test_result(int test_id) const {
      return this->test_result.find(test_id) != this->test_result.end()
             ? std::make_optional(this->test_result.at(test_id))
             : std::nullopt;
    }
    std::optional<GRADE_STATUS> get_group_result(const std::string& group_id) const {
      return this->group_result.find(group_id) != this->group_result.end()
             ? std::make_optional(this->group_result.at(group_id))
             : std::nullopt;
    }
    void set_test_result(int test, GRADE_STATUS status) { this->test_result[test] = status; }
    void set_group_result(const std::string& group_id, GRADE_STATUS status) {
      this->group_result[group_id] = status;
    }
  
    void report(std::ostream& out) const;
};

#endif //GRADER_TEST_RESULT_HPP
