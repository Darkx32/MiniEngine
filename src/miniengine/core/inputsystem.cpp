//
// Created by matheus on 11/09/2026.
//

#include "inputsystem.h"

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keyboard.h"

namespace MiniEngine
{
    static bool SDLCALL handleWindowEvents(void* windowdata, SDL_Event* event)
    {
        const auto* windowData = static_cast<Window::WindowSharedData*>(windowdata);

        if (event->type == SDL_EVENT_QUIT || event->type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
        {
            *windowData->isRunning = false;
            return false;
        }
        if (event->type == SDL_EVENT_WINDOW_RESIZED)
        {
            *windowData->hasResized = true;
        }

        return true;
    }

    InputSystem::~InputSystem()
    {
        if (p_windowData)
            SDL_RemoveEventWatch(handleWindowEvents, p_windowData);
    }

    bool InputSystem::isKeyDown(KeyCode keyCode) const
    {
        return currentStateKeyBoard.test(static_cast<size_t>(keyCode));
    }

    bool InputSystem::isKeyPressed(KeyCode keyCode) const
    {
        return currentStateKeyBoard.test(static_cast<size_t>(keyCode)) && !previousStateKeyBoard.test(static_cast<size_t>(keyCode));
    }

    bool InputSystem::isKeyReleased(KeyCode keyCode) const
    {
        return !currentStateKeyBoard.test(static_cast<size_t>(keyCode)) && previousStateKeyBoard.test(static_cast<size_t>(keyCode));
    }

    bool InputSystem::isMouseDown(const MouseButton mouseButton) const
    {
        const uint8_t mask = toButtonMask(mouseButton);
        return (currentStateMouseButton & mask) != 0;
    }

    bool InputSystem::isMousePressed(const MouseButton mouseButton) const
    {
        const uint8_t mask = toButtonMask(mouseButton);
        return currentStateMouseButton & mask && !(previousStateMouseButton & mask);
    }

    bool InputSystem::isMouseReleased(const MouseButton mouseButton) const
    {
        const uint8_t mask = toButtonMask(mouseButton);
        return !(currentStateMouseButton & mask) && previousStateMouseButton & mask;
    }

    void InputSystem::eventWatchWindow(Window::WindowSharedData* windowData)
    {
        SDL_AddEventWatch(handleWindowEvents, windowData);
        p_windowData = windowData;
    }

    void InputSystem::update()
    {
        SDL_PumpEvents();
        previousStateKeyBoard = currentStateKeyBoard;

        int numKeys = 0;
        keys = SDL_GetKeyboardState(&numKeys);

        for (std::size_t i = 0; i < numKeys; i++)
        {
            currentStateKeyBoard.set(i, keys[i]);
        }
    }

    uint8_t InputSystem::toButtonMask(MouseButton mouseButton)
    {
        if (mouseButton == MouseButton::None) return 0;
        return static_cast<uint8_t>(1 << (static_cast<uint8_t>(mouseButton) - 1));
    }
} // Miniaudio


