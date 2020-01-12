//
// Created by phirasit on 12/31/19.
//

#ifndef GRADER_SANDBOX_SINGLE_RUN_HPP
#define GRADER_SANDBOX_SINGLE_RUN_HPP

#include "yaml-cpp/yaml.h"
#include "run.hpp"

#include <ostream>

enum GRADE_STATUS {
    GRADE_STATUS_OK,
    GRADE_STATUS_FAILED,
    GRADE_STATUS_SKIP,
    GRADE_STATUS_TIMEOUT,
    GRADE_STATUS_MEMORY_ERROR,
    GRADE_STATUS_RUNTIME_ERROR,
};

std::ostream& operator << (std::ostream& out, GRADE_STATUS status);

GRADE_STATUS run_script(const RunConfig& config, int test_id);

#endif //GRADER_SANDBOX_SINGLE_RUN_HPP
