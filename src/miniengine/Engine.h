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
    void setFps(int newFps);
    void run();
    void shutdown();

private:
    int fps;
    bool isRunning;

    Window* window;
};

} // namespace miniengine
