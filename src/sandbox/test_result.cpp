//
// Created by phirasit on 1/11/20.
//

#include "test_result.hpp"

void TestResult::report(std::ostream& out) const {
  const auto test_result = this->get_result();
  for (const auto& group_id : this->group_order) {
    out << group_id << " " << test_result.at(group_id) << std::endl;
  }
}
