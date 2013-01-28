#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED


#ifdef TARGET_SALMON
	#ifdef TARGET_WIN32
		#define SALMON_WIN32
	#endif

	#ifdef TARGET_ANDROID
		#define SALMON_ANDROID
	#endif
    #ifdef TARGET_IOS
        #define SALMON_IOS
    #endif
#endif


#ifdef SALMON_WIN32
#include "SalmonEngineWindows.h"
#endif

#ifdef SALMON_ANDROID
#include "SalmonEngineAndroid.h"
#endif

#ifdef SALMON_IOS
#include "include/SalmonEngineIos.h"
#endif

#endif //ENGINE_H_INCLUDED