//
// Created by matheus on 11/09/2026.
//

#include "inputsystem.h"

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keyboard.h"

namespace MiniEngine
{
    static uint8_t currentStateMouseButton = 0;
    static uint8_t previousStateMouseButton = 0;
    static std::bitset<512> currentStateKeyBoard{0};
    static std::bitset<512> previousStateKeyBoard{0};
    float InputSystem::mouseX = 0;
    float InputSystem::mouseY = 0;

    bool InputSystem::isKeyDown(KeyCode keyCode)
    {
        return currentStateKeyBoard.test(static_cast<size_t>(keyCode));
    }

    bool InputSystem::isKeyPressed(KeyCode keyCode)
    {
        const auto idx = static_cast<size_t>(keyCode);
        return currentStateKeyBoard.test(idx) && !previousStateKeyBoard.test(idx);
    }

    bool InputSystem::isKeyReleased(KeyCode keyCode)
    {
        const auto idx = static_cast<size_t>(keyCode);
        return !currentStateKeyBoard.test(idx) && previousStateKeyBoard.test(idx);
    }

    bool InputSystem::isMouseDown(const MouseButton mouseButton)
    {
        const uint8_t mask = toButtonMask(mouseButton);
        return (currentStateMouseButton & mask) != 0;
    }

    bool InputSystem::isMousePressed(const MouseButton mouseButton)
    {
        const uint8_t mask = toButtonMask(mouseButton);
        return currentStateMouseButton & mask && !(previousStateMouseButton & mask);
    }

    bool InputSystem::isMouseReleased(const MouseButton mouseButton)
    {
        const uint8_t mask = toButtonMask(mouseButton);
        return !(currentStateMouseButton & mask) && previousStateMouseButton & mask;
    }

    Vector2 InputSystem::getMousePosition()
    {
        return {mouseX, mouseY};
    }

    void InputSystem::update()
    {
        previousStateMouseButton = currentStateMouseButton;
        previousStateKeyBoard = currentStateKeyBoard;

        SDL_PumpEvents();

        int numKeys = 0;
        const bool* keys = SDL_GetKeyboardState(&numKeys);

        if (keys != nullptr)
        {
            const size_t maxKeys = std::min(static_cast<size_t>(numKeys), currentStateKeyBoard.size());
            currentStateKeyBoard.reset(0);

            for (std::size_t i = 0; i < maxKeys; i++)
            {
                currentStateKeyBoard.set(i, keys[i]);
            }
        }
        currentStateMouseButton = SDL_GetMouseState(&mouseX, &mouseY);
    }

    uint8_t InputSystem::toButtonMask(MouseButton mouseButton)
    {
        if (mouseButton == MouseButton::None) return 0;
        return static_cast<uint8_t>(1 << (static_cast<uint8_t>(mouseButton) - 1));
    }
} // Miniaudio


