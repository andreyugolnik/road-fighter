#include "EnemyRacerCarObject.h"
#include "Game.h"

CEnemyRacerCarObject::CEnemyRacerCarObject(int nx, int ny, CTile* t, int start_delay, CGame* g)
    : CEnemyCarObject(nx, ny, t, start_delay, g)
{
    advanced = false;
}

CEnemyRacerCarObject::~CEnemyRacerCarObject()
{
}

bool CEnemyRacerCarObject::cycle(unsigned char* keyboard, unsigned char* old_keyboard)
{
    bool retval = CEnemyCarObject::cycle(keyboard, old_keyboard);

    if (!advanced && game->min_distance_to_players(y) < 48 && y_speed != 0)
    {
        advanced = true;
        if (game->S_caradvance != 0)
        {
            Sound_play(game->S_caradvance);
        }
    }

    return retval;
}
