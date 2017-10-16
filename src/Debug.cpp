#include "FileHandling.h"

#include <cstdarg>
#include <cstdio>

FILE* fp = nullptr;

void output_debug_message(const char* fmt, ...)
{
    if (fmt == nullptr)
    {
        return;
    }

    char text[1024];
    va_list ap;

    va_start(ap, fmt);
    ::vsnprintf(text, sizeof(text), fmt, ap);
    va_end(ap);

    ::printf("dbg: %s", text);

#if !defined(__EMSCRIPTEN__)
    if (fp == nullptr)
    {
        fp = f1open("roadfighter.dbg", "w", USERDATA);
    }
    ::fprintf(fp, "%s", text);
    ::fflush(fp);
#endif
}

void close_debug_messages()
{
    if (fp != nullptr)
    {
        fclose(fp);
        fp = nullptr;
    }
}
