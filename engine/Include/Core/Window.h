#pragma once

#include <string>

namespace Core {

    /**
     * @class Window
     * @brief Encapsulates a GLFW window and handles basic event polling.
     */
    class Window {
    public:
        Window(const std::string& title, int width, int height);
        ~Window();

        bool Init();
        void Shutdown();

        void PollEvents();
        bool ShouldClose() const;

        int  GetWidth() const;
        int  GetHeight() const;
        void SetTitle(const std::string& newTitle);

        // (Optional) Return the raw pointer if needed by Input or rendering code
        void* GetNativeHandle() const;

    private:
        void* m_WindowHandle; // Will store a GLFWwindow*
        bool        m_ShouldClose;
        int         m_Width;
        int         m_Height;
        std::string m_Title;
    };

} // namespace Core
