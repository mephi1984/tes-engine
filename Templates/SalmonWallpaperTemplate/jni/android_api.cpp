#include "android_api.h"

#include "main_code.h"
#include "boost/thread.hpp"

TAndroidApplication* Application = NULL;

float lastOffsetX = 0.5f;

bool OffsetChanged = false;

bool RedBkgPref = false;

JNIEXPORT void JNICALL Java_fishrungames_wallpapertemplate_JniWrapper_Init(JNIEnv * env, jobject obj,  jint width, jint height)
{

	Application = JniInitApp<TAndroidApplication>(width, height, 800.f, 480.f);
	
	boost::get<TPanoramicCamera>(Renderer->Camera).SetAlpha((lastOffsetX) * pi / 180.f);
	
	/*
	RenderMutex.lock();
	
	try
	{
	
		if (Application->IsInited())
		{
			App->OuterDeinit(); //Clean up what is left at previous launch (if applicable)
		}
		
		App->Width = width;
		App->Height = height;
		
		if (width > height)
		{
			App->OuterInit(width,  height, 800.f, 480.f);
		}
		else
		{
			App->OuterInit(width,  height, 480.f, 800.f);
		}
		
		Renderer->SetAlpha((lastOffsetX) * pi / 180.f);
		
		App->Inited = true;
	}
	catch (...)
	{
		throw;
	}
	
	RenderMutex.unlock();*/
}



JNIEXPORT void JNICALL Java_fishrungames_wallpapertemplate_JniWrapper_SetOffset(JNIEnv * env, jobject obj, jfloat offsetX, jfloat offsetY)
{

	if (Renderer != NULL)
	{
		OffsetChanged = true;
		lastOffsetX = offsetX;
	}
}

JNIEXPORT void JNICALL Java_fishrungames_wallpapertemplate_JniWrapper_SetRedBkgPref(JNIEnv * env, jobject obj, jboolean r)
{
	RedBkgPref = r;
}