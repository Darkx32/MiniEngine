//
// Created by matheus on 06/11/2026.
//

#include "../../../include/miniengine/core/audio.h"

namespace MiniEngine
{
    AudioStream::AudioStream(const ma_sound& s, const bool init)
    {
        sound = s;
        isInitialized = init;
    }

    AudioStream::~AudioStream()
    {
        if (isInitialized)
            AudioManager::deleteSound(&sound);
    }

    bool AudioManager::initialize()
    {
        if (const ma_result result = ma_engine_init(nullptr, &engine); result != MA_SUCCESS)
        {
            spdlog::error("Engine initialization failed: {} ({})", ma_result_description(result),
                          static_cast<int>(result));
            return false;
        }

        isInitialized = true;

        createGroup("master");
        return true;
    }

    void AudioManager::shutdown()
    {
        if (isInitialized)
            ma_engine_uninit(&engine);

        for (auto& group : groups | std::views::values)
            ma_sound_group_uninit(group.get());

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

        auto group = std::make_unique<ma_sound_group>();

        const ma_result result = ma_sound_group_init(
            &engine,
            0,
            nullptr,
            group.get()
        );

        if (result != MA_SUCCESS)
        {
            spdlog::error(
                "Failed to initialize audio group '{}': {}",
                groupName,
                ma_result_description(result)
            );
            return;
        }

        groups.emplace(groupName, std::move(group));
    }

    void AudioManager::deleteGroup(const std::string& groupName)
    {
        if (!groups.contains(groupName) || !isInitialized) return;

        ma_sound_group_uninit(groups[groupName].get());
        groups.erase(groupName);
    }

    uint16_t AudioManager::generateSound(const std::string& filepath, const std::string& groupName)
    {
        ma_sound sound;
        const ma_result result = ma_sound_init_from_file(&engine, filepath.c_str(), 0, groups[groupName].get(),
                                                         nullptr, &sound);
        if (result != MA_SUCCESS)
        {
            if (result == MA_INVALID_FILE)
                spdlog::warn("File not found '{}'", filepath);
            spdlog::error("Error to create sound {}", ma_result_description(result));
        }

        return ResourceManager::get().create<AudioStream>(sound, result == MA_SUCCESS);
    }

    void AudioManager::deleteSound(ma_sound* sound)
    {
        ma_sound_uninit(sound);
    }

    ma_engine AudioManager::engine = {};
    bool AudioManager::isInitialized = false;
    std::unordered_map<std::string, std::unique_ptr<ma_sound_group>> AudioManager::groups = {};
} // MiniEngine
