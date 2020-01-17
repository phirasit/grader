//
// Created by phirasit on 1/5/20.
//
#include "submission_config.hpp"
#include "utility.hpp"
#include "logger.hpp"

#include <stdlib.h>
#include <functional>
#include <numeric>
#include <map>
#include <set>

static bool find_topo_order(const std::map<std::string, Group>& groups, std::vector<std::string>* order = nullptr) {
  std::set<std::string> visited, parent;
  if (order != nullptr) {
    order->clear();
  }
  const std::function<bool(const std::string& group_id)> valid_DAG =
    [&groups, &visited, &parent, &order, &valid_DAG] (const std::string& group_id) -> bool {
      if (groups.find(group_id) == groups.end()) return false;
      if (parent.count(group_id)) return false;
      if (visited.count(group_id)) return true;
      visited.insert(group_id);
  
      const auto& depends = groups.at(group_id).get_dependency();
  
      parent.insert(group_id);
      bool result = std::all_of(depends.begin(), depends.end(), [&valid_DAG] (const auto& child_id) -> bool {
          return valid_DAG(child_id);
      });
      parent.erase(group_id);
      
      if (order != nullptr) {
        order->emplace_back(group_id);
      }
  
      return result;
    };
  
  return std::all_of(groups.begin(), groups.end(), [&valid_DAG] (const auto &it) -> bool {
    return valid_DAG(it.first);
  });
}

bool SubmissionConfig::validate() {
  return find_topo_order(this->groups, nullptr);
}

void SubmissionConfig::update(const YAML::Node& config) {
  static const Logger logger("submission-config");
  if (!config) return;
  update_if_exists(this->number_cases, config["num-tests"]);
  if (config["run"]) {
    if (this->run_config == nullptr) {
      if (config["type"]) {
        this->run_config = RunConfig::get_config(config["type"].as<std::string>());
      } else {
        this->run_config = RunConfig::get_config();
      }
    }
    this->run_config->update(config["run"]);
  }
  update_if_exists<GRADE_POLICY, std::string>(this->grade_policy, config["policy"],
    [] (const std::string& policy) {
      if (GRADE_POLICY_MAPPING.find(policy) == GRADE_POLICY_MAPPING.end()) {
        logger("[warning] policy: ", policy, " not recognized");
        return GRADE_POLICY_GRADE_ALL;
      }
      return GRADE_POLICY_MAPPING.at(policy);
    });
  
  if (config["groups"]) {
    for (const auto& it : config["groups"]) {
      const std::string& group_id = it.first.as<std::string>();
      if (this->groups.find(group_id) == this->groups.end()) {
        this->group_order.push_back(group_id);
      }
      this->groups[group_id].update(it.second);
    }
  }
  update_if_exists<decltype(this->output), std::string>(this->output, config["output"]);
}

static void grade_group(const std::string& group_id, const std::vector<int>& test_ids, RunConfig* run_config, GRADE_POLICY policy, TestResult& result) {
  bool skip = false;
  GRADE_STATUS group_result = GRADE_STATUS_OK;
  for (int test_id : test_ids) {
    // skip if already been graded
    if (result.get_test_result(test_id).has_value()) {
      continue;
    }
    
    GRADE_STATUS test_result = skip ? GRADE_STATUS_SKIP : run_config->grade(test_id);
    skip |= ((policy & GRADE_POLICY_SKIP_TEST_IF_PREVIOUS_FAILED) && test_result != GRADE_STATUS_OK);
  
    result.set_test_result(test_id, test_result);
    group_result = std::max(group_result, test_result);
  }
  result.set_group_result(group_id, group_result);
}

TestResult SubmissionConfig::grade() const {
  static const Logger logger ("submission-config");
  
  TestResult result (this->groups, this->group_order);
  if (this->groups.empty()) {
    // no groups judge every case from 1 to n
    std::vector<int> tmp_group ((size_t) this->number_cases);
    std::iota(tmp_group.begin(), tmp_group.end(), 1);
    grade_group("", tmp_group, this->run_config, this->grade_policy, result);
    return result;
  }
  
  // find topological sort
  std::vector<std::string> order;
  if (!find_topo_order(this->groups, &order)) {
    logger("invalid group topology");
    exit(1);
  }
  
  // grade the test according to the topological order
  for (const std::string& group_id : order) {
    logger("start gradding group #", group_id);
    if (this->grade_policy & GRADE_POLICY_SKIP_GROUP_IF_DEPENDENCY_FAILED) {
      const std::vector<std::string>& deps = this->groups.at(group_id).get_dependency();
      if (std::any_of(deps.begin(), deps.end(), [&result] (const std::string& group_id) {
        return result.get_group_result(group_id).value() != GRADE_STATUS_OK;
      })) {
        result.set_group_result(group_id, GRADE_STATUS_SKIP);
        continue;
      }
    }
    grade_group(group_id, this->groups.at(group_id).get_tests(), this->run_config, this->grade_policy, result);
  }
  
  return result;
}
