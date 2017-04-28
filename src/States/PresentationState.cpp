#include "../Auxiliar.h"
#include "../RoadFighter.h"

const int presentation_fade_time = 25;

int CRoadFighter::presentation_cycle(void)
{
    int i;
    bool key_pressed;

    if (state_timmer == 0)
    {
        presentation_state = 0;
        presentation_timmer = 0;
    }

    if (presentation_state == 0)
        presentation_timmer++;
    if (presentation_state == 1)
        presentation_timmer--;
    if (presentation_state == 2)
        presentation_timmer++;
    if (presentation_state == 3)
        presentation_timmer--;

    key_pressed = false;
    if (state_timmer >= 5)
    {
        for (i = 0; i < SDLK_LAST; i++)
        {
            if (keyboard[i] && !old_keyboard[i])
                key_pressed = true;
        }
    }

    if (presentation_state == 0 && (state_timmer >= 350 || key_pressed))
    {
        presentation_state = 1;
        if (presentation_timmer > presentation_fade_time)
            presentation_timmer = presentation_fade_time;
    }
    if (presentation_state == 2 && (state_timmer >= 700 || key_pressed))
    {
        presentation_state = 3;
        if (presentation_timmer > presentation_fade_time)
            presentation_timmer = presentation_fade_time;
    }

    if (presentation_state == 1 && presentation_timmer <= 0)
        presentation_state = 2;
    if (presentation_state == 3 && presentation_timmer <= 0)
        return KONAMI_STATE;

    return PRESENTATION_STATE;
}

void CRoadFighter::presentation_draw(SDL_Surface* screen)
{
    if (presentation_state < 2)
        SDL_BlitSurface(disclaimer_sfc, 0, screen, 0);
    else
        SDL_BlitSurface(retroremakes_sfc, 0, screen, 0);

    {
        float f = float(presentation_timmer) * (1.0F / float(presentation_fade_time));
        if (f < 0.0)
            f = 0.0;
        if (f < 1.0)
            surface_fader(screen, f, f, f, 0);
    }
}
