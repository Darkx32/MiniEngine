#pragma once
#include <string>

#include "core/inputsystem.h"
#include "core/window.h"

namespace MiniEngine {
class Engine {
public:
    Engine();
    ~Engine();

    bool init(const std::string& title, Vector2 size);
    void run();
    void shutdown();

private:
    Window* window;
    InputSystem* inputSystem;
};

} // namespace miniengine
