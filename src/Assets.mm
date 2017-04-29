#include "Assets.h"

#import <Foundation/Foundation.h>
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
        CFBundleRef mainBundle = CFBundleGetMainBundle();
        if (mainBundle)
        {
            CFURLRef executable = CFBundleCopyExecutableURL(mainBundle);
            CFStringRef string = CFURLCopyFileSystemPath(executable, kCFURLPOSIXPathStyle);

            CFStringGetCString(string, m_assetsPath, sizeof(m_assetsPath), kCFStringEncodingUTF8);

            CFRelease(string);
            CFRelease(executable);
        }

        char* p = ::strrchr(m_assetsPath, '/');
        if (p != nullptr)
        {
            p[0] = 0;
            p = ::strrchr(m_assetsPath, '/');
            if (p != nullptr)
            {
                p[1] = 0;
                ::strcat(m_assetsPath, "Resources/");
            }
            else
            {
                m_assetsPath[0] = 0;
            }
        }
        else
        {
            m_assetsPath[0] = 0;
        }

        m_assetsPathLength = ::strlen(m_assetsPath);
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
