#include "EnemySlidderCarObject.h"
#include "Game.h"

CEnemySlidderCarObject::CEnemySlidderCarObject(int nx, int ny, CTile* t, int start_delay, CGame* g)
    : CEnemyCarObject(nx, ny, t, start_delay, g)
{
}

CEnemySlidderCarObject::~CEnemySlidderCarObject()
{
}

bool CEnemySlidderCarObject::cycle(unsigned char* keyboard, unsigned char* old_keyboard)
{
    bool retval = true;

    retval = CEnemyCarObject::cycle(keyboard, old_keyboard);

    if (state == 1 && game->min_distance_to_players(y) < 192)
    {
        CObject* closest_player = game->find_closest_player(x, y);
        if (x > closest_player->get_x())
        {
            if (following_right_border)
                distance_to_border += 40;
            else
                distance_to_border -= 40;
        }
        else
        {
            if (following_right_border)
                distance_to_border -= 40;
            else
                distance_to_border += 40;
        }
        if (distance_to_border < 0)
            distance_to_border += 80;
        state = 2;
    }

    return retval;
}
