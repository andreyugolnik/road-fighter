#pragma once

#include "EnemyCarObject.h"

/* Fast Blue cars */
class CEnemyFastCarObject : public CEnemyCarObject
{
    friend class CGame;

public:
    CEnemyFastCarObject(int x, int y, CTile* t, int start_delay, class CGame* game);
    virtual ~CEnemyFastCarObject();

    virtual bool cycle(unsigned char* keyboard, unsigned char* old_keyboard);
};
