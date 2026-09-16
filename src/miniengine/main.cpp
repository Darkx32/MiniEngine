#include "Engine.h"
#include "core/graphics.h"

int main() {
    MiniEngine::Graphics::setClearColor(0x40afffFF);
    MiniEngine::Engine engine;
    if (!engine.init("MiniEngine", {1280, 720})) {
        return 1;
    }

    engine.run();

    return 0;
}
