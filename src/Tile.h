#pragma once
#define __RF_TILE_CLASS

#include "sge/sge_collision.h"

#include <cstdio>

class CTile final
{
    friend class CObject;
    friend class CGame;

public:
    CTile();
    CTile(int x, int y, int dx, int dy, SDL_Surface* orig, bool collision);
    ~CTile();

    void draw(int x, int y, SDL_Surface* dest);
    void draw_mask(int x, int y, SDL_Surface* dest);
    void draw_collision_mask(int x, int y, SDL_Surface* dest);

    void draw_scaled(int x, int y, SDL_Surface* dest, float scale);
    void draw_shaded(int x, int y, SDL_Surface* dest, int factor, int r, int g, int b, int a);
    void draw_bicolor(int x, int y, SDL_Surface* dest, int factor, int r1, int g1, int b1, int a1, int r2, int g2, int b2, int a2);

    void clear();
    void free();
    void instance(CTile* t);

    int get_dx() const
    {
        return r.w;
    };

    int get_dy() const
    {
        return r.h;
    };

private:
    SDL_Rect r;
    SDL_Surface *orig, *mask_visualization, *mask_collision;
    sge_cdata* collision_data;
};

/* TILE_SOURCE CLASS: */

class TILE_SOURCE final
{
    friend class CTile;
    friend class CGame;

public:
    TILE_SOURCE();
    TILE_SOURCE(char* filename);
    ~TILE_SOURCE();

    bool save(FILE* fp);
    bool load(FILE* fp);

    bool cmp(char* n);

private:
    SDL_Surface* sfc;
    char* fname;
};
