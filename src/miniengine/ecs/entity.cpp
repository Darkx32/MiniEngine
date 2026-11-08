//
// Created by matheus on 13/09/2026.
//

#include "miniengine/ecs/entity.h"

#include "core/audio.h"

namespace MiniEngine
{
    AudioStream::AudioStream(const std::string& filepath) : stream({})
    {
        this->filepath = filepath;
        stream = AudioManager::generateSound(filepath, group);
    }

    AudioStream::~AudioStream()
    {
        AudioManager::deleteSound(&stream);
    }

    void AudioStream::play()
    {
        ma_sound_start(&stream);
    }

    bool AudioStream::isPlaying() const
    {
        return ma_sound_is_playing(&stream);
    }

    void AudioStream::stop()
    {
        ma_sound_stop(&stream);
    }

    Entity::Entity(const entt::entity entity, Scene* scene, entt::registry* registry)
    {
        m_entity = entity;
        p_scene = scene;
        p_registry = registry;

        if (!hasComponent<Transform>())
            addComponent<Transform>();
        if (!hasComponent<ScriptComponent>())
            addComponent<ScriptComponent>();
    }
} // MiniEngine
