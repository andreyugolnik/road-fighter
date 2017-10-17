#include "RoadFighter.h"
#include "Auxiliar.h"
#include "Debug.h"
#include "Game.h"
#include "Objects/Object.h"
#include "Sound.h"
#include "Tile.h"

#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <cstdio>
#include <cstdlib>

#if defined(EMSCRIPTEN)
#include <emscripten.h>
#define SDL_GetKeyState SDL_GetKeyboardState
#define SDL_DisplayFormat SDL_DisplayFormatAlpha
#endif

extern int MAX_SPEED;
extern int MAX_FUEL;
extern int SCREEN_X;
extern int SCREEN_Y;

CRoadFighter::CRoadFighter()
{
    output_debug_message("CRoadFighter: in constructor...\n");

    state = PRESENTATION_STATE;
    state_timmer = 0;
    current_level = 0;
    high_score = 0;
    game = nullptr;

    presentation_state = 0;
    presentation_timmer = 0;
    konami_state = 0;
    konami_timmer = 0;
    interlevel_state = 0;
    interlevel_timmer = 0;
    gameover_state = 0;
    gameover_timmer = 0;
    endsequence_state = 0;
    endsequence_timmer = 0;
    scoreboard_fade_timmer = 0;

    menu_tittle_text = 0;
    menu_options_text = 0;
    menu_redefining_key = -1;

    load_configuration();

    game_mode = 0;

    scoreboard_x = -1;
    desired_scoreboard_x = SCREEN_X;

    for (int i = 0; i < SDLK_LAST; i++)
    {
        old_keyboard[i] = 0;
    }

    font1 = loadFont("fonts/comicbd.ttf", 16);
    font2big = loadFont("fonts/tanglewo.ttf", FONT_SIZE);
    font2medium = loadFont("fonts/tanglewo.ttf", int(FONT_SIZE * 0.8));
    font2small = loadFont("fonts/tanglewo.ttf", int(FONT_SIZE * 0.65));

    disclaimer_sfc = loadImage("graphics/disclaimer.jpg");
    retroremakes_sfc = loadImage("graphics/retroremakes.bmp");
    konami1_sfc = loadImage("graphics/konami1.jpg");
    konami2_sfc = loadImage("graphics/konami2.jpg");
    tittle_sfc = loadImage("graphics/title.jpg");
    arrow_sfc = loadImage("graphics/arrow.bmp");
    gamemap_sfc = loadImage("graphics/gamemap.bmp");
    minicar1_sfc = loadImage("graphics/minicar1.bmp");
    minicar2_sfc = loadImage("graphics/minicar2.bmp");
    gameover_sfc = loadImage("graphics/gameover.jpg");
    scoreboard_sfc = loadImage("graphics/scoreboard.bmp");
    scoreboard2_sfc = nullptr;
    scoreboardleft_sfc = loadImage("graphics/scoreboard_left.bmp");

    {
        SDL_Color c;
        const char* text = "        "
                           "Game created for the RETROREMAKES REMAKE COMPETITION"
                           "        "
                           "PROGRAMMING: Santi Ontañón (Brain)    "
                           "GRAPHICS: Miikka Poikela (McBain)    "
                           "MUSIC/SFX: Jorrith Schaap (Jorito)    "
                           "BETA TESTING: Jason Eames (JEames), Miikka Poikela (McBain), Jorrith Schaap (Jorito), Santi Ontañón (Brain)"
                           "        ";
        c.r = c.g = c.b = 128;

        credits_sfc = TTF_RenderText_Blended(font1, text, c);
        credits2_sfc = TTF_RenderText_Blended(font1, text, c);
    }

    levelintro_sfc = 0;
    minicar1_tile = new CTile(0, 0, minicar1_sfc->w / 2, minicar1_sfc->h, minicar1_sfc, false);
    minicar2_tile = new CTile(0, 0, minicar2_sfc->w / 2, minicar2_sfc->h, minicar2_sfc, false);

    {
        SDL_Surface* sfc;

        sfc = SDL_DisplayFormat(konami1_sfc);
        SDL_FreeSurface(konami1_sfc);
        konami1_sfc = sfc;

        sfc = SDL_DisplayFormat(konami2_sfc);
        SDL_FreeSurface(konami2_sfc);
        konami2_sfc = sfc;

        sfc = SDL_DisplayFormat(tittle_sfc);
        SDL_FreeSurface(tittle_sfc);
        tittle_sfc = sfc;

        /* ... */
    }

    S_menu_move = Sound_create_sound("sound/menu_move");
    S_menu_select = Sound_create_sound("sound/menu_select");
    S_menu_in = Sound_create_sound("sound/logo_in");
    S_menu_out = Sound_create_sound("sound/logo_out");

    replay_fp = 0;

    output_debug_message("CRoadFighter: constructor done.\n");
}

CRoadFighter::~CRoadFighter()
{
    output_debug_message("CRoadFighter: in destructor...\n");

    SDL_FreeSurface(retroremakes_sfc);
    SDL_FreeSurface(disclaimer_sfc);
    SDL_FreeSurface(konami1_sfc);
    SDL_FreeSurface(konami2_sfc);
    SDL_FreeSurface(tittle_sfc);
    SDL_FreeSurface(arrow_sfc);
    SDL_FreeSurface(scoreboard_sfc);
    SDL_FreeSurface(scoreboard2_sfc);
    SDL_FreeSurface(scoreboardleft_sfc);
    SDL_FreeSurface(gamemap_sfc);
    SDL_FreeSurface(minicar1_sfc);
    SDL_FreeSurface(minicar2_sfc);
    SDL_FreeSurface(gameover_sfc);
    SDL_FreeSurface(credits_sfc);
    SDL_FreeSurface(credits2_sfc);
    if (levelintro_sfc != 0)
        SDL_FreeSurface(levelintro_sfc);

    delete minicar1_tile;
    delete minicar2_tile;

    TTF_CloseFont(font1);
    TTF_CloseFont(font2big);
    TTF_CloseFont(font2medium);
    TTF_CloseFont(font2small);

    delete game;

    Sound_delete_sound(S_menu_move);
    Sound_delete_sound(S_menu_select);
    Sound_delete_sound(S_menu_in);
    Sound_delete_sound(S_menu_out);

    if (replay_fp != 0)
        fclose(replay_fp);

    output_debug_message("CRoadFighter: destructor done.\n");
}

bool CRoadFighter::cycle(void)
{
    int i, old_state = state;
    bool retval = true;
    keyboard = (unsigned char*)SDL_GetKeyState(nullptr);

    switch (state)
    {
    case PRESENTATION_STATE:
        state = presentation_cycle();
        break;
    case KONAMI_STATE:
        state = konami_cycle();
        break;
    case MENU_STATE:
        state = menu_cycle();
        break;
    case DEMO_STATE:
        state = KONAMI_STATE;
        break;
    case PLAYING_STATE:
        state = playing_cycle();
        break;
    case INTERLEVEL_STATE:
        state = interlevel_cycle();
        break;
    case GAMEOVER_STATE:
        state = gameover_cycle();
        break;
    case QUIT_STATE:
        retval = false;
        break;
    }

    if (state != old_state)
        state_timmer = 0;
    else
        state_timmer++;

    for (i = 0; i < SDLK_LAST; i++)
        old_keyboard[i] = keyboard[i];

    return retval;
}

void CRoadFighter::draw(SDL_Surface* screen)
{
    if (state_timmer == 0)
        return;

    switch (state)
    {
    case PRESENTATION_STATE:
        presentation_draw(screen);
        break;
    case KONAMI_STATE:
        konami_draw(screen);
        break;
    case MENU_STATE:
        menu_draw(screen);
        break;
    case DEMO_STATE:
        break;
    case PLAYING_STATE:
        playing_draw(screen);
        break;
    case INTERLEVEL_STATE:
        interlevel_draw(screen);
        break;
    case GAMEOVER_STATE:
        gameover_draw(screen);
        break;
    }
}

void CRoadFighter::scoreboard_draw(int x, int y, SDL_Surface* screen)
{
    if (scoreboard_sfc == nullptr)
    {
        return;
    }

    /* Draw Scoreboard: */
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = scoreboard_sfc->w;
    r.h = scoreboard_sfc->h;
    SDL_BlitSurface(scoreboard_sfc, 0, screen, &r);
    if (r.x + r.w < screen->w)
    {
        SDL_Rect r2;

        r2.x = scoreboard_sfc->w - 1;
        r2.y = 0;
        r2.w = 1;
        r2.h = scoreboard_sfc->h;
        for (int i = r.x + r.w; i < screen->w; i++)
        {
            r.x = i;
            r.w = 1;
            SDL_BlitSurface(scoreboard_sfc, &r2, screen, &r);
        }
    }

    /* Draw the Speed: */
    if (game != nullptr)
    {
        List<int> l;
        int* speed;

        game->get_speeds(&l);
        l.Rewind();
        int j = l.Length() - 1;
        while (l.Iterate(speed))
        {
            int tmp = int(112 * (float(-*(speed)) / MAX_SPEED));
            int tmp2 = (33 - l.Length()) / l.Length();

            for (int i = 367; i > 367 - tmp; i -= 2)
            {
                r.x = (x + 28) + (tmp2 + 1) * j;
                r.y = i;
                r.w = tmp2;
                r.h = 1;
                SDL_FillRect(screen, &r, SDL_MapRGB(screen->format, 255, 255, 255));
            }
            j--;
        }
    }

    /* Draw the fuel: */
    if (game != nullptr)
    {
        List<int> l;
        int* fuel;

        game->get_fuels(&l);
        l.Rewind();
        int j = l.Length() - 1;
        while (l.Iterate(fuel))
        {
            int tmp = int(112 * (float(*fuel) / MAX_FUEL));
            int tmp2 = (33 - l.Length()) / l.Length();

            for (int i = 367; i > 367 - tmp; i -= 2)
            {
                r.x = (x + 76) + (tmp2 + 1) * j;
                r.y = i;
                r.w = tmp2;
                r.h = 1;
                SDL_FillRect(screen, &r, SDL_MapRGB(screen->format, 255, 255, 255));
            }
            j--;
        }
    }

    /* Draw the mini-map: */
    {
        SDL_Rect r;
        float f;

        r.x = scoreboard_x + 41;
        r.y = 97;
        r.w = 55;
        r.h = 143;
        SDL_BlitSurface(scoreboard2_sfc, 0, screen, &r);
        f = scoreboard_fade_timmer / 25.0F;
        if (f < 0)
            f = 0;
        if (f > 1.0)
            f = 1.0;

        if (f != 1.0)
            surface_fader(screen, f, f, f, &r);
    }

    /* Draw the mini-car: */
    if (game != nullptr && ((state_timmer >> 3) & 0x01) != 0)
    {
        List<float> l;
        float* pos;

        game->get_positions(&l);
        l.Rewind();

        int car_x = x + 68 + l.Length() * minicar1_tile->get_dx() / 2 - minicar1_tile->get_dx();
        int j = 0;
        while (l.Iterate(pos))
        {
            int car_y = int(108 + *pos * (121 - minicar1_tile->get_dy()));
            if (j == 0)
                minicar1_tile->draw(car_x, car_y, screen);
            if (j == 1)
                minicar2_tile->draw(car_x, car_y, screen);
            car_x -= minicar1_tile->get_dx();
            j++;
        }
    }

    /* Draw the score: */
    if (game != nullptr)
    {
        /* High-score: */
        List<int> l;
        int* itmp;

        char tmp[80];
        sprintf(tmp, "%i", high_score);

        SDL_Color c;
        c.r = c.b = 0;
        c.g = 255;
        SDL_Surface* sfc = TTF_RenderText_Blended(font1, tmp, c);
        r.x = x + 103 - sfc->w;
        r.y = 21;
        r.w = sfc->w;
        r.h = sfc->h;
        SDL_BlitSurface(sfc, 0, screen, &r);
        SDL_FreeSurface(sfc);

        /* Scores: */
        game->get_scores(&l);
        l.Rewind();
        int j = 0;
        int score_x = 103;
        while (l.Iterate(itmp))
        {
            if (((state_timmer >> 5) % l.Length()) == j)
            {
                sprintf(tmp, "%i", *itmp);
                c.r = c.b = c.g = 0;
                if (j == 0)
                    c.r = 255;
                if (j == 1)
                {
                    c.r = 255;
                    c.g = 255;
                }
                sfc = TTF_RenderText_Blended(font1, tmp, c);
                r.x = x + score_x - sfc->w;
                r.y = 64;
                r.w = sfc->w;
                r.h = sfc->h;
                SDL_BlitSurface(sfc, 0, screen, &r);
                SDL_FreeSurface(sfc);
            }
            j++;
        }
    }

    /* Draw the left scoreboard: */
    if (n_players == 1)
    {
        float f;
        SDL_Rect r;

        f = float(screen->w - scoreboard_x) / float(screen->w - desired_scoreboard_x);
        r.x = (-scoreboardleft_sfc->w) + int((desired_scoreboard_x - 352) * f);
        r.y = 0;
        r.w = scoreboardleft_sfc->w;
        r.h = scoreboardleft_sfc->h;

        SDL_BlitSurface(scoreboardleft_sfc, 0, screen, &r);
    }
}
