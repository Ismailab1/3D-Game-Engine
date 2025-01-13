#include <catch2/catch_test_macros.hpp>

TEST_CASE("Simple Math Test") {
    REQUIRE(1 + 1 == 2);
}

TEST_CASE("Multiplication Test") {
    REQUIRE(2 * 2 == 4);
}
