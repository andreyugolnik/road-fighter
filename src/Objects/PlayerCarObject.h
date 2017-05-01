#pragma once

#include "CarObject.h"

#include <SDL_mixer.h>

class CPlayerCarObject : public CCarObject
{
public:
    CPlayerCarObject(int x, int y, List<CTile>* l, int first_tile, int last_tile,
                     int lk, int rk, int fk, int sc, int init_delay, class CGame* game);
    virtual ~CPlayerCarObject();

    virtual bool cycle(unsigned char* keyboard, unsigned char* old_keyboard);
    virtual void draw(int sx, int sy, SDL_Surface* screen);

    int get_fuel() const
    {
        return fuel;
    };
    int get_score() const
    {
        return score;
    };

    void reach_goal();

protected:
    int car_tile(int angle); /* in degrees */
    void tyre_coordinates(int angle, int* x1, int* y1, int* x2, int* y2);

    // int state;		0 - Starting
    // 1 - Driving
    // 2 - Bouncing right
    // 3 - Bouncing left
    // 4 - Exploding
    // 5 - Sliding	right
    // 6 - Slidding left */

protected:
    int sound_timmer;
    int state_timmer;
    int rotating_angle;
    int blinking_time;
    int old_angle;

    int enginesound_channel, skidsound_channel;
    Mix_Chunk S_carengine_working;
    Mix_Chunk S_carskid_working;

    int score;
    int bonus, next_bonus, bonus_timmer;
    int fuel;
    int left_key, right_key, fire_key;

    bool goal_reached;
};
