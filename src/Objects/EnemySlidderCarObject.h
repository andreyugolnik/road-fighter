#pragma once

#include "EnemyCarObject.h"

/* Pink cars */
class CEnemySlidderCarObject : public CEnemyCarObject
{
    friend class CGame;

public:
    CEnemySlidderCarObject(int x, int y, CTile* t, int start_delay, class CGame* game);
    virtual ~CEnemySlidderCarObject();

    virtual bool cycle(unsigned char* keyboard, unsigned char* old_keyboard);
};
