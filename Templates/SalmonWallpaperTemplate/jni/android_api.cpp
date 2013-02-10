#include "android_api.h"

#include "main_code.h"
#include "boost/thread.hpp"

TAndroidApplication* Application = NULL;

float lastOffsetX = 0.5f;

bool OffsetChanged = false;

bool RedBkgPref = false;

JNIEXPORT void JNICALL Java_fishrungames_wallpapertemplate_JniWrapper_Init(JNIEnv * env, jobject obj,  jint width, jint height)
{

	Application = JniInitApp<TAndroidApplication>(width, height, width, height);
	
	boost::get<TPanoramicCamera>(Renderer->Camera).SetAlpha((lastOffsetX) * pi / 180.f);

}



JNIEXPORT void JNICALL Java_fishrungames_wallpapertemplate_JniWrapper_SetOffset(JNIEnv * env, jobject obj, jfloat offsetX, jfloat offsetY)
{

	if (Renderer != NULL)
	{
		OffsetChanged = true;
		lastOffsetX = offsetX;
		boost::get<TPanoramicCamera>(Renderer->Camera).SetAlpha((lastOffsetX) * pi / 180.f);
	}
}

JNIEXPORT void JNICALL Java_fishrungames_wallpapertemplate_JniWrapper_SetRedBkgPref(JNIEnv * env, jobject obj, jboolean r)
{
	RedBkgPref = r;
}