#pragma once

#include "Object.h"

class CSemaphoreObject : public CObject
{
public:
    CSemaphoreObject(int x, int y, CTile* t1, CTile* t2, CTile* t3, CTile* t4, CTile* t5, class CGame* game);
    virtual ~CSemaphoreObject();

    virtual bool cycle(unsigned char* keyboard, unsigned char* old_keyboard);

protected:
    int timmer;
};
