#pragma once

#include <cstdio>

enum filetype
{
    GAMEDATA,
    USERDATA
};

FILE* f1open(const char* f, const char* m, const enum filetype t);
