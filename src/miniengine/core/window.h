//
// Created by matheus on 11/09/2026.
//

#ifndef MINIENGINE_WINDOW_H
#define MINIENGINE_WINDOW_H
#include <string>

#include "../math/vector2.h"

struct SDL_Window;

namespace MiniEngine
{
    class Window
    {
    public:
        Window(const char* title, Vector2 size);
        ~Window();

        struct WindowSharedData
        {
            bool* isRunning;
            bool* hasResized;
        };

        [[nodiscard]] bool windowShouldClose() const;
        [[nodiscard]] int getWidth() const;
        [[nodiscard]] int getHeight() const;

    private:
        friend class Engine;

        bool init();
        void shutdown();

        [[nodiscard]] void* getNativeWindowHandle() const;
        [[nodiscard]] void* getNativeDisplayType() const;

        SDL_Window *window;
        WindowSharedData windowSharedData;
        bool isRunning;
        bool hasResized;

        std::string title;
        int width, height;
    };
}

#endif //MINIENGINE_WINDOW_H
