#pragma once

#include "Object.h"

class CCarObject : public CObject
{
public:
    CCarObject();
    CCarObject(int x, int y, CTile* t, class CGame* game);
    virtual ~CCarObject();

    virtual bool cycle(unsigned char* keyboard, unsigned char* old_keyboard);

    int get_y_speed() const
    {
        return y_speed;
    };

    void car_collision(CCarObject* car);

    long int compute_next_x();
    long int compute_next_y();

protected:
    CCarObject* last_collision;

    long int y_precision, x_precision;
    int y_speed, x_speed;
};
