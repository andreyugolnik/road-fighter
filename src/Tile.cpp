#include "Tile.h"
#include "Auxiliar.h"
#include "sge/sge.h"

#include <SDL/SDL_image.h>

CTile::CTile()
    : r({ 0, 0, 0, 0 })
    , orig(nullptr)
    , mask_visualization(nullptr)
    , mask_collision(nullptr)
    , collision_data(nullptr)
{
}

CTile::CTile(int x, int y, int dx, int dy, SDL_Surface* o, bool collision)
    : r({ (Sint16)x, (Sint16)y, (Uint16)dx, (Uint16)dy })
{
    orig = SDL_CreateRGBSurface(0, dx, dy, 32, RMASK, GMASK, BMASK, AMASK);
    SDL_SetAlpha(orig, 0, SDL_ALPHA_OPAQUE);
    SDL_BlitSurface(o, &r, orig, nullptr);

    SDL_SetAlpha(orig, SDL_SRCALPHA, SDL_ALPHA_OPAQUE);
    surface_mask_from_bitmap(orig, o, r.x + r.w, r.y);
    mask_visualization = nullptr;

    if (collision)
    {
        SDL_Rect r2{ (Sint16)(r.x + (r.w * 2)), r.y, r.w, r.h };
        mask_collision = SDL_CreateRGBSurface(0, r.w, r.h, 32, RMASK, GMASK, BMASK, AMASK);
        SDL_BlitSurface(o, &r2, mask_collision, nullptr);
        surface_bw(mask_collision, 128);
        Uint32 colorKey = 0; //mask_collision->format->colorkey;
        collision_data = sge_make_cmap(mask_collision, colorKey);
    }
    else
    {
        mask_collision = nullptr;
        collision_data = nullptr;
    }
}

CTile::~CTile()
{
    free();
}

void CTile::draw(int x, int y, SDL_Surface* dest)
{
    if (orig != nullptr)
    {
        // Emscripten SDL has bug with negative x and y,
        // so I fix input and output rects.

        SDL_Rect d{
            (Sint16)(x >= 0 ? x : 0),
            (Sint16)(y >= 0 ? y : 0),
            (Uint16)(x >= 0 ? r.w : (r.w + x)),
            (Uint16)(y >= 0 ? r.h : (r.h + y))
        };

        if (x >= 0 && y >= 0)
        {
            SDL_BlitSurface(orig, nullptr, dest, &d);
        }
        else
        {
            Sint16 nx = x >= 0 ? 0 : -x;
            Sint16 ny = y >= 0 ? 0 : -y;
            SDL_Rect s{ nx, ny, (Uint16)(orig->w - x), (Uint16)(orig->h - y) };
            SDL_BlitSurface(orig, &s, dest, &d);
        }

        // SDL_BlitSurface(mask_collision,0,dest,&d);
    }
}

void CTile::draw_collision_mask(int x, int y, SDL_Surface* dest)
{
    if (orig != nullptr)
    {
        if (mask_collision != nullptr)
        {
            SDL_Rect d{ (Sint16)x, (Sint16)y, r.w, r.h };
            SDL_BlitSurface(mask_collision, nullptr, dest, &d);
        }
    }
}

void CTile::draw_shaded(int x, int y, SDL_Surface* dest, int factor, int red, int green, int blue, int alpha)
{
    if (orig != nullptr)
    {
        SDL_Surface* tmp = SDL_DisplayFormatAlpha(orig);

        SDL_Rect d{ 0, 0, r.w, r.h };
        surface_shader(tmp, factor / 100.0F, red, green, blue, alpha);

        d.x = x;
        d.y = y;
        SDL_BlitSurface(tmp, 0, dest, &d);

        SDL_FreeSurface(tmp);
    }
}

void CTile::draw_bicolor(int x, int y, SDL_Surface* dest, int factor, int r1, int g1, int b1, int a1, int r2, int g2, int b2, int a2)
{
    if (orig != nullptr)
    {
        SDL_Surface* tmp = SDL_DisplayFormatAlpha(orig);

        SDL_Rect d{ 0, 0, r.w, r.h };
        surface_bicolor(tmp, factor / 100.0F, r1, g1, b1, a1, r2, g2, b2, a2);

        d.x = x;
        d.y = y;
        SDL_BlitSurface(tmp, nullptr, dest, &d);

        SDL_FreeSurface(tmp);
    }
}

void CTile::draw_scaled(int x, int y, SDL_Surface* dest, float scale)
{
    if (orig != nullptr)
    {
        sge_transform(orig, dest, 0, scale, scale, 0, 0, x, y, 0);
    }

    /*
    if (orig != 0)
    {
        SDL_Rect d;
        SDL_Surface* res = zoomSurface(orig, scale, scale, 0);

        d.x = x;
        d.y = y;
        d.w = res->w;
        d.h = res->h;
        SDL_BlitSurface(res, 0, dest, &d);

        SDL_FreeSurface(res);
    }
    */
}

void CTile::draw_mask(int x, int y, SDL_Surface* dest)
{
    if (orig != nullptr)
    {
        if (mask_visualization == nullptr)
        {
            mask_visualization = SDL_CreateRGBSurface(SDL_HWSURFACE, r.w, r.h, 32, 0, 0, 0, 0);
            SDL_LockSurface(orig);
            SDL_LockSurface(mask_visualization);

            for (int i = 0; i < r.w; i++)
            {
                for (int j = 0; j < r.h; j++)
                {
                    Uint8 r, g, b, a;

                    Uint32 color = getpixel(orig, i, j);
                    SDL_GetRGBA(color, orig->format, &r, &g, &b, &a);

                    color = SDL_MapRGBA(mask_visualization->format, a, a, a, 0);
                    putpixel(mask_visualization, i, j, color);
                }
            }

            SDL_UnlockSurface(mask_visualization);
            SDL_UnlockSurface(orig);
        }

        SDL_Rect d{ (Sint16)x, (Sint16)y, r.w, r.h };
        SDL_BlitSurface(mask_visualization, nullptr, dest, &d);
    }
}

void CTile::clear()
{
    orig = nullptr;
    mask_visualization = nullptr;
    mask_collision = nullptr;
    collision_data = nullptr;
}

void CTile::free()
{
    if (orig != nullptr)
        SDL_FreeSurface(orig);
    orig = nullptr;

    if (mask_visualization != nullptr)
        SDL_FreeSurface(mask_visualization);
    mask_visualization = nullptr;

    if (mask_collision != nullptr)
        SDL_FreeSurface(mask_collision);
    mask_collision = nullptr;

    if (collision_data != nullptr)
        sge_destroy_cmap(collision_data);
    collision_data = nullptr;
}

void CTile::instance(CTile* t)
{
    r = t->r;
    orig = t->orig;
    mask_visualization = t->mask_visualization;
    mask_collision = t->mask_collision;
    collision_data = t->collision_data;
}

TILE_SOURCE::TILE_SOURCE()
{
    fname = nullptr;
    sfc = nullptr;
}

TILE_SOURCE::TILE_SOURCE(const char* filename)
{
    fname = new char[strlen(filename) + 1];
    strcpy(fname, filename);

    SDL_Surface* tmp_sfc = loadImage(fname);

    sfc = SDL_CreateRGBSurface(SDL_HWSURFACE, tmp_sfc->w, tmp_sfc->h, 32, 0, 0, 0, AMASK);
    SDL_SetAlpha(sfc, 0, SDL_ALPHA_OPAQUE);
    SDL_BlitSurface(tmp_sfc, nullptr, sfc, nullptr);

    SDL_FreeSurface(tmp_sfc);
}

TILE_SOURCE::~TILE_SOURCE()
{
    delete fname;
    fname = nullptr;
    SDL_FreeSurface(sfc);
}

bool TILE_SOURCE::save(FILE* fp)
{
    fprintf(fp, "%s\n", fname);

    return true;
}

bool TILE_SOURCE::load(FILE* fp)
{
    char tmp[256];
    if (1 != fscanf(fp, "%s", tmp))
        return false;

    delete fname;
    fname = new char[strlen(tmp) + 1];
    strcpy(fname, tmp);

    SDL_Surface* tmp_sfc = loadImage(fname);

    sfc = SDL_CreateRGBSurface(SDL_HWSURFACE, tmp_sfc->w, tmp_sfc->h, 32, 0, 0, 0, AMASK);
    SDL_SetAlpha(sfc, 0, SDL_ALPHA_OPAQUE);
    SDL_BlitSurface(tmp_sfc, nullptr, sfc, nullptr);

    SDL_FreeSurface(tmp_sfc);

    return true;
}

bool TILE_SOURCE::cmp(const char* n)
{
    return strcmp(n, fname) == 0;
}
