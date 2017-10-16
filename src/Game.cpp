#include "Game.h"
#include "Auxiliar.h"
#include "Objects/EnemyFastCarObject.h"
#include "Objects/EnemyNormalCarObject.h"
#include "Objects/EnemyRacerCarObject.h"
#include "Objects/EnemySlidderCarObject.h"
#include "Objects/EnemyTruckObject.h"
#include "Objects/FuelObject.h"
#include "Objects/PlayerCarObject.h"
#include "Sound.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>

#include <SDL_image.h>
#include <SDL_mixer.h>

extern int SCREEN_Y;
extern int MAX_SPEED;

int PLAYING_WINDOW = 384 + 64;
int CAR_APPEARING_OFFSET = 384 + 32;

// Player:
int MIN_SPEED = 0;
int MAX_SPEED = (24 << 8);
int MAX_ACCEL_RATE = -48;
int BRAKE_RATE = 16;
int BRAKE_RATE_NO_FUEL = 64;
int MAX_HSPEED = 768;
int BOUNCE_HSPEED = 880;
int MAX_FUEL = 2048;
int FUEL_RECHARGE = 384;
int FUEL_LOSS = 256;

// Enemy:
int ENEMY_SPEED = (13 << 8);
int ENEMY_HSPEED = 416;
int CAR_INTERVAL = 38;

// Obstacles:
int obstacle_chance[4][6] = { { -1, -1, 25, 20, 18, 15 },
                              { 50, 25, 20, 18, 15, 12 },
                              { 50, 25, 20, 18, 15, 12 },
                              { 50, 25, 20, 18, 15, 12 } };

int obstacle_oil[4][6] = { { 50, 50, 50, 50, 50, 50 },
                           { 50, 50, 48, 47, 45, 44 },
                           { 48, 47, 45, 43, 42, 40 },
                           { 48, 47, 45, 43, 42, 40 } };

int obstacle_water[4][6] = { { 50, 50, 50, 50, 50, 50 },
                             { 50, 50, 47, 46, 45, 44 },
                             { 47, 46, 45, 42, 41, 40 },
                             { 47, 46, 45, 42, 41, 40 } };

int obstacle_rock[4][6] = { { 0, 0, 0, 0, 0, 0 },
                            { 0, 0, 5, 7, 10, 12 },
                            { 5, 7, 10, 15, 17, 20 },
                            { 5, 7, 10, 15, 17, 20 } };

// Fast cars:
int fastcar_chance[4][6] = { { -1, -1, -1, -1, -1, -1 },
                             { -1, -1, 25, 23, 20, 18 },
                             { 40, 37, 35, 33, 30, 25 },
                             { 40, 37, 35, 33, 30, 25 } };

const int fade_time = 25;
const int default_start_delay = 50;

void CGame::init_game(const char* mapname)
{
    game_timmer = 0;
    game_state = 0;
    start_delay = default_start_delay;

    if (!load_map(mapname))
    {
        assert(!"can't load map");
    }

    init_quick_tables();

    font = loadFont("fonts/tanglewo.ttf", 16);

    player1_car = loadImage("graphics/car1.bmp");
    player2_car = loadImage("graphics/car2.bmp");

    player_tiles.Add(new CTile(0, 0, 32, 32, player1_car, true));
    player_tiles.Add(new CTile(0, 32, 32, 32, player1_car, true));
    player_tiles.Add(new CTile(0, 64, 32, 32, player1_car, true));
    player_tiles.Add(new CTile(0, 96, 32, 32, player1_car, true));
    player_tiles.Add(new CTile(0, 128, 32, 32, player1_car, true));
    player_tiles.Add(new CTile(0, 160, 32, 32, player1_car, true));
    player_tiles.Add(new CTile(0, 192, 32, 32, player1_car, true));
    player_tiles.Add(new CTile(0, 224, 32, 32, player1_car, true));
    player_tiles.Add(new CTile(0, 256, 32, 32, player1_car, true));

    player_tiles.Add(new CTile(0, 0, 32, 32, player2_car, true));
    player_tiles.Add(new CTile(0, 32, 32, 32, player2_car, true));
    player_tiles.Add(new CTile(0, 64, 32, 32, player2_car, true));
    player_tiles.Add(new CTile(0, 96, 32, 32, player2_car, true));
    player_tiles.Add(new CTile(0, 128, 32, 32, player2_car, true));
    player_tiles.Add(new CTile(0, 160, 32, 32, player2_car, true));
    player_tiles.Add(new CTile(0, 192, 32, 32, player2_car, true));
    player_tiles.Add(new CTile(0, 224, 32, 32, player2_car, true));
    player_tiles.Add(new CTile(0, 256, 32, 32, player2_car, true));

    enemy_cars = loadImage("graphics/enemycars.bmp");

    enemy_tiles.Add(new CTile(0, 0, 32, 32, enemy_cars, true));
    enemy_tiles.Add(new CTile(0, 32, 32, 32, enemy_cars, true));
    enemy_tiles.Add(new CTile(0, 64, 32, 32, enemy_cars, true));
    enemy_tiles.Add(new CTile(0, 96, 32, 64, enemy_cars, true));

    fuel_sfc = loadImage("graphics/fuel.bmp");

    if (start_delay != default_start_delay)
    { /* There is a SEMAPHORE */
        objects.Add(new CEnemyRacerCarObject((dx / 2) - 30, dy - 176, enemy_tiles[0], start_delay, this));
        objects.Add(new CEnemyRacerCarObject((dx / 2) + 14, dy - 176, enemy_tiles[0], start_delay, this));
        objects.Add(new CEnemyRacerCarObject((dx / 2) - 30, dy - 224, enemy_tiles[0], start_delay, this));
        objects.Add(new CEnemyRacerCarObject((dx / 2) + 14, dy - 224, enemy_tiles[0], start_delay, this));
        objects.Add(new CEnemyRacerCarObject((dx / 2) - 30, dy - 272, enemy_tiles[0], start_delay, this));
        objects.Add(new CEnemyRacerCarObject((dx / 2) + 14, dy - 272, enemy_tiles[0], start_delay, this));
        objects.Add(new CEnemyRacerCarObject((dx / 2) - 30, dy - 320, enemy_tiles[0], start_delay, this));
        objects.Add(new CEnemyRacerCarObject((dx / 2) + 14, dy - 320, enemy_tiles[0], start_delay, this));
        objects.Add(new CEnemyRacerCarObject((dx / 2) - 30, dy - 368, enemy_tiles[0], start_delay, this));
        objects.Add(new CEnemyRacerCarObject((dx / 2) + 14, dy - 368, enemy_tiles[0], start_delay, this));
    }

    empty_sfc = loadImage("graphics/empty.bmp");
    fuelscores_sfc = loadImage("graphics/fuel_scores.bmp");
    //	start_sfc=loadImage("graphics/start.bmp");
    checkpoint_sfc = loadImage("graphics/checkpoint.bmp");
    goal_sfc = loadImage("graphics/goal.bmp");
    obstacles_sfc = loadImage("graphics/obstacles.bmp");
    pause_sfc = loadImage("graphics/pause.bmp");
    explosion_sfc = loadImage("graphics/explosion.bmp");

    extra_tiles.Add(new CTile(0, 0, 32, 32, fuel_sfc, true)); /* 0 */

    extra_tiles.Add(new CTile(0, 0, empty_sfc->w / 2, empty_sfc->h, empty_sfc, false)); /* 1 */
    extra_tiles.Add(new CTile(0, 0, fuelscores_sfc->w / 2, fuelscores_sfc->h / 4, fuelscores_sfc, false));
    extra_tiles.Add(new CTile(0, fuelscores_sfc->h / 4, fuelscores_sfc->w / 2, fuelscores_sfc->h / 4, fuelscores_sfc, false));
    extra_tiles.Add(new CTile(0, fuelscores_sfc->h / 2, fuelscores_sfc->w / 2, fuelscores_sfc->h / 4, fuelscores_sfc, false));
    extra_tiles.Add(new CTile(0, 3 * (fuelscores_sfc->h / 4), fuelscores_sfc->w / 2, fuelscores_sfc->h / 4, fuelscores_sfc, false));

    extra_tiles.Add(new CTile(0, 0, obstacles_sfc->w / 3, obstacles_sfc->h / 3, obstacles_sfc, true)); /* 6 */
    extra_tiles.Add(new CTile(0, (obstacles_sfc->h / 3), obstacles_sfc->w / 3, obstacles_sfc->h / 3, obstacles_sfc, true));
    extra_tiles.Add(new CTile(0, 2 * (obstacles_sfc->h / 3), obstacles_sfc->w / 3, obstacles_sfc->h / 3, obstacles_sfc, true));

    extra_tiles.Add(new CTile(0, 0, pause_sfc->w / 2, pause_sfc->h, pause_sfc, false)); /* 9 */

    extra_tiles.Add(new CTile(0, 0, checkpoint_sfc->w / 2, checkpoint_sfc->h, checkpoint_sfc, false)); /* 10 */
    extra_tiles.Add(new CTile(0, 0, goal_sfc->w / 2, goal_sfc->h, goal_sfc, false)); /* 11 */

    explosion_tiles.Add(new CTile(0, 0, 64, 64, explosion_sfc, false));
    explosion_tiles.Add(new CTile(0, 64, 64, 64, explosion_sfc, false));
    explosion_tiles.Add(new CTile(0, 128, 64, 64, explosion_sfc, false));
    explosion_tiles.Add(new CTile(0, 192, 64, 64, explosion_sfc, false));
    explosion_tiles.Add(new CTile(0, 256, 64, 64, explosion_sfc, false));
    explosion_tiles.Add(new CTile(0, 320, 64, 64, explosion_sfc, false));
    explosion_tiles.Add(new CTile(0, 384, 64, 64, explosion_sfc, false));
    explosion_tiles.Add(new CTile(0, 448, 64, 64, explosion_sfc, false));
    explosion_tiles.Add(new CTile(0, 512, 64, 64, explosion_sfc, false));
    explosion_tiles.Add(new CTile(0, 576, 64, 64, explosion_sfc, false));
    explosion_tiles.Add(new CTile(0, 640, 64, 64, explosion_sfc, false));
    explosion_tiles.Add(new CTile(0, 704, 64, 64, explosion_sfc, false));

    MAX_FUEL = 2500;
    FUEL_RECHARGE = 400;
    FUEL_LOSS = 225;
    CAR_INTERVAL = 38;

    if (game_mode == 1)
    {
        MAX_FUEL = 1800;
        FUEL_RECHARGE = 300;
        FUEL_LOSS = 150;
        CAR_INTERVAL = 24;
    }
    if (game_mode == 2)
    {
        MAX_FUEL = 1300;
        FUEL_RECHARGE = 275;
        FUEL_LOSS = 75;
        CAR_INTERVAL = 16;
    }
    if (game_mode == 3)
    {
        MAX_FUEL = 1250;
        FUEL_RECHARGE = 200;
        FUEL_LOSS = 100;
        CAR_INTERVAL = 12;
    }

    S_takefuel = Sound_create_sound("sound/takefuel");
    S_redlight = Sound_create_sound("sound/redlight");
    S_greenlight = Sound_create_sound("sound/greenlight");
    S_crash = Sound_create_sound("sound/car_crash");
    S_carstart = Sound_create_sound("sound/car_start");
    S_fuelempty = Sound_create_sound("sound/fuelempty");
    S_caradvance = Sound_create_sound("sound/car_pass");
    S_carengine = Sound_create_sound("sound/car_running");
    S_carskid = Sound_create_sound("sound/car_brake");
    S_water = Sound_create_sound("sound/water");
    S_collision = Sound_create_sound("sound/collision");
    S_truck = Sound_create_sound("sound/truck");

    fastcar_counter = 0;
    esc_pressed = false;
    backspace_pressed = false;
    paused = false;
}

CGame::CGame(const char* mapname, int mode, int left_key, int right_key, int fire_key, int score, int cl, bool extras)
{
    game_mode = mode;

    init_game(mapname);
    current_level = cl;

    game_remake_extras = extras;

    if (start_delay != default_start_delay)
    { /* There is a SEMAPHORE */
        objects.Add(new CEnemyRacerCarObject((dx / 2) + 14, dy - 128, enemy_tiles[0], start_delay, this));
    }

    CObject* o = new CPlayerCarObject((dx / 2) - 30, dy - 128, &player_tiles, 0, 8,
                                      left_key, right_key, fire_key, score, start_delay + 8, this);
    objects.Add(o);
    focusing_objects.Add(o);
    checkpoint_delay.Add(new int(-1));
    focusing_fy.Add(new float(0.66F));
    focusing_next_car.Add(new int(CAR_INTERVAL));
    focusing_next_car_index.Add(new int(0));
}

CGame::CGame(const char* mapname, int mode, int left_key1, int right_key1, int fire_key1,
             int left_key2, int right_key2, int fire_key2,
             int score1, int score2, int cl, bool extras)
{
    game_mode = mode;

    init_game(mapname);
    current_level = cl;

    game_remake_extras = extras;

    CObject* o = new CPlayerCarObject((dx / 2) - 30, dy - 128, &player_tiles, 0, 8,
                                      left_key1, right_key1, fire_key1, score1, start_delay + 8, this);
    objects.Add(o);
    focusing_objects.Add(o);
    checkpoint_delay.Add(new int(-1));
    focusing_fy.Add(new float(0.66F));
    focusing_next_car.Add(new int(CAR_INTERVAL));
    focusing_next_car_index.Add(new int(0));

    o = new CPlayerCarObject((dx / 2) + 14, dy - 128, &player_tiles, 9, 17,
                             left_key2, right_key2, fire_key2, score2, start_delay + 8, this);
    objects.Add(o);
    focusing_objects.Add(o);
    checkpoint_delay.Add(new int(-1));
    focusing_fy.Add(new float(0.66F));
    focusing_next_car.Add(new int(CAR_INTERVAL));
    focusing_next_car_index.Add(new int(0));
}

CGame::~CGame()
{
    delete_quick_tables();

    TTF_CloseFont(font);

    SDL_FreeSurface(player1_car);
    SDL_FreeSurface(player2_car);

    SDL_FreeSurface(empty_sfc);
    SDL_FreeSurface(fuelscores_sfc);
    //	SDL_FreeSurface(start_sfc);
    SDL_FreeSurface(checkpoint_sfc);
    SDL_FreeSurface(goal_sfc);
    SDL_FreeSurface(obstacles_sfc);
    SDL_FreeSurface(pause_sfc);

    while (!focusing_objects.EmptyP())
        focusing_objects.ExtractIni();

    while (Mix_Playing(-1) != 0)
        ;

    Sound_delete_sound(S_takefuel);
    Sound_delete_sound(S_redlight);
    Sound_delete_sound(S_greenlight);
    Sound_delete_sound(S_crash);
    Sound_delete_sound(S_carstart);
    Sound_delete_sound(S_fuelempty);
    Sound_delete_sound(S_caradvance);
    Sound_delete_sound(S_carengine);
    Sound_delete_sound(S_carskid);
    Sound_delete_sound(S_water);
    Sound_delete_sound(S_collision);
    Sound_delete_sound(S_truck);
}

void CGame::get_speeds(List<int>* lr) const
{
    List<CObject> l;
    CObject* o;

    l.Instance(focusing_objects);
    l.Rewind();
    while (l.Iterate(o))
    {
        auto co = (CCarObject*)o;
        lr->Add(new int(co->get_y_speed()));
    }
}

void CGame::get_fuels(List<int>* lr) const
{
    List<CObject> l;
    CObject* o;

    l.Instance(focusing_objects);
    l.Rewind();
    while (l.Iterate(o))
    {
        auto co = (CPlayerCarObject*)o;
        lr->Add(new int(co->get_fuel()));
    }
}

void CGame::get_positions(List<float>* lr) const
{
    List<CObject> l;
    CObject* o;

    l.Instance(focusing_objects);
    l.Rewind();
    while (l.Iterate(o))
    {
        float f;
        auto co = (CCarObject*)o;
        f = float(co->get_y()) / float(dy - 48);
        if (f < 0.0)
            f = 0.0;
        if (f > 1.0)
            f = 1.0;
        lr->Add(new float(f));
    }
}

void CGame::get_scores(List<int>* lr) const
{
    List<CObject> l;
    CObject* o;

    l.Instance(focusing_objects);
    l.Rewind();
    while (l.Iterate(o))
    {
        auto co = (CPlayerCarObject*)o;
        lr->Add(new int(co->get_score()));
    }
}

bool CGame::level_completed() const
{
    List<CObject> l;
    CObject* o;
    bool completed = false;

    /* Test is level completed: */
    l.Instance(focusing_objects);
    l.Rewind();
    while (l.Iterate(o))
    {
        auto cpo = (CPlayerCarObject*)o;
        if (o->get_y() >= -32 && cpo->get_fuel() > 0)
            return false;
        if (o->get_y() < -32)
            completed = true;
    }

    return completed;
}

bool CGame::cycle(unsigned char* keyboard, unsigned char* old_keyboard)
{
    List<CObject> l;
    CObject* o;

    if (keyboard[SDLK_F1] && !old_keyboard[SDLK_F1])
    {
        if (paused)
            paused = false;
        else
            paused = true;
    }

    if (paused)
        return true;

    if (start_delay > 0)
    {
        start_delay--;
        start_delay2 = 25;
        if (start_delay == 0)
        {
            if (S_greenlight != 0)
                Sound_play(S_greenlight);
            start_delay2 = 25;
        }
    }
    else
    {
        if (start_delay2 > 0)
        {
            start_delay2--;
            if (start_delay2 == 0)
            {
                if ((current_level % 2) == 1)
                    Sound_create_music("sound/game_theme", -1);
                else
                    Sound_create_music("sound/game_theme2", -1);
            }
        }
    }

    if (game_state == 0 && game_timmer == 0)
    {
        if (S_carstart != 0)
            Sound_play(S_carstart);
    }

    if (keyboard[SDLK_ESCAPE] && !old_keyboard[SDLK_ESCAPE] && game_state == 0)
    {
        game_state = 1;
        game_timmer = fade_time;
        esc_pressed = true;
    }

    if (keyboard[SDLK_BACKSPACE] && !old_keyboard[SDLK_BACKSPACE] && game_state == 0)
    {
        game_state = 1;
        game_timmer = fade_time;
        backspace_pressed = true;
    }

    l.Instance(background);
    l.Rewind();
    while (l.Iterate(o))
        o->cycle(keyboard, old_keyboard);

    l.Instance(middleground);
    l.Rewind();
    while (l.Iterate(o))
        o->cycle(keyboard, old_keyboard);

    l.Instance(foreground);
    l.Rewind();
    while (l.Iterate(o))
        o->cycle(keyboard, old_keyboard);

    l.Instance(objects);
    l.Rewind();
    while (l.Iterate(o))
    {
        //		if (start_delay==0 && o->get_state()==0) o->set_state(1);
        if (!o->cycle(keyboard, old_keyboard))
            todelete.Add(o);
    }

    while (!todelete.EmptyP())
    {
        o = todelete.ExtractIni();
        objects.DeleteElement(o);
        delete o;
        o = 0;
    }

    if (game_state == 0)
        game_timmer++;
    if (game_state == 1)
        game_timmer--;

    /* Add cars to the race: */
    {
        /* Very simple rule: add a car 512 pixels before each racer (if this new car is		*/
        /* not very close to another racer): */
        List<CObject> l;
        List<int> l2, l3;
        int *ip, *ip2;
        CObject* o;
        CCarObject* co;
        int new_car_y = -1;

        l.Instance(focusing_objects);
        l.Rewind();
        l2.Instance(focusing_next_car);
        l2.Rewind();
        l3.Instance(focusing_next_car_index);
        l3.Rewind();
        while (l.Iterate(o) && l2.Iterate(ip) && l3.Iterate(ip2))
        {
            co = (CCarObject*)o;
            if ((-co->get_y_speed()) > 0.8F * MAX_SPEED)
                *ip = (*ip) - 1;

            if ((*ip) <= 0)
            {
                new_car_y = o->get_y() - CAR_APPEARING_OFFSET;
                if ((first_player(o) || min_distance_to_other_players(new_car_y, o) > PLAYING_WINDOW) && min_distance_to_car(new_car_y) > 64)
                {
                    /* Change this in function of the level: */
                    int* pattern;
                    int pattern1[] = { 0, 1, 0, 0, 0, 4,
                                       0, 1, 0, 0, 0, 0, 0, 2, 0, 1, 0, 1, 0, 4,
                                       0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 4, -1, 6 };
                    int pattern2[] = { 0, 1, 0, 2, 0, 0, 0, 4,
                                       0, 0, 2, 2, 0, 2, 0, 0, 0, 2, 0, 4,
                                       2, 2, 2, 0, 0, 0, 1, 0, 1, 0, 0, 0, 4, -1, 8 };
                    /*	gpgbppg tggWggbtggpgg gpgbtggObgpgbppg ... */
                    int pattern3[] = { 0, 2, 0, 1, 2, 2, 0, 4,
                                       3, 0, 0, 0, 0, 1, 3, 0, 0, 2, 0, 0, 4,
                                       0, 2, 0, 1, 3, 0, 0, 1, 0, 2, 0, 1, 2, 2, 0, 4, -1, 8 };
                    /*	gbtggpg tggbtggptgg tgggtggpgggbtggpg ... */
                    int pattern4[] = { 0, 1, 3, 0, 0, 2, 0, 4,
                                       3, 0, 0, 1, 3, 0, 0, 2, 3, 0, 0, 4,
                                       3, 0, 0, 0, 3, 0, 0, 2, 0, 0, 0, 1, 3, 0, 0, 2, 0, 4, -1, 8 };
                    /* tggbppppg pgpgbpptggbtgpptggbppppg ... */
                    int pattern5[] = { 3, 0, 0, 1, 2, 2, 2, 2, 0, 4,
                                       2, 0, 2, 0, 1, 2, 2, 3, 0, 0, 1, 3, 0, 2, 2, 3, 0, 0, 1, 2, 2, 2, 2, 0, 4, -1, 10 };
                    /* gggpppgpppg tggpppgbtgpptggggggpppgpppg ... */
                    int pattern6[] = { 0, 0, 0, 2, 2, 2, 0, 2, 2, 2, 0, 4,
                                       3, 0, 0, 2, 2, 2, 0, 1, 3, 0, 2, 2, 3, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 2, 2, 2, 0, -1, 10 };

                    pattern = pattern1;
                    if (current_level == 2)
                        pattern = pattern2;
                    if (current_level == 3)
                        pattern = pattern3;
                    if (current_level == 4)
                        pattern = pattern4;
                    if (current_level == 5)
                        pattern = pattern5;
                    if (current_level == 6)
                        pattern = pattern6;
                    if (pattern[*ip2] == -1)
                        *ip2 = pattern[(*ip2) + 1];

                    if (pattern[*ip2] == 0)
                        add_enemy_car(0, new_car_y);
                    if (pattern[*ip2] == 1)
                        add_enemy_car(1, new_car_y);
                    if (pattern[*ip2] == 2)
                        add_enemy_car(2, new_car_y);
                    if (pattern[*ip2] == 3)
                        add_enemy_car(3, new_car_y);
                    if (pattern[*ip2] == 4)
                        add_enemy_car(4, new_car_y);

                    if (obstacle_chance[game_mode][current_level - 1] != -1 && (rand() % obstacle_chance[game_mode][current_level - 1]) == 0)
                    {
                        int i = rand() % 100;
                        if (i < obstacle_oil[game_mode][current_level - 1])
                        {
                            add_obstacle(0, new_car_y);
                        }
                        else
                        {
                            i -= obstacle_oil[game_mode][current_level - 1];
                            if (i < obstacle_water[game_mode][current_level - 1])
                            {
                                add_obstacle(1, new_car_y);
                            }
                            else
                            {
                                add_obstacle(2, new_car_y);
                            }
                        }
                    }

                    fastcar_counter++;
                    if (fastcar_chance[game_mode][current_level - 1] != -1 && fastcar_counter > fastcar_chance[game_mode][current_level - 1])
                    {
                        fastcar_counter = 0;
                        add_enemy_car(5, o->get_y() + 128);
                    }

                    if (CAR_INTERVAL == 16)
                    {
                        *ip = CAR_INTERVAL + (rand() % 16) - 8;
                    }
                    else if (CAR_INTERVAL == 12)
                    {
                        *ip = CAR_INTERVAL + (rand() % 16) - 8;
                    }
                    else
                    {
                        *ip = CAR_INTERVAL;
                    }
                    (*ip2)++;
                }
            }
        }
    }

    /* Test if all the players are out of fuel: */
    if (game_state == 0)
    {
        bool found = false;
        l.Instance(focusing_objects);
        l.Rewind();
        while (l.Iterate(o))
        {
            CPlayerCarObject* pco = (CPlayerCarObject*)o;
            if (pco->get_fuel() > 0 || pco->get_y_speed() != 0)
                found = true;
        }

        if (!found)
        {
            /* All out of fuel: */
            game_state = 1;
            game_timmer = fade_time * 2;
        }
    }

    /* Test if level completed: */
    if (game_state == 0 && level_completed())
    {
        game_state = 1;
        game_timmer = fade_time * 4;
    }

    if (game_state == 1 && game_timmer < 0)
        return false;
    return true;
}

void CGame::draw(SDL_Surface* sfc, SDL_Rect vp)
{
    if (focusing_objects.Length() > 0)
    {
        int dx = vp.w / focusing_objects.Length();
        SDL_Rect vp2{
            (Sint16)(vp.x + (focusing_objects.Length() - 1) * dx),
            vp.y,
            (Uint16)(focusing_objects.Length() > 1 ? (dx - 1) : dx),
            vp.h
        };

        CObject* o = nullptr;
        List<CObject> l;
        l.Instance(focusing_objects);
        l.Rewind();

        float* fy = nullptr;
        List<float> l2;
        l2.Instance(focusing_fy);
        l2.Rewind();

        int* cp_delay = nullptr;
        List<int> l3;
        l3.Instance(checkpoint_delay);
        l3.Rewind();

        while (1)
        {
            bool a0 = l.Iterate(o);
            bool a1 = l2.Iterate(fy);
            bool a2 = l3.Iterate(cp_delay);
            if (a0 == false || a1 == false || a2 == false)
            {
                break;
            }
            /* draw a viewport: */
            draw(sfc, vp2, (CCarObject*)o, fy, cp_delay);
            vp2.x -= dx;
        }
    }

    {
        float f = std::min<float>(std::max<float>(0.0f, float(game_timmer) / float(fade_time)), 1.0f);
        if (f < 1.0)
        {
            surface_fader(sfc, f, f, f, &vp);
        }
    }

    if (paused)
    {
        surface_fader(sfc, 0.5F, 0.5F, 0.5F, &vp);
        extra_tiles[9]->draw(vp.x + vp.w / 2 - extra_tiles[9]->get_dx() / 2, vp.y + vp.h / 2 - 64, sfc);
    }
}

void CGame::draw(SDL_Surface* sfc, SDL_Rect logic_vp, CCarObject* focusing, float* fy, int* cp_delay)
{
    SDL_Rect vp = logic_vp;

    {
        float f = float(-focusing->get_y_speed()) / MAX_SPEED;
        f = 0.66F + (0.85F - 0.66F) * f;
        *fy = (2 * (*fy) + f) / 3;
    }

    int sx = focusing->get_x() - vp.w / 2;
    int sy = int(focusing->get_y() - vp.h * (*fy));
    if (sx + vp.w > dx)
        sx = dx - vp.w;
    if (sy + vp.h > dy)
        sy = dy - vp.h;
    if (sx < 0)
        sx = 0;
    if (sy < 0)
        sy = 0;
    if (vp.w > dx)
    {
        vp.x = logic_vp.x + vp.w - dx;
        vp.w = dx;
    }
    sx -= vp.x;
    sy -= vp.y;
    SDL_SetClipRect(sfc, &vp);

    if (game_remake_extras && current_level == 3)
    {
        /* 128-96 */
        /* Water animation for level 3: */
        List<CTile> l;
        CTile* t;

        SDL_Surface* tmp = SDL_CreateRGBSurface(0, 128, 96, 32, 0, 0, 0, 0);
        l.Instance(tiles[3]);
        l.Rewind();
        while (l.Iterate(t))
        {
            if (t->r.x == 0 && t->r.y == 0 && t->r.w == 128 && t->r.h == 96)
            {
                SDL_Rect r1{ 0, 0, 128, 96 };
                SDL_Rect r2{ 0, 0, 128, 96 };
                SDL_BlitSurface(t->orig, &r1, tmp, &r2);

                r1.x = 1;
                r1.w = 127;
                SDL_BlitSurface(tmp, &r1, t->orig, &r2);

                r1.x = 0;
                r1.w = 1;
                r2.x = 127;
                SDL_BlitSurface(tmp, &r1, t->orig, &r2);
            }
        }
        SDL_FreeSurface(tmp);
    }

    {
        CObject* o = nullptr;
        List<CObject> l;

        int min = 0;
        int max = 0;

        get_quick_min_max(sy + vp.y, sy + vp.y + vp.h, &min, &max);

        for (int i = min; i <= max; i++)
        {
            l.Instance(quick_background[i]);
            l.Rewind();
            while (l.Iterate(o))
            {
                o->draw(sx, sy, sfc);
            }
        }

        for (int i = min; i <= max; i++)
        {
            l.Instance(quick_middleground[i]);
            l.Rewind();
            while (l.Iterate(o))
            {
                o->draw(sx, sy, sfc);
            }
        }

        /* TYRE MARKS: */
        if (game_remake_extras)
        {
            List<CTyreMark> l;
            l.Instance(tyre_marks);
            l.Rewind();

            if (l.EndP() == false)
            {
                bool cleanup = true;

                SDL_LockSurface(sfc);

                CTyreMark* r;
                while (l.Iterate(r))
                {
                    int y1 = r->y - sy;
                    int y2 = r->y2 - sy;
                    if ((y1 >= 0 && y1 < SCREEN_Y) || (y2 >= 0 && y2 < SCREEN_Y))
                    {
                        cleanup = false;
                        int x1 = r->x - sx;
                        int x2 = r->x2 - sx;
                        draw_line_locked(sfc, x1, y1, x2, y2, 0);
                        draw_line_locked(sfc, x1 + 1, y1, x2 + 1, y2, 0);
                    }
                }

                SDL_UnlockSurface(sfc);

                if (cleanup)
                {
                    tyre_marks.Delete();
                }
            }
        }

        l.Instance(objects);
        l.Rewind();
        while (l.Iterate(o))
        {
            o->draw(sx, sy, sfc);
        }

        for (int i = min; i <= max; i++)
        {
            l.Instance(quick_foreground[i]);
            l.Rewind();
            while (l.Iterate(o))
            {
                o->draw(sx, sy, sfc);
            }
        }
    }

    /* Draw overlays: */
    if (focusing->get_y() < 384)
    {
        (*cp_delay)++;
        ((CPlayerCarObject*)focusing)->reach_goal();
    }

    if ((*cp_delay) >= 0)
    {
        CTile* tile = extra_tiles[current_level == 6 ? 11 : 10];

        if (vp.w >= tile->get_dx())
        {
            int amp = std::min<int>(128, std::max<int>(0, *cp_delay));
            amp = std::max<int>(0, (128 - amp) * (128 - amp) / 128);

            int offs = int(cosf(float((*cp_delay) * (*cp_delay)) / 500.0) * amp);
            if (offs < 0)
                offs = -offs;

            tile->draw(vp.x + (vp.w / 2) - (tile->get_dx() / 2), vp.y + 64 - offs, sfc);
        }
        else
        {
            int x1 = -tile->get_dx();
            int x2 = (vp.w - tile->get_dx()) / 2;
            int i = *cp_delay;

            tile->draw(vp.x + x1 + ((x2 - x1) / 32) * i, vp.y + 32, sfc);
        }
    }

    SDL_SetClipRect(sfc, 0);
}

CObject* CGame::object_collision(int xoffs, int yoffs, CObject* o, int constitution)
{
    List<CObject> l;
    CObject* o2;

    if ((constitution & CONSTITUTION_SOLID) != 0)
    {
        int min = 0, max = 0;

        get_quick_min_max(o, yoffs, &min, &max);

        for (int i = min; i <= max; i++)
        {
            l.Instance(quick_background[i]);
            l.Rewind();
            while (l.Iterate(o2))
                if (o2->collision(xoffs, yoffs, o))
                    return o2;

            l.Instance(quick_middleground[i]);
            l.Rewind();
            while (l.Iterate(o2))
                if (o2->collision(xoffs, yoffs, o))
                    return o2;

            l.Instance(quick_foreground[i]);
            l.Rewind();
            while (l.Iterate(o2))
                if (o2->collision(xoffs, yoffs, o))
                    return o2;
        }
    }

    l.Instance(objects);
    l.Rewind();
    while (l.Iterate(o2))
        if (o2 != o && o2->constitution_test(constitution) && o2->collision(xoffs, yoffs, o))
            return o2;

    return 0;
}

CObject* CGame::add_enemy_car(int type, long y)
{
    CObject* o = 0;
    int i, j;
    int left = 0, right = dx;
    bool coll, last_coll;

    if (type == 0)
        o = (CObject*)new CEnemyNormalCarObject(0, y, enemy_tiles[1], 0, this);
    if (type == 1)
        o = (CObject*)new CEnemyRacerCarObject(0, y, enemy_tiles[0], 0, this);
    if (type == 2)
        o = (CObject*)new CEnemySlidderCarObject(0, y, enemy_tiles[2], 0, this);
    if (type == 3)
        o = (CObject*)new CEnemyTruckObject(0, y, enemy_tiles[3], 0, this);
    if (type == 4)
        o = (CObject*)new CFuelObject(0, y, extra_tiles[0], this);
    if (type == 5)
        o = (CObject*)new CEnemyFastCarObject(0, y, enemy_tiles[0], 0, this);

    if (o == 0)
        return 0;

    /* Find a proper 'x': */
    coll = last_coll = false;
    for (i = 0, j = 0; i < dx && j < 2; i += 4)
    {
        coll = (object_collision(i, 0, o, CONSTITUTION_SOLID | CONSTITUTION_CAR) != 0 ? true : false);

        if (j == 0 && last_coll && !coll)
        {
            left = i;
            j = 1;
        }

        if (j == 1 && !last_coll && coll)
        {
            right = i - 4;
            j = 2;
        }
        last_coll = coll;
    }

    if (left == 0 || right == dx || left > right)
    {
        delete o;
        return 0;
    }

    if (type == 0 || type == 1 || type == 2 || type == 5)
    {
        CEnemyCarObject* po = (CEnemyCarObject*)o;
        po->x = int(left + 8 + (rand() % (right - left - 16)));
        if (std::abs(po->x - left) < std::abs(po->x - right))
        {
            po->following_right_border = false;
            po->distance_to_border = (po->x - left);
        }
        else
        {
            po->following_right_border = true;
            po->distance_to_border = (right - po->x);
        }
    }
    if (type == 3)
        o->x = int(left + 8 + (rand() % (right - left - 16)));
    if (type == 4)
        o->x = (left + right) / 2;

    objects.Insert(o);

    return o;
}

CObject* CGame::add_obstacle(int type, long y)
{
    CObject* o = 0;
    int i, j;
    int left = 0, right = dx;
    bool coll, last_coll;

    if (type == 0)
        o = (CObject*)new CObject(0, y, extra_tiles[6], CONSTITUTION_OIL, this);
    if (type == 1)
        o = (CObject*)new CObject(0, y, extra_tiles[7], CONSTITUTION_WATER, this);
    if (type == 2)
        o = (CObject*)new CObject(0, y, extra_tiles[8], CONSTITUTION_SOLID, this);

    if (o == 0)
        return 0;

    coll = last_coll = false;
    for (i = 0, j = 0; i < dx && j < 2; i += 4)
    {
        coll = (object_collision(i, 0, o, CONSTITUTION_SOLID) != 0 ? true : false);

        if (j == 0 && last_coll && !coll)
        {
            left = i;
            j = 1;
        }

        if (j == 1 && !last_coll && coll)
        {
            right = i - 4;
            j = 2;
        }
        last_coll = coll;
    }

    if ((right - left) < 80)
    {
        /* There's no space: */
        delete o;
        return 0;
    }

    o->x = int(left + 8 + (rand() % (right - left - 16)));
    objects.Insert(o);

    return o;
}

long CGame::min_distance_to_players(long y)
{
    /* Compute de distance to the closest player: */
    List<CObject> l;
    CObject* o;
    long mindistance = PLAYING_WINDOW * 2, distance;

    l.Instance(focusing_objects);
    l.Rewind();
    while (l.Iterate(o))
    {
        distance = std::abs(o->get_y() - y);
        if (mindistance > distance)
            mindistance = distance;
    }

    return mindistance;
}

long CGame::min_distance_to_other_players(long y, CObject* player)
{
    /* Compute de distance to the closest player: */
    List<CObject> l;
    CObject* o;
    long mindistance = PLAYING_WINDOW * 2, distance;

    l.Instance(focusing_objects);
    l.Rewind();
    while (l.Iterate(o))
    {
        if (o != player)
        {
            distance = std::abs(o->get_y() - y);
            if (mindistance > distance)
                mindistance = distance;
        }
    }

    return mindistance;
}

long CGame::min_distance_to_car(long y)
{
    /* Compute de distance to the closest car: */
    List<CObject> l;
    CObject* o;
    long mindistance = PLAYING_WINDOW * 2, distance;

    l.Instance(objects);
    l.Rewind();
    while (l.Iterate(o))
    {
        distance = std::abs(o->get_y() - y);
        if (mindistance < distance)
            mindistance = distance;
    }

    return mindistance;
}

bool CGame::first_player(CObject* player)
{
    List<CObject> l;
    CObject *o, *first_player = 0;

    l.Instance(focusing_objects);
    l.Rewind();
    while (l.Iterate(o))
    {
        if (first_player == 0 || o->get_y() < first_player->get_y())
            first_player = o;
    }

    if (first_player == player)
        return true;

    return false;
}

CObject* CGame::find_closest_player(long x, long y)
{
    /* Compute de distance to the further player: */
    List<CObject> l;
    CObject* o;
    long mindistance = 0, distance;
    CObject* closest = 0;

    l.Instance(focusing_objects);
    l.Rewind();
    while (l.Iterate(o))
    {
        distance = std::abs(o->get_y() - y) + std::abs(o->get_x() - x);
        if (closest == 0 || distance < mindistance)
        {
            closest = o;
            mindistance = distance;
        }
    }

    return closest;
}
