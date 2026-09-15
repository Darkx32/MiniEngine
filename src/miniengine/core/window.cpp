//
// Created by matheus on 11/09/2026.
//

#include "window.h"

#include "SDL3/SDL_init.h"
#include "spdlog/spdlog.h"

namespace MiniEngine
{
    Window::Window(const char* title, const Vector2 size) : window(nullptr), isOpen(false), hasResized(false)
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
        return isOpen;
    }

    int Window::getWidth() const
    {
        return width;
    }

    int Window::getHeight() const
    {
        return height;
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

        isOpen = true;
        return isOpen;
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

    void Window::pollEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT || event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
                isOpen = false;
            if (event.type == SDL_EVENT_WINDOW_RESIZED)
                hasResized = true;
        }
    }

    void* Window::getNativeWindowHandle() const
    {
        const SDL_PropertiesID props = SDL_GetWindowProperties(window);

        #if defined(SDL_PLATFORM_WINDOWS)
                return SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);

        #elif defined(SDL_PLATFORM_MACOS)
                return SDL_GetPointerProperty(props, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr);

        #elif defined(SDL_PLATFORM_LINUX)
                if (void* waylandSurface = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, nullptr)) {
                    return waylandSurface;
                }
                const Uint64 x11Window = SDL_GetNumberProperty(props, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
                return reinterpret_cast<void*>(static_cast<uintptr_t>(x11Window));

        #else
                return nullptr;
        #endif
    }

    void* Window::getNativeDisplayType() const
    {
        #if defined(SDL_PLATFORM_LINUX)
            const SDL_PropertiesID props = SDL_GetWindowProperties(window);

            if (void* waylandDisplay = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, nullptr)) {
                return waylandDisplay;
            }
            return SDL_GetPointerProperty(props, SDL_PROP_WINDOW_X11_DISPLAY_POINTER, nullptr);
        #else
                return nullptr;
        #endif
    }
}

