//
// Created by matheus on 07/11/2026.
//

#include "../../../include/miniengine/systems/audiosystem.h"

#include "ecs/entity.h"

namespace MiniEngine
{
    void AudioSystem::prepare(entt::registry* registry)
    {
        registry->view<AudioStreamPlayer>().each(
            [](const AudioStreamPlayer& audioStream)
            {
                if (audioStream.autoPlay)
                    audioStream.play();
            });
    }
} // MiniEngine
