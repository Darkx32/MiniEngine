//
// Created by matheus on 06/11/2026.
//

#include "../../../include/miniengine/core/audio.h"

namespace MiniEngine
{
    bool AudioSystem::initialize()
    {
        if (ma_engine_init(nullptr, &engine) != MA_SUCCESS)
            return false;

        isInitialized = true;
        return true;
    }

    void AudioSystem::shutdown()
    {
        if (isInitialized)
            ma_engine_uninit(&engine);
    }

    ma_engine AudioSystem::engine = {};
    bool AudioSystem::isInitialized = false;
} // MiniEngine
