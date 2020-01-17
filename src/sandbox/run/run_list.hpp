//
// Created by phirasit on 1/16/20.
//

#ifndef GRADER_RUN_LIST_HPP
#define GRADER_RUN_LIST_HPP

#include "run.hpp"

#include <string>

#define DEFAULT_RUN_CONFIG "DEFAULT"

class RunConfigList {
public:
    [[nodiscard]]
    static RunConfig* get_run_config(const std::string& type = DEFAULT_RUN_CONFIG);
};

#endif //GRADER_RUN_LIST_HPP
