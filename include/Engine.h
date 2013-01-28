#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#ifdef TARGET_WIN32
#include "SalmonEngineWindows.h"
#endif

#ifdef TARGET_ANDROID
#include "SalmonEngineAndroid.h"
#endif

#ifdef TARGET_IOS
#include "include/SalmonEngineIos.h"
#endif

#endif //ENGINE_H_INCLUDED