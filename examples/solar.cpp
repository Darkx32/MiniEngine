#include "miniengine/engine.h"
#include "miniengine/core/graphics.h"
#include "miniengine/core/scene.h"
#include "miniengine/ecs/entity.h"
#include "miniengine/ecs/lights.h"
#include "miniengine/ecs/physics.h"
#include "miniengine/ecs/renderer.h"

namespace
{
    class SunScript : public MiniEngine::IScript
    {
    public:
        void startup() override
        {
            const auto entity = getEntity();

            materialID = resourceManager->create<MiniEngine::Material>();
            auto* material = resourceManager->get<MiniEngine::Material>(materialID);
            material->color = {1.0f, 1.0f, 0.0f, 0.0f};

            auto& transform = entity.getComponent<MiniEngine::Transform>();
            transform.scale.x = transform.scale.y = 50;

            entity.addComponent<MiniEngine::MeshRenderer>(MiniEngine::Graphics::getCirclePrimitiveID(),
                                                          materialID);
            auto& _ = entity.addComponent<MiniEngine::PointLight2D>();
            auto& rb = entity.addComponent<MiniEngine::RigidBody>(transform);

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
            const auto entity = getEntity();

            materialID = resourceManager->create<MiniEngine::Material>();
            auto* material = resourceManager->get<MiniEngine::Material>(materialID);
            material->color = {0.0f, 1.0f, 0.0f, 0.0f};

            auto& transform = entity.getComponent<MiniEngine::Transform>();
            transform.position = {-150.f, 0.0f, .0f};
            transform.scale.x = transform.scale.y = 5;

            entity.addComponent<MiniEngine::MeshRenderer>(MiniEngine::Graphics::getCirclePrimitiveID(),
                                                          materialID);
            auto& shadow = entity.addComponent<MiniEngine::Shadow2D>();
            shadow.shape = MiniEngine::ShadowShape2D::Circle;
            auto& rb = entity.addComponent<MiniEngine::RigidBody>(transform);

            auto collider = rb.getCollider();
            collider.type = MiniEngine::ShapeType::Circle;
            rb.setCollider(collider, transform);
            rb.setGravityScale(0.0f);
            rb.setMass(4);
            rb.setLinearVelocity({0.0f, 42.5f});
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
            const auto entity = getEntity();

            const auto& player = entity.addComponent<MiniEngine::AudioStreamPlayer>("music.mp3");

            player.play();
        }

        void update(float dt) override
        {
            const auto entity = getEntity();

            const auto entities = entity.getScene()->getAllEntitiesWithComponents<
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

    class SolarScene : public MiniEngine::Scene
    {
    public:
        using Scene::Scene;
        ~SolarScene() override = default;

        void initialize() override
        {
            Scene::initialize();

            const auto camera = createEntity();
            auto& _ = camera.addComponent<MiniEngine::Camera2D>();

            const auto sun = createEntity();
            sun.addScript(std::make_unique<SunScript>());

            const auto earth = createEntity();
            earth.addScript(std::make_unique<EarthScript>());

            const auto controller = createEntity();
            controller.addScript(std::make_unique<ControllerScript>());

            const auto obstacle1 = createEntity();
            auto& resourceManager = MiniEngine::ResourceManager::get();
            auto materialID = resourceManager.create<MiniEngine::Material>();
            obstacle1.addComponent<MiniEngine::MeshRenderer>(MiniEngine::Graphics::getQuadPrimitiveID(), materialID);

            auto& transform1 = obstacle1.getComponent<MiniEngine::Transform>();
            transform1.position = {0.0f, 200, .0f};
            transform1.scale = {200.0f, 50.0f, .0f};

            const auto obstacle2 = createEntity();
            obstacle2.addComponent<MiniEngine::MeshRenderer>(MiniEngine::Graphics::getQuadPrimitiveID(), materialID);

            auto& transform2 = obstacle2.getComponent<MiniEngine::Transform>();
            transform2.position = {0.0f, -200, .0f};
            transform2.scale = {200.0f, 50.0f, .0f};
        }
    };
}

int main()
{
    MiniEngine::Graphics::setClearColor(0x000000FF);
    MiniEngine::engine engine;

    if (!engine.init("MiniEngine", {1280, 720}))
        return 1;

    engine.setScene<SolarScene>();
    engine.run();

    return 0;
}
