#include "FileHandling.h"

#include <cstdlib>
#include <cstring>
#include <sys/stat.h>

#define STRLEN 4096
#define GAMENAME "roadfighter"

FILE* f1open(const char* f, const char* m, const enum filetype t)
{
    if (t == GAMEDATA)
    {
        return ::fopen(f, m);
    }

    char fname[STRLEN];

#if defined(_WIN32)

    char buffer[STRLEN];
    ExpandEnvironmentStrings("%USERPROFILE%", buffer, sizeof(buffer));
    ::snprintf(fname, sizeof(fname), "%s\\.%s\\", buffer, GAMENAME);

#elif defined(__linux__)

    const char* xdgPath = ::getenv("XDG_CONFIG_HOME");
    if (xdgPath != nullptr)
    {
        ::snprintf(fname, sizeof(fname), "%s/%s/%s", xdgPath, GAMENAME, f);
    }
    else
    {
        ::snprintf(fname, sizeof(fname), "%s/.config/%s/%s", ::getenv("HOME"), GAMENAME, f);
    }

#else

    ::snprintf(fname, sizeof(fname), "%s/Library/Application Support/%s/", ::getenv("HOME"), GAMENAME);

#endif

#if defined(_WIN32)
    CreateDirectory(fname, 0);
#else
    ::mkdir(fname, 0700);
#endif

    return ::fopen(fname, m);
}
