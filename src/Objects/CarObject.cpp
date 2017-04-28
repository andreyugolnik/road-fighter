#include "CarObject.h"
#include "../Tile.h"

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

CCarObject::CCarObject(void)
    : CObject()
    , last_collision(nullptr)
{
}

CCarObject::CCarObject(int nx, int ny, CTile* t, CGame* g)
    : CObject(nx, ny, t, CONSTITUTION_CAR, g)
    , last_collision(nullptr)
    , y_precision(0)
    , x_precision(0)
    , y_speed(0)
    , x_speed(0)
{
    state = 0;
}

CCarObject::~CCarObject()
{
}

bool CCarObject::cycle(unsigned char* /*keyboard*/, unsigned char* /*old_keyboard*/)
{
    y_precision += y_speed;
    while (y_precision > (1 << 8))
    {
        y++;
        y_precision -= 1 << 8;
    }

    while (y_precision < (-(1 << 8)))
    {
        y--;
        y_precision += 1 << 8;
    }

    x_precision += x_speed;
    while (x_precision > (1 << 8))
    {
        x++;
        x_precision -= 1 << 8;
    }

    while (x_precision < (-(1 << 8)))
    {
        x--;
        x_precision += 1 << 8;
    }

    return true;
}

void CCarObject::car_collision(CCarObject* car)
{
    last_collision = car;
}

long int CCarObject::compute_next_x()
{
    long int tmp = x_precision + x_speed;
    long int new_x = x;

    while (tmp > (1 << 8))
    {
        new_x++;
        tmp -= 1 << 8;
    }

    while (tmp < (-(1 << 8)))
    {
        new_x--;
        tmp += 1 << 8;
    }

    return new_x;
}

long int CCarObject::compute_next_y()
{
    long int tmp = y_precision + y_speed;
    long int new_y = y;

    while (tmp > (1 << 8))
    {
        new_y++;
        tmp -= 1 << 8;
    }

    while (tmp < (-(1 << 8)))
    {
        new_y--;
        tmp += 1 << 8;
    }

    return new_y;
}
