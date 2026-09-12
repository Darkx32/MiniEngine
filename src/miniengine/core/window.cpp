//
// Created by matheus on 11/09/2026.
//

#include "window.h"

#include "SDL3/SDL_init.h"
#include "spdlog/spdlog.h"

namespace MiniEngine
{
    Window::Window(const char* title, const Vector2 size) : window(nullptr), isRunning(false)
    {
        this->title = title;
        this->width = static_cast<int>(size.x);
        this->height = static_cast<int>(size.y);
    }

    Window::~Window()
    {
        shutdown();
    }

    bool Window::windowShouldClose() const
    {
        return isRunning;
    }

    bool Window::init()
    {
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
        {
            spdlog::error("SDL could not initialize!");
            return false;
        }

        window = SDL_CreateWindow(title.c_str(), width, height, 0);

        if (!window)
        {
            spdlog::error("Window could not be created!");
            return false;
        }

        isRunning = true;
        return true;
    }

    void Window::shutdown()
    {
        if (window)
        {
            SDL_DestroyWindow(window);
            window = nullptr;
        }

        SDL_Quit();
    }
}

