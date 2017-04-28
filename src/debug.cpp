#include "FileHandling.h"

#ifdef KITSCHY_DEBUG_MEMORY
#include "debug_memorymanager.h"
#endif

#ifdef _WIN32
#include <Windows.h>
#endif

#include <cstdarg>
#include <cstdio>

FILE* fp = nullptr;

void output_debug_message(const char* fmt, ...)
{
    char text[256];
    va_list ap;

    if (fmt == 0)
        return;

    va_start(ap, fmt);
    vsprintf(text, fmt, ap);
    va_end(ap);

    if (fp == nullptr)
        fp = f1open("roadfighter.dbg", "w", USERDATA);

    fprintf(fp, "%s", text);
    fflush(fp);
}

void close_debug_messages()
{
    fclose(fp);
    fp = nullptr;
}
