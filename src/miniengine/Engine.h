#pragma once
#include <memory>
#include <string>

#include "core/window.h"

namespace MiniEngine {
    class ResourceManager;
    class Scene;

    class Engine {
    public:
        Engine();
        ~Engine();

        bool init(const std::string& title, Vector2 size);
        void setScene(Scene* newScene);
        void setFps(int newFps);
        [[nodiscard]] ResourceManager& getResourceManager() const;
        void run();
        void shutdown() const;

    private:
        int fps;
        bool isRunning;
        std::unique_ptr<ResourceManager> resourceManager;
        Scene* scene;

        Window* window;
    };

} // namespace miniengine
