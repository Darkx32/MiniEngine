//
// Created by matheus on 13/09/2026.
//

#include "miniengine/ecs/entity.h"

#include "core/audio.h"

namespace MiniEngine
{
    AudioStreamPlayer::AudioStreamPlayer(const std::string& filepath)
    {
        streamHandle = AudioManager::generateSound(filepath);
    }

    AudioStreamPlayer::AudioStreamPlayer(const std::string& filepath, const std::string& groupName)
    {
        streamHandle = AudioManager::generateSound(filepath, groupName);
    }

    void AudioStreamPlayer::play() const
    {
        ResourceManager::assertValid(streamHandle);
        if (const auto* stream = ResourceManager::get().get<AudioStream>(streamHandle); stream->isInitialized)
            ma_sound_start(stream->sound.get());
    }

    bool AudioStreamPlayer::isPlaying() const
    {
        ResourceManager::assertValid(streamHandle);
        if (const auto* stream = ResourceManager::get().get<AudioStream>(streamHandle); stream->isInitialized)
            return ma_sound_is_playing(stream->sound.get());
        return false;
    }

    void AudioStreamPlayer::stop() const
    {
        ResourceManager::assertValid(streamHandle);
        if (const auto* stream = ResourceManager::get().get<AudioStream>(streamHandle); stream->isInitialized)
            ma_sound_stop(stream->sound.get());
    }

    Entity::Entity(const entt::entity entity, Scene* scene, entt::registry* registry)
    {
        m_entity = entity;
        p_scene = scene;
        p_registry = registry;
    }
} // MiniEngine
