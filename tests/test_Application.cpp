#include <catch2/catch_all.hpp>
#include "Core/Application.h"
#include "Utils/Logger.h"

TEST_CASE("Application lifecycle", "[application]") {
    Logger::Init();  // Ensure logger is initialized

    Core::Application app;

    SECTION("Init and run the application") {
        REQUIRE(app.Init() == true);

        // For a real test, you might run `app.Run()` in a separate thread or set up a condition
        // to close the window. That can get tricky in a test environment.
        //
        // We'll just call it for a short time or skip if it blocks forever:
        // This is more of an "integration test" approach.
        //
        // For a quick test, let's just do something minimal:
        // e.g. mimic one iteration of the main loop:

        // We won't do a full "Run" call because it's an infinite loop in most engines.
        // Instead, we can test the loop logic:

        // Pseudocode:
        // while (!app.m_Window->ShouldClose()) {
        //     app.m_Window->PollEvents();
        //     Input::Update();
        //     // ...
        // }

        // But we do confirm it doesn't crash on PollEvents:
        // Instead of app.m_Window->PollEvents();
        app.GetWindow()->PollEvents();


        app.Shutdown();
        SUCCEED("Application started and shut down without crashing.");
    }
}
