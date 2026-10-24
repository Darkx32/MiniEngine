//
// Created by matheus on 16/09/2026.
//

#ifndef MINIENGINE_RENDER_H
#define MINIENGINE_RENDER_H
#include <array>
#include <cstdint>
#include "miniengine/core/resourcemanager.h"

namespace MiniEngine
{
    struct Transform;
    struct Vector2;

    struct ShaderData : IResource
    {
        uint16_t program_idx = 0xFFFF;

        uint16_t ucolor_idx = 0xFFFF;

        uint16_t ulightPositions_idx = 0xFFFF;
        uint16_t ushadowSegments_idx = 0xFFFF;
        uint16_t ulightColors_idx = 0xFFFF;
        uint16_t ulightParams_idx = 0xFFFF;
        uint16_t ulightCount_idx = 0xFFFF;
        uint16_t ushadowCount_idx = 0xFFFF;
        uint16_t ushadowIgnore_idx = 0xFFFF;
        uint16_t uambientColor_idx = 0xFFFF;

        ShaderData();
        ~ShaderData() override;
    };

    struct MeshData : IResource
    {
        uint16_t vbh_idx = 0xFFFF;
        uint16_t ibh_idx = 0xFFFF;

        uint16_t shaderData = 0xFFFF;
    };

    struct Material : IResource
    {
        std::array<float, 4> color{1.0f, 1.0f, 1.0f, 1.0f};
        uint16_t texture;

        Material();
        explicit Material(std::array<float, 4> c);
        explicit Material(const char* filepath);
        ~Material() override;
    };

    struct QuadPrimitive : MeshData
    {
        QuadPrimitive();
        ~QuadPrimitive() override;
    };

    struct CirclePrimitive : MeshData
    {
        CirclePrimitive();
        ~CirclePrimitive() override;
    };

    struct Camera2D
    {
        float zoom = 1.0f;

        float view[16]{};
        float proj[16]{};

        void calculate(const Vector2& windowSize);
        void updateView(const Transform& transform);
    };

    struct MeshRenderer
    {
        explicit MeshRenderer(const uint16_t mesh, const uint16_t material) : meshData(mesh), material(material)
        {
        }

        const uint16_t meshData = 0xFFFF;
        const uint16_t material = 0xFFFF;
    };
}

#endif //MINIENGINE_RENDER_H
