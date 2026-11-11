//
// Created by matheus on 06/11/2026.
//

#ifndef MINIENGINE_AUDIO_H
#define MINIENGINE_AUDIO_H
#include "resourcemanager.h"

namespace MiniEngine
{
    struct AudioStream : IResource
    {
        explicit AudioStream(const ma_sound& s, bool init);

        ~AudioStream() override;

        ma_sound sound{};
        bool isInitialized;
    };

    class AudioManager
    {
    public:
        static bool initialize();
        static void shutdown();

        static void setMasterVolume(float volume);
        static void createGroup(const std::string& groupName);
        static void deleteGroup(const std::string& groupName);

        static uint16_t generateSound(const std::string& filepath, const std::string& groupName = "master");
        static void deleteSound(ma_sound* sound);

    private:
        static std::unordered_map<std::string, std::unique_ptr<ma_sound_group>> groups;

        static ma_engine engine;
        static bool isInitialized;
    };
} // MiniEngine

#endif //MINIENGINE_AUDIO_H
