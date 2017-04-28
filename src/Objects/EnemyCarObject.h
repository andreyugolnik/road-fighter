#pragma once

#include "CarObject.h"

class CEnemyCarObject : public CCarObject
{
    friend class CGame;

public:
    CEnemyCarObject(int x, int y, CTile* t, int start_delay, class CGame* game);
    virtual ~CEnemyCarObject();

    virtual bool cycle(unsigned char* keyboard, unsigned char* old_keyboard);

protected:
    int slide_direction;
    int slide_speed;
    int slide_timmer;

    int state_timmer;
    bool following_right_border;
    int distance_to_border;
};
