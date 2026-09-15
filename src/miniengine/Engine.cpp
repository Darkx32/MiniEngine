#include "Engine.h"
#include <spdlog/spdlog.h>

#include "bgfx/bgfx.h"
#include "core/renderstartup.h"
#include "SDL3/SDL_timer.h"

namespace MiniEngine {

Engine::Engine() : fps(60), isRunning(false), window(nullptr) {}

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

    if (!RenderStartup::initializeRender(window->getNativeWindowHandle(), window->getNativeDisplayType(),
            static_cast<uint32_t>(window->getWidth()), static_cast<uint32_t>(window->getHeight())))
    {
        spdlog::error("Failed to initialize render");
        return false;
    }

    spdlog::info("Miniengine started successfully.");
    isRunning = true;
    return isRunning;
}

void Engine::setFps(const int newFps)
{
    this->fps = newFps;
}

void Engine::run() {
    spdlog::info("Starting main loop...");

    while (isRunning)
    {
        const uint64_t start_time = SDL_GetTicks();

        InputSystem::update();
        window->pollEvents();

        if (!window->windowShouldClose() || InputSystem::isKeyPressed(InputSystem::KeyCode::Escape))
            isRunning = false;

        if (window->hasResized)
        {
            window->hasResized = false;
            bgfx::setViewRect(0, 0, 0, window->width, window->height);
        }

        bgfx::touch(0);

        const uint64_t frame_duration = SDL_GetTicks() - start_time;
        if (const int FRAME_DELAY = 1000 / fps; frame_duration < FRAME_DELAY)
        {
            SDL_Delay(FRAME_DELAY - frame_duration);
        }
    }
}

void Engine::shutdown() {
    RenderStartup::shutdownRender();

    delete window;

    spdlog::info("Miniengine finished successfully.");
}

} // namespace miniengine
