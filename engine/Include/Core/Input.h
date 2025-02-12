#pragma once

namespace Core {

    enum class KeyCode {
        W, A, S, D,
        ESCAPE,
        SPACE,
        MAX_KEYS // Keep this last to define array sizes, etc.
    };

    class Input {
    public:
        static void Update();

        static bool IsKeyDown(KeyCode key);
        static bool IsKeyUp(KeyCode key);

        static bool  IsMouseButtonDown(int button);
        static float GetMouseX();
        static float GetMouseY();

    private:
        // Internal arrays to store which keys/mouse are down
        static bool  s_KeysDown[(int)KeyCode::MAX_KEYS];
        static bool  s_MouseButtons[5];
        static float s_MouseX;
        static float s_MouseY;
    };

} // namespace Core
