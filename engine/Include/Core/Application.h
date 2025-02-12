#pragma once

#include "Core/Window.h"

namespace Core {

    /**
     * @class Application
     * @brief Manages the main engine loop (initialization, update, shutdown).
     */
    class Application {
    public:
        Application();
        ~Application();

        bool Init();
        void Run();
        void Shutdown();

        Window* GetWindow() const { return m_Window; }

    private:
        Window* m_Window;  // Pointer to your window object
    };

} // namespace Core
