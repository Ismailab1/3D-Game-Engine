#include <catch2/catch_all.hpp>
#include "Core/Window.h"

/*
 * Basic tests for the Window class.
 * NOTE: These tests will create an actual GLFW window (though hopefully minimized or hidden),
 *       so they may not be fully "headless."
 */

TEST_CASE("Window initialization and shutdown", "[window]") {
    Core::Window window("Test Window", 800, 600);

    SECTION("Initialize the window") {
        bool initResult = window.Init();
        REQUIRE(initResult == true);
        REQUIRE(window.GetWidth() == 800);
        REQUIRE(window.GetHeight() == 600);
        REQUIRE_FALSE(window.ShouldClose());

        // Clean up
        window.Shutdown();
    }

    SECTION("Shutdown is safe to call multiple times") {
        // Even if we never call Init(), calling Shutdown shouldn't crash
        window.Shutdown();
        // And calling it again is still safe
        window.Shutdown();
    }
}

TEST_CASE("Window set title", "[window]") {
    Core::Window window("Initial Title", 640, 480);
    REQUIRE(window.Init() == true);

    SECTION("Change window title") {
        window.SetTitle("New Test Title");
        // There's no direct getter for the title in our example,
        // but if there were, we could check it.
        // Alternatively, we just ensure no crash or error occurs.
    }

    // Cleanup
    window.Shutdown();
}
