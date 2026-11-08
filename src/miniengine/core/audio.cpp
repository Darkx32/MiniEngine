//
// Created by matheus on 06/11/2026.
//

#include "../../../include/miniengine/core/audio.h"

namespace MiniEngine
{
    bool AudioManager::initialize()
    {
        if (ma_engine_init(nullptr, &engine) != MA_SUCCESS)
            return false;

        isInitialized = true;

        createGroup("master");
        return true;
    }

    void AudioManager::shutdown()
    {
        if (isInitialized)
            ma_engine_uninit(&engine);

        for (auto& group : groups | std::views::values)
            ma_sound_group_uninit(&group);

        groups.clear();
        isInitialized = false;
    }

    void AudioManager::setMasterVolume(const float volume)
    {
        ma_engine_set_volume(&engine, volume);
    }

    void AudioManager::createGroup(const std::string& groupName)
    {
        if (groups.contains(groupName) || !isInitialized)
            return;

        auto& group = groups[groupName];

        if (ma_sound_group_init(&engine, 0, nullptr, &group) != MA_SUCCESS)
        {
            groups.erase(groupName);
        }
    }

    void AudioManager::deleteGroup(const std::string& groupName)
    {
        if (!groups.contains(groupName) || !isInitialized) return;

        ma_sound_group_uninit(&groups[groupName]);
        groups.erase(groupName);
    }

    ma_sound AudioManager::generateSound(const std::string& filepath, const std::string& groupName)
    {
        ma_sound sound;
        ma_sound_init_from_file(&engine, filepath.c_str(), 0, &groups[groupName], nullptr, &sound);

        return sound;
    }

    void AudioManager::deleteSound(ma_sound* sound)
    {
        ma_sound_uninit(sound);
    }

    ma_engine AudioManager::engine = {};
    bool AudioManager::isInitialized = false;
    std::unordered_map<std::string, ma_sound_group> AudioManager::groups = {};
} // MiniEngine
