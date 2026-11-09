#pragma once
#include <memory>

#include "core/scene.h"
#include "miniengine/core/window.h"

namespace MiniEngine
{
    class ResourceManager;
    class Scene;

    class Engine
    {
    public:
        Engine();
        ~Engine();

        bool init(const std::string& title, Vector2 size);
        void setFps(int newFps);
        [[nodiscard]] ResourceManager& getResourceManager() const;
        void run();
        void shutdown() const;

        template <typename T>
        T& setScene()
        {
            static_assert(std::is_base_of_v<Scene, T>, "T has been a scene");

            currentScene = std::make_unique<T>();
            if (isRunning)
                initializeScene();

            return static_cast<T&>(*currentScene);
        }

        template <typename T>
        T* getScene()
        {
            return dynamic_cast<T*>(currentScene.get());
        }

        [[nodiscard]] bool hasState() const
        {
            return currentScene != nullptr;
        }

    private:
        int fps;
        bool isRunning;
        bool isDebug;
        bool showDebugUI;
        std::unique_ptr<ResourceManager> resourceManager;
        std::unique_ptr<Scene> currentScene;

        void initializeScene();

        std::unique_ptr<Window> window;
    };
} // namespace miniengine
