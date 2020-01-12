//
// Created by phirasit on 1/5/20.
//

#ifndef GRADER_SANBOX_GROUP_HPP
#define GRADER_SANBOX_GROUP_HPP

#include "yaml-cpp/yaml.h"

class Group {
private:
    std::vector<int> tests;
    std::vector<std::string> dependency;
    
public:
    const std::vector<std::string>& get_dependency() const { return this->dependency; }
    const std::vector<int>& get_tests() const { return this->tests; }
    
    void update(const YAML::Node& config);
};

#endif //GRADER_SANBOX_GROUP_HPP
