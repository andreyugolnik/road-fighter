#pragma once

#include "CarObject.h"

/* Trucks */
class CEnemyTruckObject : public CCarObject
{
    friend class CGame;

public:
    CEnemyTruckObject(int x, int y, CTile* t, int start_delay, class CGame* game);
    virtual ~CEnemyTruckObject();

    virtual bool cycle(unsigned char* keyboard, unsigned char* old_keyboard);

protected:
    int state_timmer;
    bool following_right_border;
    int distance_to_border;

    bool advanced;
};
