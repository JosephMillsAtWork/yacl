#ifdef __APPLE__
#include "osx-config.h"

#elif __ANDROID__
#include "android-confg.h"

#elif __WIN32
#include "defines/win32-config.h"

#elif __WIN64
#include "defines/win32-config.h"

#elif WIN32
#include "defines/win32-config.h"

#endif



#ifdef __linux__
#ifdef IMX6
    #include "imx6-config.h"
#else
    #include "linux-config.h"
#endif

#endif
