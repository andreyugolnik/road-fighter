#pragma once

#include "EnemyCarObject.h"

/* Green cars */
class CEnemyNormalCarObject : public CEnemyCarObject
{
    friend class CGame;

public:
    CEnemyNormalCarObject(int x, int y, CTile* t, int start_delay, class CGame* game);
    virtual ~CEnemyNormalCarObject();
};
