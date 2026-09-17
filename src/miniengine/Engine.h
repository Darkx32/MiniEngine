#pragma once
#include <string>

#include "core/window.h"

namespace MiniEngine {
    class Scene;

    class Engine {
    public:
        Engine();
        ~Engine();

        bool init(const std::string& title, Vector2 size);
        void setScene(Scene* newScene);
        void setFps(int newFps);
        void run();
        void shutdown() const;

    private:
        int fps;
        bool isRunning;
        Scene* scene;

        Window* window;
    };

} // namespace miniengine
