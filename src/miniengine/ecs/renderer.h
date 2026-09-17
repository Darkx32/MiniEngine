//
// Created by matheus on 16/09/2026.
//

#ifndef MINIENGINE_RENDER_H
#define MINIENGINE_RENDER_H
#include "miniengine/core/graphics.h"

namespace MiniEngine
{
    struct MeshRenderer
    {
        explicit MeshRenderer(const Mesh* mesh) : meshData(mesh) {}
        const Mesh* meshData{nullptr};
    };
}

#endif //MINIENGINE_RENDER_H
