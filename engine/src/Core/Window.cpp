#include "Core/Window.h"
#include <GLFW/glfw3.h>
#include <iostream>  // For basic error logs if needed

namespace Core {

    Window::Window(const std::string& title, int width, int height)
        : m_WindowHandle(nullptr)
        , m_ShouldClose(false)
        , m_Width(width)
        , m_Height(height)
        , m_Title(title)
    {
    }

    Window::~Window() {
        Shutdown();
    }

    bool Window::Init() {
        // 1. Initialize GLFW if not already
        if (!glfwInit()) {
            std::cerr << "[Window] Failed to initialize GLFW.\n";
            return false;
        }

        // 2. (Optional) Set hints if you want an OpenGL context
        //    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        //    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        //    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // 3. Create the GLFW window
        GLFWwindow* window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
        if (!window) {
            std::cerr << "[Window] Failed to create GLFW window.\n";
            glfwTerminate();
            return false;
        }

        m_WindowHandle = window;

        // If using OpenGL for rendering, make the context current
        // glfwMakeContextCurrent(window);

        return true;
    }

    void Window::Shutdown() {
        // Destroy the GLFW window and terminate
        if (m_WindowHandle) {
            glfwDestroyWindow(static_cast<GLFWwindow*>(m_WindowHandle));
            m_WindowHandle = nullptr;
        }
        glfwTerminate();
    }

    void Window::PollEvents() {
        glfwPollEvents();

        // Check if the user closed the window
        if (m_WindowHandle) {
            GLFWwindow* window = static_cast<GLFWwindow*>(m_WindowHandle);
            if (glfwWindowShouldClose(window)) {
                m_ShouldClose = true;
            }
        }
    }

    bool Window::ShouldClose() const {
        return m_ShouldClose;
    }

    int Window::GetWidth() const {
        return m_Width;
    }

    int Window::GetHeight() const {
        return m_Height;
    }

    void Window::SetTitle(const std::string& newTitle) {
        m_Title = newTitle;
        if (m_WindowHandle) {
            glfwSetWindowTitle(static_cast<GLFWwindow*>(m_WindowHandle), m_Title.c_str());
        }
    }

    void* Window::GetNativeHandle() const {
        return m_WindowHandle;
    }

} // namespace Core
