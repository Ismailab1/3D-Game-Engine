#define CATCH_CONFIG_RUNNER
#include <catch2/catch_all.hpp>

// Include your logger header
#include "../../engine/include/Utils/Logger.h"

int main(int argc, char* argv[])
{
    // 1) First, init the logger:
    Logger::Init();
    // 2) Now run all Catch2 tests
    return Catch::Session().run(argc, argv);
}
