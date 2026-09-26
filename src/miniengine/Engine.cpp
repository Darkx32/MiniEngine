#include "Engine.h"
#include <spdlog/spdlog.h>

#include "bgfx/bgfx.h"
#include "core/graphics.h"
#include "core/inputsystem.h"
#include "core/resourcemanager.h"
#include "core/scene.h"
#include "ecs/physicssystem.h"
#include "ecs/rendersystem.h"
#include "ecs/scriptsystem.h"
#include "SDL3/SDL_timer.h"

namespace MiniEngine {

Engine::Engine() : fps(60), isRunning(false), resourceManager(nullptr), scene(nullptr), window(nullptr)
{
}

Engine::~Engine() {
    shutdown();
}

bool Engine::init(const std::string& title, const Vector2 size) {
    #ifndef NDEBUG
    spdlog::set_level(spdlog::level::debug);
    #endif
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
    spdlog::info("MiniEngine - Starting");
    spdlog::info("Starting miniengine...");

    window = new Window(title.c_str(), size);
    if (!window->init())
    {
        spdlog::error("Failed to start Window");
        return false;
    }
    resourceManager = std::make_unique<ResourceManager>();

    if (!Graphics::initializePrograms({.resourceManager = resourceManager.get(), .nativeWindowHandle = window->getNativeWindowHandle(),
            .nativeDisplayType = window->getNativeDisplayType(),
            .width = static_cast<uint32_t>(window->getWidth()), .height = static_cast<uint32_t>(window->getHeight())}))
    {
        spdlog::error("Failed to initialize Graphics");
        return false;
    }

    spdlog::info("Physics System initialize");
    PhysicsSystem::initialize();

    spdlog::info("Miniengine started successfully.");
    isRunning = true;
    return isRunning;
}

void Engine::setScene(Scene* newScene)
{
    scene = newScene;
}

void Engine::setFps(const int newFps)
{
    this->fps = newFps;
}

ResourceManager& Engine::getResourceManager() const
{
    return *this->resourceManager;
}

void Engine::run() {
    spdlog::info("Starting main loop...");

    if (scene)
    {
        spdlog::info("Startup components...");
        ScriptSystem::startup(scene->registry);
        RenderSystem::calculate(scene->registry,
    {static_cast<float>(window->width), static_cast<float>(window->height)});
    }
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
        if (scene)
            PhysicsSystem::update(scene->registry, dt);

        if (window->windowShouldClose() || InputSystem::isKeyPressed(InputSystem::KeyCode::Escape))
            isRunning = false;

        if (window->hasResized)
        {
            window->hasResized = false;
            bgfx::setViewRect(Graphics::DEFAULT, 0, 0, window->width, window->height);
            if (scene)
                RenderSystem::calculate(scene->registry,
        {static_cast<float>(window->width), static_cast<float>(window->height)});
        }

        if (scene)
            ScriptSystem::update(scene->registry, dt);
        lastTime = currentTime;

        bgfx::setViewClear(Graphics::DEFAULT, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, Graphics::color, 1.0f, 0);

        if (scene)
            RenderSystem::render(scene->registry);

        bgfx::frame();

        const double frameTime = (static_cast<double>(SDL_GetPerformanceCounter()) -
                static_cast<double>(currentTime)) / static_cast<double>(SDL_GetPerformanceFrequency());

        if (const double targetFrameTime = 1.0 / fps; frameTime < targetFrameTime) {
            SDL_Delay(static_cast<uint32_t>(targetFrameTime - frameTime) * 1000);
        }
    }
}

void Engine::shutdown() const
{
    resourceManager->clean();
    PhysicsSystem::shutdown();
    Graphics::shutdown();

    delete window;

    spdlog::info("Miniengine finished successfully.");
}

} // namespace miniengine
