//
// Created by phirasit on 1/11/20.
//

#include "test_result.hpp"

std::ostream& operator << (std::ostream& out, const TestResult& test_result) {
  for (const auto& group_id : test_result.get_group_order()) {
    const CaseResult& _group_result = test_result.get_group_result(group_id).value_or(CaseResult::SkipResult);
    out << group_id << ": " << _group_result << std::endl;
    if (_group_result.get_result() != GRADE_STATUS_SKIP) {
      for (const auto &test_id : test_result.get_groups().at(group_id).get_tests()) {
        const CaseResult& _test_result = test_result.get_test_result(test_id).value_or(CaseResult::SkipResult);
        out << " >> " << test_id << ": " << _test_result << std::endl;
      }
    }
  }
  return out;
}
