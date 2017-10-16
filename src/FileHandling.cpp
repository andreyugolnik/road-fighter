#include "FileHandling.h"

#include <cstdlib>
#include <cstring>
#include <sys/stat.h>

#define STRLEN 4096
#define GAMENAME "roadfighter"

FILE* f1open(const char* file, const char* m, const enum filetype t)
{
    if (t == GAMEDATA)
    {
        return ::fopen(file, m);
    }

    char profileDir[STRLEN];

#if defined(_WIN32)

    char buffer[STRLEN];
    ExpandEnvironmentStrings("%USERPROFILE%", buffer, sizeof(buffer));
    ::snprintf(profileDir, sizeof(profileDir), "%s\\.%s\\", buffer, GAMENAME);

#elif defined(__linux__)

    const char* xdgPath = ::getenv("XDG_CONFIG_HOME");
    if (xdgPath != nullptr)
    {
        ::snprintf(profileDir, sizeof(profileDir), "%s/%s/", xdgPath, GAMENAME);
    }
    else
    {
        ::snprintf(profileDir, sizeof(profileDir), "%s/.config/%s/", ::getenv("HOME"), GAMENAME);
    }

#else

    ::snprintf(profileDir, sizeof(profileDir), "%s/Library/Application Support/%s/", ::getenv("HOME"), GAMENAME);

#endif

#if defined(_WIN32)
    CreateDirectory(profileDir, 0);
#else
    ::mkdir(profileDir, 0700);
#endif

    char filePath[STRLEN];
    ::snprintf(filePath, sizeof(filePath), "%s/%s", profileDir, file);

    return ::fopen(filePath, m);
}
