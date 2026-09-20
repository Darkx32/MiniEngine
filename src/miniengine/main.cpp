#include "Engine.h"
#include "core/graphics.h"
#include "core/scene.h"
#include "ecs/entity.h"
#include "ecs/renderer.h"

class Player : public MiniEngine::IScript
{
public:
    void startup() override
    {
        auto& transform = entity->getComponent<MiniEngine::Transform>();
        transform.scale = {100, 100, 1};
    }
    void update(float dt) override
    {

    }
};

int main() {
    MiniEngine::Graphics::setClearColor(0x40afffFF);
    MiniEngine::Engine engine;

    if (!engine.init("MiniEngine", {1280, 720})) {
        return 1;
    }

    MiniEngine::Scene scene;
    auto entity = scene.createEntity();
    auto camera = scene.createEntity();
    auto& resourceManager = engine.getResourceManager();
    camera.addComponent<MiniEngine::Camera2D>();
    entity.addComponent<MiniEngine::MeshRenderer>(resourceManager.get<MiniEngine::QuadPrimitive>(MiniEngine::Graphics::QuadPrimitiveID));

    entity.addScript(std::make_unique<Player>());

    engine.setScene(&scene);

    engine.run();

    return 0;
}
