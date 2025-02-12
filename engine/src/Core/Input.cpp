#include "Core/Input.h"
#include <GLFW/glfw3.h>

namespace Core {

    // Define static data
    bool  Input::s_KeysDown[(int)KeyCode::MAX_KEYS] = { false };
    bool  Input::s_MouseButtons[5] = { false };
    float Input::s_MouseX = 0.0f;
    float Input::s_MouseY = 0.0f;

    // If you need access to the active GLFWwindow, you can store it globally or fetch it from Window
    static GLFWwindow* GetActiveWindow() {
        // This is just a placeholder. You could:
        // 1) Pass a pointer in Input::Update(...).
        // 2) Have an engine-wide pointer to the active GLFWwindow.
        // 3) Or store the pointer in a static variable from Window init.
        extern void* g_WindowHandle; // declared somewhere in your code
        return static_cast<GLFWwindow*>(g_WindowHandle);
    }

    void Input::Update() {
        GLFWwindow* window = GetActiveWindow();
        if (!window) return;

        // Update keys
        s_KeysDown[(int)KeyCode::W] = (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS);
        s_KeysDown[(int)KeyCode::A] = (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS);
        s_KeysDown[(int)KeyCode::S] = (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS);
        s_KeysDown[(int)KeyCode::D] = (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS);
        s_KeysDown[(int)KeyCode::ESCAPE] = (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS);
        s_KeysDown[(int)KeyCode::SPACE] = (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS);

        // Update mouse position
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        s_MouseX = static_cast<float>(xpos);
        s_MouseY = static_cast<float>(ypos);

        // Update mouse buttons
        s_MouseButtons[0] = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
        s_MouseButtons[1] = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
        s_MouseButtons[2] = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS);
    }

    bool Input::IsKeyDown(KeyCode key) {
        return s_KeysDown[(int)key];
    }

    bool Input::IsKeyUp(KeyCode key) {
        return !s_KeysDown[(int)key];
    }

    bool Input::IsMouseButtonDown(int button) {
        if (button < 0 || button >= 5)
            return false;
        return s_MouseButtons[button];
    }

    float Input::GetMouseX() {
        return s_MouseX;
    }

    float Input::GetMouseY() {
        return s_MouseY;
    }

} // namespace Core
