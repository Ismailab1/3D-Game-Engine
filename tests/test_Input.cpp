#include <catch2/catch_all.hpp>
#include "Core/Input.h"

/*
 * Tests for the Input class.
 * These tests won't actually simulate real keypresses or mouse clicks
 * unless you integrate with the OS or generate synthetic GLFW events.
 * Instead, we verify that the code runs without crashing and has the expected
 * default behavior.
 */

TEST_CASE("Input default states", "[input]") {
    // Before updating, everything should be false or 0
    for (int i = 0; i < (int)Core::KeyCode::MAX_KEYS; i++) {
        REQUIRE(Core::Input::IsKeyDown((Core::KeyCode)i) == false);
        REQUIRE(Core::Input::IsKeyUp((Core::KeyCode)i) == true);
    }

    // Mouse
    for (int b = 0; b < 5; b++) {
        REQUIRE(Core::Input::IsMouseButtonDown(b) == false);
    }
    REQUIRE(Core::Input::GetMouseX() == 0.0f);
    REQUIRE(Core::Input::GetMouseY() == 0.0f);
}

TEST_CASE("Input update call", "[input]") {
    // If we had a global pointer to the active window, we might need to set that up here,
    // e.g., g_WindowHandle = someValidGLFWwindowPointer;
    // For now, just ensure it doesn't crash:
    Core::Input::Update();

    // We can't easily test real key presses here without a window and event injection,
    // so we just call Update() and check no crash occurs.
    SUCCEED("Input::Update() executed without crash");
}
