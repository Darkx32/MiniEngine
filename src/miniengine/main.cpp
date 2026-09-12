#include "Engine.h"

int main() {
    MiniEngine::Engine engine;
    if (!engine.init("MiniEngine", {1280, 720})) {
        return 1;
    }

    engine.run();

    return 0;
}
