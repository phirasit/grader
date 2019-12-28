//
// Created by phirasit on 12/27/19.
//
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

int factorial(int x) {
  return x <= 1 ? 1 : factorial(x-1) * x;
}

TEST_CASE( "factorials are computed", "[factorial]" ) {
  REQUIRE( factorial(2) == 2 );
  REQUIRE( factorial(3) == 6 );
  REQUIRE( factorial(10) == 3628800 );
}
