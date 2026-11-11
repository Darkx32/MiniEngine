//
// Created by matheus on 13/09/2026.
//

#include "miniengine/ecs/entity.h"

#include "core/audio.h"

namespace MiniEngine
{
    AudioStreamPlayer::AudioStreamPlayer(const std::string& filepath)
    {
        this->filepath = filepath;
        streamID = AudioManager::generateSound(filepath, group);
    }

    void AudioStreamPlayer::play() const
    {
        auto* stream = ResourceManager::get().get<AudioStream>(streamID);
        if (stream->isInitialized)
            ma_sound_start(&stream->sound);
    }

    bool AudioStreamPlayer::isPlaying() const
    {
        const auto* stream = ResourceManager::get().get<AudioStream>(streamID);
        if (stream->isInitialized)
            return ma_sound_is_playing(&stream->sound);
        return false;
    }

    void AudioStreamPlayer::stop() const
    {
        auto* stream = ResourceManager::get().get<AudioStream>(streamID);
        if (stream->isInitialized)
            ma_sound_stop(&stream->sound);
    }

    Entity::Entity(const entt::entity entity, Scene* scene, entt::registry* registry)
    {
        m_entity = entity;
        p_scene = scene;
        p_registry = registry;
    }
} // MiniEngine
