#include "android_api.h"

#include "main_code.h"
#include "boost\thread.hpp"

std::shared_ptr<TAndroidApplication> App(new TAndroidApplication);

boost::mutex RenderMutex;

float lastOffsetX = 0.5f;

bool OffsetChanged = false;

bool RedBkgPref = false;

JNIEXPORT void JNICALL Java_fishrungames_wallpapertemplate_JniWrapper_Init(JNIEnv * env, jobject obj,  jint width, jint height)
{

	
	RenderMutex.lock();
	
	try
	{
	
		if (App->IsInited())
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
	
	RenderMutex.unlock();
}



JNIEXPORT void JNICALL Java_fishrungames_wallpapertemplate_JniWrapper_Update(JNIEnv * env, jobject obj, long dt)
{
	RenderMutex.unlock();
	
	try
	{
		if (App->IsInited())
		{
			App->OuterDraw();
			App->OuterUpdate(dt);
			
			if (OffsetChanged)
			{
				Renderer->SetAlpha((lastOffsetX) * pi / 180.f);
			}
		}
	}
	catch (...)
	{
		throw;
	}
	
	RenderMutex.unlock();
}


JNIEXPORT void JNICALL Java_fishrungames_wallpapertemplate_JniWrapper_OnTapMove(JNIEnv * env, jobject obj, jfloat x, jfloat y)
{
	try
	{
		if (App->IsInited())
		{
		
			vec2 offset = vec2(x*Renderer->GetMatrixWidth()/static_cast<float>(App->Width), y*Renderer->GetMatrixHeight()/static_cast<float>(App->Height));
			
			App->OuterOnMove(offset);

		}
	}
	catch (...)
	{
		throw;
	}
}


JNIEXPORT void JNICALL Java_fishrungames_wallpapertemplate_JniWrapper_SetOffset(JNIEnv * env, jobject obj, jfloat offsetX, jfloat offsetY)
{

	if (Renderer != NULL)
	{
		OffsetChanged = true;
		lastOffsetX = offsetX;
	}
}

JNIEXPORT void JNICALL Java_fishrungames_wallpapertemplate_JniWrapper_SetOrientation(JNIEnv * env, jobject obj, int isLandscape)
{
}

JNIEXPORT void JNICALL Java_fishrungames_wallpapertemplate_JniWrapper_SetRedBkgPref(JNIEnv * env, jobject obj, jboolean r)
{
	RedBkgPref = r;
}