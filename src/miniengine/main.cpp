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
            rb.setMass(40000);
        }

        void update(float dt) override
        {
        }

    private:
        uint16_t materialID = 0xFFFF;
    };

    class EarthScript : public MiniEngine::IScript
    {
    public:
        void startup() override
        {
            materialID = resourceManager->create<MiniEngine::Material>();
            auto* material = resourceManager->get<MiniEngine::Material>(materialID);
            material->color = {0.0f, 1.0f, 0.0f, 0.0f};

            auto& transform = entity->getComponent<MiniEngine::Transform>();
            transform.position = {-150.f, 0.0f, .0f};
            transform.scale.x = transform.scale.y = 5;

            entity->addComponent<MiniEngine::MeshRenderer>(MiniEngine::Graphics::CirclePrimitiveID, materialID);
            auto& rb = entity->addComponent<MiniEngine::RigidBody>(transform);

            auto collider = rb.getCollider();
            collider.type = MiniEngine::ShapeType::Circle;
            rb.setCollider(collider, transform);
            rb.setGravityScale(0.0f);
            rb.setMass(4);
            rb.setLinearVelocity({0.0f, 32.5});
        }

        void update(float dt) override
        {
        }

    private:
        uint16_t materialID = 0xFFFF;
    };

    class ControllerScript : public MiniEngine::IScript
    {
    public:
        void startup() override
        {
        }

        void update(float dt) override
        {
            auto entities = entity->getScene()->getAllEntitiesWithComponents<
                MiniEngine::Transform, MiniEngine::RigidBody>();

            for (size_t i = 0; i < entities.size(); ++i)
            {
                for (size_t j = i + 1; j < entities.size(); ++j)
                {
                    auto& entityA = entities[i];
                    auto& entityB = entities[j];

                    const auto& transformA = entityA.getComponent<MiniEngine::Transform>();
                    auto& rbA = entityA.getComponent<MiniEngine::RigidBody>();

                    const auto& transformB = entityB.getComponent<MiniEngine::Transform>();
                    auto& rbB = entityB.getComponent<MiniEngine::RigidBody>();

                    const float d = transformA.position.distanceTo(transformB.position);
                    const float massMultiplied = rbA.getMass() * rbB.getMass();
                    const float force = GRAVITATIONAL_CONSTANT * massMultiplied / (d * d);

                    MiniEngine::Vector3 direction = transformB.position - transformA.position;
                    direction.normalize();

                    rbA.addForce({direction.x * force, direction.y * force});
                    rbB.addForce({-direction.x * force, -direction.y * force});
                }
            }
        }

    private:
        const float GRAVITATIONAL_CONSTANT = 6.674f;
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

    auto earth = scene.createEntity();
    earth.addScript(std::make_unique<EarthScript>());

    auto controller = scene.createEntity();
    controller.addScript(std::make_unique<ControllerScript>());

    engine.setScene(&scene);

    engine.run();

    return 0;
}
