//
// Created by phirasit on 1/11/20.
//

#include "test_result.hpp"

void TestResult::report(std::ostream& out) const {
  for (const auto& group_id : this->group_order) {
    const GRADE_STATUS _group_result = this->get_group_result(group_id).value_or(GRADE_STATUS_SKIP);
    out << group_id << ": " << _group_result << std::endl;
    if (_group_result != GRADE_STATUS_SKIP) {
      for (const auto &test_id : this->groups.at(group_id).get_tests()) {
        const GRADE_STATUS _test_result = this->get_test_result(test_id).value_or(GRADE_STATUS_SKIP);
        out << " >> " << test_id << ": " << _test_result << std::endl;
      }
    }
  }
}
