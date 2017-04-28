#pragma once

#include "Object.h"

class CExplosionObject : public CObject
{
public:
    CExplosionObject(int x, int y, List<CTile>* l, int first_tile, int last_tile, class CGame* game);
    virtual ~CExplosionObject();

    virtual bool cycle(unsigned char* keyboard, unsigned char* old_keyboard);

protected:
    int timmer;
};
