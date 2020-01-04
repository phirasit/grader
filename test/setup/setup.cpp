//
// Created by phirasit on 1/1/20.
//

#include "catch.hpp"
#include "setup.hpp"

#include <stdlib.h>

int setup_test_system() {
  return !system(nullptr) || system("bin/setup");
}
