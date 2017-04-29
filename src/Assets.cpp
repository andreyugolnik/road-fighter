#include "Assets.h"

#include <cstring>

namespace assets
{
    namespace
    {
        char m_assetsPath[1024];
        unsigned m_assetsPathLength;
    }

    void initialize()
    {
        m_assetsPathLength = 0;
        m_assetsPathLength = 0;
    }

    void deinitialize()
    {
    }

    const char* makePath(const char* path)
    {
        ::strncpy(m_assetsPath + m_assetsPathLength, path, sizeof(m_assetsPath) - m_assetsPathLength);
        return m_assetsPath;
    }
}
