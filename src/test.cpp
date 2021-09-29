#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

int AddOne(int x) { return x + 1; }

TEST_CASE("Add one Test") {
    REQUIRE(AddOne(1) == 2);
    REQUIRE(AddOne(2) == 3);
}
