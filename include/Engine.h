#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED



#ifdef TARGET_HALIBUT
	#ifdef TARGET_WIN32
		#define HALIBUT_WIN32
	#endif

	#ifdef TARGET_ANDROID
		#define HALIBUT_ANDROID
	#endif
    #ifdef TARGET_IOS
        #define HALIBUT_IOS
    #endif
#endif

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

#ifdef HALIBUT_WIN32
#include "HalibutEngineWindows.h"
#endif


#ifdef SALMON_ANDROID
#include "SalmonEngineAndroid.h"
#endif
#ifdef HALIBUT_ANDROID
#include "HalibutEngineAndroid.h"
#endif


#ifdef SALMON_IOS
#include "include/SalmonEngineIos.h"
#endif
#ifdef HALIBUT_IOS
#include "include/HalibutEngineIos.h"
#endif

#endif //ENGINE_H_INCLUDED