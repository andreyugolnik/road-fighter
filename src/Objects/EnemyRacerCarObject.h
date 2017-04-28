#pragma once

#include "EnemyCarObject.h"

/* Blue cars */
class CEnemyRacerCarObject : public CEnemyCarObject
{
    friend class CGame;

public:
    CEnemyRacerCarObject(int x, int y, CTile* t, int start_delay, class CGame* game);
    virtual ~CEnemyRacerCarObject();

    virtual bool cycle(unsigned char* keyboard, unsigned char* old_keyboard);

protected:
    bool advanced;
};
