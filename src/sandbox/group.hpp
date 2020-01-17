//
// Created by phirasit on 1/5/20.
//

#ifndef GRADER_SANDBOX_GROUP_HPP
#define GRADER_SANDBOX_GROUP_HPP

#include "yaml-cpp/yaml.h"

#include <map>
#include <vector>

class Group {
private:
    std::vector<int> tests;
    std::vector<std::string> dependency;
    
public:
    [[nodiscard]]
    const std::vector<std::string>& get_dependency() const { return this->dependency; }
    
    [[nodiscard]]
    const std::vector<int>& get_tests() const { return this->tests; }
    
    void update(const YAML::Node& config);
};

#endif //GRADER_SANDBOX_GROUP_HPP
