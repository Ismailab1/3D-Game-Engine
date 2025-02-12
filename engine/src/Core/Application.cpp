#include "Core/Application.h"
#include "Core/Input.h"       // If you need input in your loop
#include "Utils/Logger.h"     // For logging macros

namespace Core {

	void* g_WindowHandle = nullptr; // Global pointer to the active GLFW window

    // Constructor
    Application::Application()
        : m_Window(nullptr)
    {
    }

    // Destructor
    Application::~Application() {
        // Ensure we clean up
        Shutdown();
    }

    bool Application::Init() {
        // Optional: Initialize your logger system if you haven't done so already:
        // Logger::Init();

        m_Window = new Window("My GLFW Window", 1280, 720);
        if (!m_Window->Init()) {
            LOG_ENGINE_ERROR("Failed to initialize the Window!");
            return false;
        }

        LOG_ENGINE_INFO("Window initialized successfully!");

        return true;
    }

    void Application::Run() {
        // Main game/engine loop
        while (!m_Window->ShouldClose()) {
            // 1) Poll window events
            m_Window->PollEvents();

            // 2) Update input states
            Input::Update();

            // 3) Update your game logic
            // ...

            // 4) Render
            // ...
        }
    }

    void Application::Shutdown() {
        if (m_Window) {
            m_Window->Shutdown();
            delete m_Window;
            m_Window = nullptr;
            LOG_ENGINE_INFO("Window shut down successfully!");
        }
    }

} // namespace Core
