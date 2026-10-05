#include "Engine.h"
#include "core/graphics.h"
#include "core/scene.h"
#include "ecs/entity.h"
#include "ecs/physics.h"
#include "ecs/renderer.h"

namespace
{
    class SunScript : public MiniEngine::IScript
    {
    public:
        void startup() override
        {
            materialID = resourceManager->create<MiniEngine::Material>();
            auto* material = resourceManager->get<MiniEngine::Material>(materialID);
            material->color = {1.0f, 1.0f, 0.0f, 0.0f};

            auto& transform = entity->getComponent<MiniEngine::Transform>();

            transform.scale.x = transform.scale.y = 50;

            entity->addComponent<MiniEngine::MeshRenderer>(MiniEngine::Graphics::CirclePrimitiveID, materialID);
            auto& rb = entity->addComponent<MiniEngine::RigidBody>(transform);

            auto collider = rb.getCollider();
            collider.type = MiniEngine::ShapeType::Circle;
            rb.setCollider(collider, transform);
            rb.setGravityScale(0.0f);
        }

        void update(float dt) override
        {
        }

    private:
        uint16_t materialID = 0xFFFF;
    };
}

int main()
{
    MiniEngine::Graphics::setClearColor(0x000000FF);
    MiniEngine::Engine engine;

    if (!engine.init("MiniEngine", {1280, 720}))
    {
        return 1;
    }

    MiniEngine::Scene scene;
    auto camera = scene.createEntity();
    camera.addComponent<MiniEngine::Camera2D>();

    auto sun = scene.createEntity();
    sun.addScript(std::make_unique<SunScript>());

    engine.setScene(&scene);

    engine.run();

    return 0;
}
