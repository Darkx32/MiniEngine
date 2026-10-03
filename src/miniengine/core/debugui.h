//
// Created by matheus on 01/10/2026.
//

#ifndef MINIENGINE_DEBUGUI_H
#define MINIENGINE_DEBUGUI_H

struct SDL_Window;
union SDL_Event;

namespace MiniEngine
{
    class DebugUI
    {
    public:
        static void initialize(SDL_Window* window);
        static void update(const SDL_Event* event);
        static void render();
        static void shutdown();

    private:
        static double toMs(double frameTime, double frameFreq);
        static double toMB(double memory);

        static uint16_t idx_program;
        static uint16_t idx_sampler;
        static uint16_t idx_texture;
    };
} // MiniEngine

#endif //MINIENGINE_DEBUGUI_H
