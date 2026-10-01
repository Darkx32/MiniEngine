//
// Created by matheus on 16/09/2026.
//

#ifndef MINIENGINE_RENDER_H
#define MINIENGINE_RENDER_H
#include "miniengine/core/resourcemanager.h"

namespace MiniEngine
{
    struct Transform;
    struct Vector2;

    struct MeshData
    {
        uint16_t vbh_idx = 0xFFFF;
        uint16_t ibh_idx = 0xFFFF;
        uint16_t program_idx = 0xFFFF;

        uint16_t ucolor_idx= 0xFFFF;
    };

    struct Material : IResource
    {
        float color[4];
        uint16_t texture;

        Material();
        explicit Material(const float* c);
        explicit Material(const char* filepath);
        ~Material() override;
    };

    struct QuadPrimitive : IResource, MeshData
    {
        QuadPrimitive();
        ~QuadPrimitive() override;
    };

    struct CirclePrimitive : IResource, MeshData
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
        explicit MeshRenderer(const MeshData* mesh, const Material* material) : meshData(mesh), material(material) {}
        const MeshData* meshData{nullptr};
        const Material* material{nullptr};
    };
}

#endif //MINIENGINE_RENDER_H
