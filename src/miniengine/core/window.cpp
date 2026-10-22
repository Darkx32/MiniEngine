//
// Created by matheus on 11/09/2026.
//

#include "miniengine/core/window.h"

#include "miniengine/core/debugui.h"

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
        return !isOpen;
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
            DebugUI::update(&event);

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
        return SDL_GetPointerProperty(
            props,
            SDL_PROP_WINDOW_WIN32_HWND_POINTER,
            nullptr
        );

#elif defined(SDL_PLATFORM_MACOS)
        return SDL_GetPointerProperty(
            props,
            SDL_PROP_WINDOW_COCOA_WINDOW_POINTER,
            nullptr
        );

#elif defined(SDL_PLATFORM_LINUX)
        const char* driver = SDL_GetCurrentVideoDriver();

        if (driver && SDL_strcmp(driver, "wayland") == 0)
        {
            return SDL_GetPointerProperty(
                props,
                SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER,
                nullptr
            );
        }

        if (driver && SDL_strcmp(driver, "x11") == 0)
        {
            const Uint64 x11Window = SDL_GetNumberProperty(
                props,
                SDL_PROP_WINDOW_X11_WINDOW_NUMBER,
                0
            );

            return reinterpret_cast<void*>(
                static_cast<uintptr_t>(x11Window)
            );
        }

        return nullptr;
#else
        return nullptr;
#endif
    }

    void* Window::getNativeDisplayType() const
    {
#if defined(SDL_PLATFORM_LINUX)
        const SDL_PropertiesID props = SDL_GetWindowProperties(window);
        const char* driver = SDL_GetCurrentVideoDriver();

        if (driver && SDL_strcmp(driver, "wayland") == 0)
        {
            return SDL_GetPointerProperty(
                props,
                SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER,
                nullptr
            );
        }

        if (driver && SDL_strcmp(driver, "x11") == 0)
        {
            return SDL_GetPointerProperty(
                props,
                SDL_PROP_WINDOW_X11_DISPLAY_POINTER,
                nullptr
            );
        }

        return nullptr;
#else
        return nullptr;
#endif
    }

    bool Window::getNativeWindowHandleType()
    {
#if defined(SDL_PLATFORM_LINUX)

        if (const char* driver = SDL_GetCurrentVideoDriver())
        {
            if (SDL_strcmp(driver, "wayland") == 0)
                return bgfx::NativeWindowHandleType::Wayland;

            if (SDL_strcmp(driver, "x11") == 0)
                return bgfx::NativeWindowHandleType::Default;
        }

#elif defined(SDL_PLATFORM_WINDOWS)
        return bgfx::NativeWindowHandleType::Default;

#elif defined(SDL_PLATFORM_MACOS)
        return bgfx::NativeWindowHandleType::Default;
#endif

        return bgfx::NativeWindowHandleType::Default;
    }
}
