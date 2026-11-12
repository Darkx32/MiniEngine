//
// Created by matheus on 13/09/2026.
//

#include "miniengine/ecs/entity.h"

#include "core/audio.h"

namespace MiniEngine
{
    AudioStreamPlayer::AudioStreamPlayer(const std::string& filepath)
    {
        streamID = AudioManager::generateSound(filepath);
    }

    AudioStreamPlayer::AudioStreamPlayer(const std::string& filepath, const std::string& groupName)
    {
        streamID = AudioManager::generateSound(filepath, groupName);
    }

    void AudioStreamPlayer::play() const
    {
        assert(streamID != INVALID_ID && "Stream is not valid");
        if (const auto* stream = ResourceManager::get().get<AudioStream>(streamID); stream->isInitialized)
            ma_sound_start(stream->sound.get());
    }

    bool AudioStreamPlayer::isPlaying() const
    {
        assert(streamID != INVALID_ID && "Stream is not valid");
        if (const auto* stream = ResourceManager::get().get<AudioStream>(streamID); stream->isInitialized)
            return ma_sound_is_playing(stream->sound.get());
        return false;
    }

    void AudioStreamPlayer::stop() const
    {
        assert(streamID != INVALID_ID && "Stream is not valid");
        if (const auto* stream = ResourceManager::get().get<AudioStream>(streamID); stream->isInitialized)
            ma_sound_stop(stream->sound.get());
    }

    Entity::Entity(const entt::entity entity, Scene* scene, entt::registry* registry)
    {
        m_entity = entity;
        p_scene = scene;
        p_registry = registry;
    }
} // MiniEngine
