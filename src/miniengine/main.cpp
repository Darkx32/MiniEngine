#include "Engine.h"
#include "core/graphics.h"
#include "core/scene.h"
#include "ecs/entity.h"
#include "ecs/physics.h"
#include "ecs/renderer.h"

namespace
{
    class Player : public MiniEngine::IScript
    {
    public:
        void startup() override
        {
            auto& transform = entity->getComponent<MiniEngine::Transform>();
            transform.scale = {100, 100, 0.0f};
            constexpr float color[] = {0.0f, 0.0f, 0.0f, 1.0f};
            const uint16_t materialId = resourceManager->create<MiniEngine::Material>(color);
            entity->addComponent<MiniEngine::MeshRenderer>(MiniEngine::Graphics::QuadPrimitiveID,
                    materialId);
            entity->addComponent<MiniEngine::RigidBody>(transform, MiniEngine::RigidBody::BodyType::Dynamic, MiniEngine::RigidBody::ShapeType::Box);
        }
        void update(float dt) override
        {

        }
    };

    class Ground : public MiniEngine::IScript
    {
    public:
        void startup() override
        {
            auto& transform = entity->getComponent<MiniEngine::Transform>();
            transform.position = {0, -300, 1};
            transform.scale = {400, 50, 1};
            transform.rotation.z = 20;
            const uint16_t materialId = resourceManager->create<MiniEngine::Material>();
            entity->addComponent<MiniEngine::MeshRenderer>(MiniEngine::Graphics::QuadPrimitiveID,
                    materialId);
            entity->addComponent<MiniEngine::RigidBody>(transform, MiniEngine::RigidBody::BodyType::Static);
        }
        void update(float dt) override
        {

        }
    };
}

int main() {
    MiniEngine::Graphics::setClearColor(0x40afffFF);
    MiniEngine::Engine engine;

    if (!engine.init("MiniEngine", {1280, 720})) {
        return 1;
    }

    MiniEngine::Scene scene;
    auto ground = scene.createEntity();
    ground.addScript(std::make_unique<Ground>());

    auto entity = scene.createEntity();
    auto camera = scene.createEntity();

    camera.addComponent<MiniEngine::Camera2D>();
    entity.addScript(std::make_unique<Player>());

    engine.setScene(&scene);

    engine.run();

    return 0;
}
