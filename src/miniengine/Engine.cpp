#include "Engine.h"
#include <spdlog/spdlog.h>

#include "bgfx/bgfx.h"
#include "core/renderstartup.h"
#include "SDL3/SDL_timer.h"

namespace MiniEngine {

Engine::Engine() : window(nullptr), inputSystem(nullptr) {}

Engine::~Engine() {
    shutdown();
}

bool Engine::init(const std::string& title, const Vector2 size) {
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

void Engine::run() {
    spdlog::info("Starting main loop...");

    while (window->windowShouldClose())
    {
        inputSystem->update();

        if (inputSystem->isKeyPressed(InputSystem::KeyCode::Escape))
            window->isRunning = false;

        if (window->hasResized)
        {
            window->hasResized = false;
            bgfx::setViewRect(0, 0, 0, window->width, window->height);
        }

        bgfx::touch(0);
    }
}

void Engine::shutdown() {
    RenderStartup::shutdownRender();

    delete inputSystem;
    delete window;

    spdlog::info("Miniengine finished successfully.");
}

} // namespace miniengine
