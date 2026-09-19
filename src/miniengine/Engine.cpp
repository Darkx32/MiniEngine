#include "Engine.h"
#include <spdlog/spdlog.h>

#include "bgfx/bgfx.h"
#include "bx/math.h"
#include "core/graphics.h"
#include "core/inputsystem.h"
#include "core/resourcemanager.h"
#include "core/scene.h"
#include "ecs/rendersystem.h"
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
    resourceManager = new ResourceManager();

    if (!Graphics::initializePrograms({.resourceManager = resourceManager, .nativeWindowHandle = window->getNativeWindowHandle(),
            .nativeDisplayType = window->getNativeDisplayType(),
            .width = static_cast<uint32_t>(window->getWidth()), .height = static_cast<uint32_t>(window->getHeight())}))
    {
        spdlog::error("Failed to initialize Graphics");
        return false;
    }

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

ResourceManager* Engine::getResourceManager() const
{
    return this->resourceManager;
}

void Engine::run() {
    spdlog::info("Starting main loop...");

    float view[16];
    float proj[16];
    bx::mtxOrtho(proj, 0.0f, static_cast<float>(window->width), static_cast<float>(window->height),
    0.0f, 0.0f, 100.0f, 0.0f, bgfx::getCaps()->homogeneousDepth);
    while (isRunning)
    {
        const uint64_t start_time = SDL_GetTicks();

        InputSystem::update();
        window->pollEvents();

        if (window->windowShouldClose() || InputSystem::isKeyPressed(InputSystem::KeyCode::Escape))
            isRunning = false;

        if (window->hasResized)
        {
            window->hasResized = false;
            bgfx::setViewRect(Graphics::DEFAULT, 0, 0, window->width, window->height);
            bx::mtxOrtho(proj, 0.0f, static_cast<float>(window->width), static_cast<float>(window->height),
            0.0f, 0.0f, 100.0f, 0.0f, bgfx::getCaps()->homogeneousDepth);
        }

        bx::mtxIdentity(view);
        bgfx::setViewTransform(Graphics::DEFAULT, view, proj);
        bgfx::setViewClear(Graphics::DEFAULT, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, Graphics::color, 1.0f, 0);
        bgfx::touch(Graphics::DEFAULT);

        if (scene)
            RenderSystem::render(scene->registry);

        bgfx::frame();

        const uint64_t frame_duration = SDL_GetTicks() - start_time;
        if (const int FRAME_DELAY = 1000 / fps; frame_duration < FRAME_DELAY)
        {
            SDL_Delay(FRAME_DELAY - frame_duration);
        }
    }
}

void Engine::shutdown() const
{
    delete resourceManager;
    Graphics::shutdown();

    delete window;

    spdlog::info("Miniengine finished successfully.");
}

} // namespace miniengine
