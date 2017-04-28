#include "../FileHandling.h"
#include "../RoadFighter.h"
#include "../Game.h"

#include <ctime>

bool record_replay = false;
bool load_replay = false;

extern int start_level;

int CRoadFighter::playing_cycle(void)
{
    if (state_timmer == 0)
    {
        if (record_replay)
        {
            unsigned int seed;
            replay_fp = f1open("replay.txt", "w+", USERDATA);
            seed = (unsigned int)time(NULL);
            fprintf(replay_fp, "%u\n", seed);
            srand(seed);
        }
        else
        {
            if (load_replay)
            {
                unsigned int seed;
                replay_fp = f1open("replay.txt", "r+", USERDATA);
                fscanf(replay_fp, "%u", &seed);
                srand(seed);
            }
        }
    }

    if (record_replay)
    {
        int i;
        for (i = 0; i < SDLK_LAST; i++)
        {
            if (keyboard[i] && !old_keyboard[i])
                fprintf(replay_fp, "1 %i\n", i);
            if (!keyboard[i] && old_keyboard[i])
                fprintf(replay_fp, "0 %i\n", i);
        }
        fprintf(replay_fp, "-1\n");
    }
    else
    {
        if (load_replay)
        {
            int v, v2;
            do
            {
                if (1 != fscanf(replay_fp, "%i", &v))
                {
                    fclose(replay_fp);
                    replay_fp = 0;
                    return GAMEOVER_STATE;
                }
                if (v != -1)
                {
                    if (1 != fscanf(replay_fp, "%i", &v2))
                    {
                        fclose(replay_fp);
                        replay_fp = 0;
                        return GAMEOVER_STATE;
                    }
                    if (v == 1)
                        keyboard[v2] = true;
                    if (v == 0)
                        keyboard[v2] = false;
                }
            } while (v != -1);
        }
    }

    if (!game->cycle(keyboard, old_keyboard))
    {
        if (game->level_completed())
        {
            current_level++;

            if (record_replay || load_replay)
            {
                fclose(replay_fp);
                replay_fp = 0;
            }
            return INTERLEVEL_STATE;
        }
        else
        {
            if (game->backspace_pressed)
            {
                if (record_replay || load_replay)
                {
                    fclose(replay_fp);
                    replay_fp = 0;
                }
                delete game;
                game = 0;
                current_level = start_level;
                return INTERLEVEL_STATE;
            }
            else
            {
                gameover_state = 0;
                if (record_replay || load_replay)
                {
                    fclose(replay_fp);
                    replay_fp = 0;
                }
                return GAMEOVER_STATE;
            }
        }
    }
    else
    {
        if (!playing_reachedend && game->level_completed())
        {
            playing_reachedend = true;
            Sound_create_music("sound/levelcomplete", 0);
        }
    }

    if (!playing_reachedend)
    {
        int volume = (game->get_game_timmer() * MIX_MAX_VOLUME) / 25;
        if (volume < 0)
            volume = 0;
        if (volume > MIX_MAX_VOLUME)
            volume = MIX_MAX_VOLUME;
        Sound_music_volume(volume);
    }

    /* Scores: */
    {
        List<int> l;
        int* itmp;
        game->get_scores(&l);
        l.Rewind();
        while (l.Iterate(itmp))
            if ((*itmp) > high_score)
                high_score = *itmp;
    }

    return PLAYING_STATE;
}

void CRoadFighter::playing_draw(SDL_Surface* screen)
{
    SDL_Rect r;

    SDL_FillRect(screen, 0, 0);

    scoreboard_draw(scoreboard_x, 0, screen);

    /* Draw game: */
    if (n_players == 1)
    {
        r.x = (desired_scoreboard_x - 352);
        r.y = 0;
        r.w = 352;
        r.h = screen->h;
    }
    else
    {
        r.x = 0;
        r.y = 0;
        r.w = scoreboard_x;
        r.h = screen->h;
    }
    game->draw(screen, r);
}
