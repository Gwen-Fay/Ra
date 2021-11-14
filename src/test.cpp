/**
 *
 * Catch2 Tests are all located here (if aditional cpp files are needed,
 * reformat this to test01.cpp, test02.cpp, ... etc)
 *
 * TODO make actual tests instead this dummy test
 *
 */

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

int AddOne(int x) { return x + 1; }

TEST_CASE("Add one Test") {
    REQUIRE(AddOne(1) == 2);
    REQUIRE(AddOne(2) == 3);
}
