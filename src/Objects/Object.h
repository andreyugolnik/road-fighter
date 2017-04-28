#pragma once

#define CONSTITUTION_NONE 1
#define CONSTITUTION_SOLID 2
#define CONSTITUTION_CAR 4
#define CONSTITUTION_PLAYER 8
#define CONSTITUTION_FUEL 16
#define CONSTITUTION_WATER 32
#define CONSTITUTION_OIL 64

#include "List.h"

#include <SDL/SDL.h>

class CTile;

class CObject
{
    friend class CGame;

public:
    CObject();
    CObject(int x, int y, CTile* t, int constitution, class CGame* game);
    virtual ~CObject();

    virtual bool cycle(unsigned char* keyboard, unsigned char* old_keyboard);
    virtual void draw(int sx, int sy, SDL_Surface* screen);
    bool collision(int offsx, int offst, CObject* o);

    long get_x() const
    {
        return x;
    }

    long get_y() const
    {
        return y;
    }

    int get_dx() const;
    int get_dy() const;

    void set_state(int s)
    {
        state = s;
    }

    int get_state() const
    {
        return state;
    }

    bool constitution_test(int cons) const;

protected:
    int state; /*	0 - Stopped
					1 - ...	*/
    int constitution;

    CGame* game;
    CTile** tiles;
    int ntiles, tile;

    long x, y;
    long draw_x, draw_y;
};
