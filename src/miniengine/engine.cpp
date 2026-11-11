#include "miniengine/engine.h"
#include <spdlog/spdlog.h>

#include "bgfx/bgfx.h"
#include "core/audio.h"
#include "miniengine/core/debugui.h"
#include "miniengine/core/graphics.h"
#include "miniengine/core/inputsystem.h"
#include "miniengine/core/resourcemanager.h"
#include "miniengine/core/scene.h"
#include "miniengine/systems/physicssystem.h"
#include "miniengine/systems/rendersystem.h"
#include "miniengine/systems/scriptsystem.h"
#include "SDL3/SDL_timer.h"
#include "systems/audiosystem.h"

namespace MiniEngine
{
    engine::engine() : fps(60), isRunning(false), isDebug(false), showDebugUI(false), resourceManager(nullptr),
                       window(nullptr)
    {
    }

    engine::~engine()
    {
        shutdown();
    }

    bool engine::init(const std::string& title, const Vector2 size)
    {
#ifndef NDEBUG
        spdlog::set_level(spdlog::level::debug);
        isDebug = true;
#else
        isDebug = false;
#endif

        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
        spdlog::info("MiniEngine - Starting");
        spdlog::info("Starting miniengine...");

        window = std::make_unique<Window>(title.c_str(), size);
        if (!window->init())
        {
            spdlog::error("Failed to start Window");
            return false;
        }
        resourceManager = std::make_unique<ResourceManager>();

        if (!Graphics::initializePrograms({
            .resourceManager = resourceManager.get(), .nativeWindowHandle = window->getNativeWindowHandle(),
            .nativeDisplayType = window->getNativeDisplayType(),
            .width = static_cast<uint32_t>(window->getWidth()), .height = static_cast<uint32_t>(window->getHeight()),
            .type = Window::getNativeWindowHandleType()
        }))
        {
            spdlog::error("Failed to initialize Graphics");
            return false;
        }

        spdlog::info("Audio System initializing...");
        if (!AudioManager::initialize())
        {
            spdlog::error("Error to initialize Audio System");
            return false;
        }

        spdlog::info("Physics System initializing...");
        PhysicsSystem::initialize();

        spdlog::info("DebugUI System initializing...");
        DebugUI::initialize(window->window);

        spdlog::info("Miniengine started successfully.");
        showDebugUI = isDebug;
        return true;
    }

    void engine::setFps(const int newFps)
    {
        this->fps = newFps;
    }

    ResourceManager& engine::getResourceManager() const
    {
        return *this->resourceManager;
    }

    void engine::run()
    {
        if (currentScene)
            initializeScene();

        spdlog::info("Starting main loop...");
        uint64_t lastTime = SDL_GetPerformanceCounter();
        while (isRunning)
        {
            const uint64_t currentTime = SDL_GetPerformanceCounter();

            const auto dt = static_cast<float>(
                static_cast<double>(currentTime - lastTime) /
                static_cast<double>(SDL_GetPerformanceFrequency())
            );

            InputSystem::update();
            window->pollEvents();
            if (currentScene)
                PhysicsSystem::update(currentScene->registry.get(), dt);

            if (window->windowShouldClose() || (InputSystem::isKeyPressed(InputSystem::KeyCode::Escape) && isDebug))
                isRunning = false;
            if (InputSystem::isKeyPressed(InputSystem::KeyCode::F12)) showDebugUI = !showDebugUI;

            if (window->hasResized)
            {
                window->hasResized = false;
                bgfx::setViewRect(Graphics::DEFAULT, 0, 0, window->width, window->height);
                if (currentScene)
                    RenderSystem::calculate(currentScene->registry.get(),
                                            {static_cast<float>(window->width), static_cast<float>(window->height)});
            }

            if (currentScene)
                ScriptSystem::update(currentScene->registry.get(), dt);
            lastTime = currentTime;

            bgfx::setViewClear(Graphics::DEFAULT, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, Graphics::color, 1.0f, 0);

            if (currentScene)
                RenderSystem::render(currentScene->registry.get());

            if (showDebugUI && isDebug)
                DebugUI::render();

            bgfx::frame();

            const double frameTime = (static_cast<double>(SDL_GetPerformanceCounter()) -
                static_cast<double>(currentTime)) / static_cast<double>(SDL_GetPerformanceFrequency());

            if (const double targetFrameTime = 1.0 / fps; frameTime < targetFrameTime)
            {
                SDL_Delay(static_cast<uint32_t>(targetFrameTime - frameTime) * 1000);
            }
        }
    }

    void engine::shutdown() const
    {
        spdlog::info("Miniengine cleanup...");
        DebugUI::shutdown();
        resourceManager->clean();
        PhysicsSystem::shutdown();
        AudioManager::shutdown();
        Graphics::shutdown();

        spdlog::info("Miniengine finished successfully.");
    }

    void engine::initializeScene()
    {
        spdlog::info("Initializing scene...");

        currentScene->initialize();
        spdlog::info("Startup components...");
        ScriptSystem::startup(currentScene->registry.get());
        RenderSystem::calculate(currentScene->registry.get(),
                                {static_cast<float>(window->width), static_cast<float>(window->height)});
        AudioSystem::prepare(currentScene->registry.get());

        spdlog::info("Scene successfully initialized.");
        isRunning = true;
    }
} // namespace miniengine
