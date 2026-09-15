//
// Created by matheus on 11/09/2026.
//

#ifndef MINIENGINE_INPUTSYSTEM_H
#define MINIENGINE_INPUTSYSTEM_H
#include <bitset>
#include <cstdint>

#include "window.h"

namespace MiniEngine
{
    class InputSystem
    {
    public:
        enum class MouseButton : uint8_t
        {
            None = 0,
            Left,
            Middle,
            Right,
            X1,
            X2
        };

        enum class KeyCode : uint16_t
        {
            Unknown = 0,

            A = 4, B = 5, C = 6, D = 7, E = 8, F = 9, G = 10,
            H = 11, I = 12, J = 13, K = 14, L = 15, M = 16, N = 17,
            O = 18, P = 19, Q = 20, R = 21, S = 22, T = 23, U = 24,
            V = 25, W = 26, X = 27, Y = 28, Z = 29,

            Num1 = 30, Num2 = 31, Num3 = 32, Num4 = 33, Num5 = 34,
            Num6 = 35, Num7 = 36, Num8 = 37, Num9 = 38, Num0 = 39,

            Return = 40,
            Escape = 41,
            Backspace = 42,
            Tab = 43,
            Space = 44,

            LeftCtrl = 224,
            LeftShift = 225,
            LeftAlt = 226,
            LeftGui = 227,
            RightCtrl = 228,
            RightShift = 229,
            RightAlt = 230,
            RightGui = 231
        };

        [[nodiscard]] static bool isKeyDown(KeyCode keyCode);
        [[nodiscard]] static bool isKeyPressed(KeyCode keyCode);
        [[nodiscard]] static bool isKeyReleased(KeyCode keyCode);
        [[nodiscard]] static bool isMouseDown(MouseButton mouseButton);
        [[nodiscard]] static bool isMousePressed(MouseButton mouseButton);
        [[nodiscard]] static bool isMouseReleased(MouseButton mouseButton);
        [[nodiscard]] static Vector2 getMousePosition();

    private:
        friend class Engine;
        static void update();
        static uint8_t toButtonMask(MouseButton mouseButton);

        static float mouseX, mouseY;
    };
} // Miniaudio

#endif //MINIENGINE_INPUTSYSTEM_H
