#include "Auxiliar.h"
#include "RoadFighter.h"

const int konami_fade_time = 25;

int CRoadFighter::konami_cycle()
{
    if (state_timmer == 0)
    {
        konami_state = 0;
        konami_timmer = 0;
    }

    if (konami_state == 0)
        konami_timmer++;
    if (konami_state == 1)
        konami_timmer--;

    if (konami_state == 0 && (state_timmer >= 350 || (keyboard[fire_key] && !old_keyboard[fire_key]) || (keyboard[SDLK_ESCAPE] && !old_keyboard[SDLK_ESCAPE])))
    {
        konami_state = 1;
        if (konami_timmer > konami_fade_time)
            konami_timmer = konami_fade_time;
    }

    if (konami_state == 1 && konami_timmer <= 0)
        return MENU_STATE;

    return KONAMI_STATE;
}

void CRoadFighter::konami_draw(SDL_Surface* screen)
{
    SDL_Rect r;

    r.x = 0;
    r.y = 0;
    r.w = konami1_sfc->w;
    r.h = konami1_sfc->h;

    SDL_BlitSurface(konami1_sfc, &r, screen, &r);

    r.x = 0;
    r.y = 0;
    r.w = konami2_sfc->w;
    r.h = state_timmer * 2;

    SDL_BlitSurface(konami2_sfc, &r, screen, &r);

    {
        float f = float(konami_timmer) * (1.0F / float(konami_fade_time));
        if (f < 0.0)
            f = 0.0;
        if (f < 1.0)
            surface_fader(screen, f, f, f, 0);
    }
}
