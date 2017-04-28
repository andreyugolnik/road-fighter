#pragma once

#include "Object.h"

class CFuelObject : public CObject
{
public:
    CFuelObject(int x, int y, CTile* t, class CGame* game);
    virtual ~CFuelObject();

    virtual bool cycle(unsigned char* keyboard, unsigned char* old_keyboard);

protected:
    int y_precision, x_precision;
    int y_speed, x_speed;
};
