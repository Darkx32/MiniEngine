//
// Created by matheus on 11/09/2026.
//

#ifndef MINIENGINE_WINDOW_H
#define MINIENGINE_WINDOW_H
#include <string>

#include "vector2.h"

struct SDL_Window;

namespace MiniEngine
{
    class Window
    {
    public:
        Window(const char* title, Vector2 size);
        ~Window();

        [[nodiscard]] bool windowShouldClose() const;

    private:
        friend class Engine;

        bool init();
        void shutdown();

        SDL_Window *window;
        bool isRunning;

        std::string title;
        int width, height;
    };
}

#endif //MINIENGINE_WINDOW_H
