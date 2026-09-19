#include "Engine.h"
#include "core/graphics.h"
#include "core/scene.h"
#include "ecs/entity.h"
#include "ecs/renderer.h"

int main() {
    MiniEngine::Graphics::setClearColor(0x40afffFF);
    MiniEngine::Engine engine;

    if (!engine.init("MiniEngine", {1280, 720})) {
        return 1;
    }

    MiniEngine::Scene scene;
    auto entity = scene.createEntity();
    auto& resourceManager = engine.getResourceManager();
    entity.addComponent<MiniEngine::MeshRenderer>(resourceManager.get<MiniEngine::QuadPrimitive>(MiniEngine::Graphics::QuadPrimitiveID));

    auto& transform = entity.getComponent<MiniEngine::Transform>();
    transform.position = {20, 20, 1.0};
    transform.scale = {100, 100, 1};

    engine.setScene(&scene);

    engine.run();

    return 0;
}
