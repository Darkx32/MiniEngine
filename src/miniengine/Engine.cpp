#include "Engine.h"
#include <spdlog/spdlog.h>

#include "bgfx/bgfx.h"
#include "core/renderstartup.h"
#include "SDL3/SDL_timer.h"

namespace MiniEngine {

Engine::Engine() : fps(60), window(nullptr), inputSystem(nullptr) {}

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

    inputSystem = new InputSystem();
    if (!inputSystem)
    {
        spdlog::error("Failed to start InputSystem");
        return false;
    }
    inputSystem->eventWatchWindow(&window->windowSharedData);

    spdlog::info("Miniengine started successfully.");
    return true;
}

void Engine::setFps(const int newFps)
{
    this->fps = newFps;
}

void Engine::run() {
    spdlog::info("Starting main loop...");

    while (window->windowShouldClose())
    {
        const uint64_t start_time = SDL_GetTicks();

        inputSystem->update();

        if (inputSystem->isKeyPressed(InputSystem::KeyCode::Escape))
            window->isRunning = false;

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

    delete inputSystem;
    delete window;

    spdlog::info("Miniengine finished successfully.");
}

} // namespace miniengine
