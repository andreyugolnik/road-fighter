#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

SDL_Surface* loadImage(const char* path);
Mix_Chunk* loadSound(const char* path);
Mix_Music* loadMusic(const char* path);
TTF_Font* loadFont(const char* path, int size);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
// PPC values:
#define AMASK 0xff000000
#define BMASK 0x000000ff
#define GMASK 0x0000ff00
#define RMASK 0x00ff0000
#define AOFFSET 0
#define BOFFSET 3
#define GOFFSET 2
#define ROFFSET 1

#else
// Intel values:
#define AMASK 0xff000000
#define BMASK 0x000000ff
#define GMASK 0x0000ff00
#define RMASK 0x00ff0000
#define AOFFSET 3
#define BOFFSET 0
#define GOFFSET 1
#define ROFFSET 2

#endif

#ifndef _WIN32

void setupTickCount();
long GetTickCount();

#endif

void pause(unsigned int time);

void putpixel(SDL_Surface* surface, int x, int y, Uint32 pixel);
void maximumpixel(SDL_Surface* surface, int x, int y, Uint32 pixel);
Uint32 getpixel(SDL_Surface* surface, int x, int y);
void draw_rectangle(SDL_Surface* surface, int x, int y, int w, int h, Uint32 pixel);
void draw_line(SDL_Surface* SDL, int x1, int y1, int x2, int y2, Uint32 pixel);
void draw_line_locked(SDL_Surface* SDL, int x1, int y1, int x2, int y2, Uint32 pixel);

void surface_fader(SDL_Surface* surface, float r_factor, float g_factor, float b_factor, SDL_Rect* r);
void surface_shader(SDL_Surface* surface, float factor, int red, int green, int blue, int alpha);
void surface_bicolor(SDL_Surface* surface, float factor, int r1, int g1, int b1, int a1, int r2, int g2, int b2, int a2);

SDL_Surface* load_maskedimage(char* image, char* mask, char* path);

void surface_automatic_alpha(SDL_Surface* sfc);
void surface_bw(SDL_Surface* sfc, int threshold);
void surface_mask_from_bitmap(SDL_Surface* sfc, SDL_Surface* mask, int x, int y);

SDL_Surface* multiline_text_surface(char* text, int line_dist, TTF_Font* font, SDL_Color c);
SDL_Surface* multiline_text_surface2(char* text, int line_dist, TTF_Font* font, SDL_Color c1, SDL_Color c2, int line, float glow);
