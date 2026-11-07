//
// Created by matheus on 06/11/2026.
//

#ifndef MINIENGINE_AUDIO_H
#define MINIENGINE_AUDIO_H

namespace MiniEngine
{
    class AudioSystem
    {
    public:
        static bool initialize();
        static void shutdown();

    private:
        static ma_engine engine;
        static bool isInitialized;
    };
} // MiniEngine

#endif //MINIENGINE_AUDIO_H
