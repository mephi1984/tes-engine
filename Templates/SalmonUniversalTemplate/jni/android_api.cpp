#include "android_api.h"

#include "main_code.h"
#include "boost\thread.hpp"

boost::shared_ptr<TMyApplication> App(new TMyApplication);

boost::mutex m;

JNIEXPORT void JNICALL Java_fishrungames_salmonjnitemplate_JniWrapper_Init(JNIEnv * env, jobject obj,  jint width, jint height)
{

	m.lock();
	try
	{
	if (App->IsInited())
	{
		App->OuterDeinit(); //Clean up what is left at previous launch (if applicable)
	}
	
	App->OuterInit(width, height, 480.f, 320.f);
	
	App->Inited = true;
	}
	catch (...)
	{
		throw;
	}
	m.unlock();
}	

JNIEXPORT void JNICALL Java_fishrungames_salmonjnitemplate_JniWrapper_StopSounds(JNIEnv * env, jobject obj)
{

}


JNIEXPORT void JNICALL Java_fishrungames_salmonjnitemplate_JniWrapper_Update(JNIEnv * env, jobject obj, long dt)
{
m.lock();
	try
	{
	if (App->IsInited())
	{
		App->OuterDraw();
		App->OuterUpdate(dt);
	}
	
	}
	catch (...)
	{
		throw;
	}
	m.unlock();

}

JNIEXPORT int JNICALL Java_fishrungames_salmonjnitemplate_JniWrapper_IsInited(JNIEnv * env, jobject obj)
{   

    if (App->IsInited())
    {
        return 1;
    }
    else
    {
        return 0;
    }

}

JNIEXPORT void JNICALL Java_fishrungames_salmonjnitemplate_JniWrapper_Destroy(JNIEnv * env, jobject obj)
{
m.lock();
	try
	{
	if (App->IsInited())
	{
		App->OuterDeinit();
		App->Inited = false;
	}
	
	}
	catch (...)
	{
		throw;
	}
	m.unlock();
}


JNIEXPORT void JNICALL Java_fishrungames_salmonjnitemplate_JniWrapper_OnTapDown(JNIEnv * env, jobject obj, float x, float y, long time)
{
m.lock();
	try
	{
		Renderer->MoveDist(1.0f);
	}
	catch (...)
	{
		throw;
	}
	m.unlock();
}


JNIEXPORT void JNICALL Java_fishrungames_salmonjnitemplate_JniWrapper_OnTapUp(JNIEnv * env, jobject obj, float x, float y, long time)
{
}

JNIEXPORT void JNICALL Java_fishrungames_salmonjnitemplate_JniWrapper_OnTapMove(JNIEnv * env, jobject obj, float x, float y, long time)
{
}

JNIEXPORT void JNICALL Java_fishrungames_salmonjnitemplate_JniWrapper_OnFling(JNIEnv * env, jobject obj, jfloat velocityX, jfloat velocityY, long time)
{
    
}

JNIEXPORT void JNICALL Java_fishrungames_salmonjnitemplate_JniWrapper_OnScroll(JNIEnv * env, jobject obj, jfloat distanceX, jfloat distanceY, long time)
{
    
}

JNIEXPORT void JNICALL Java_fishrungames_salmonjnitemplate_JniWrapper_OnKeyPress(JNIEnv * env, jobject obj, jint keyCode)
{
	//App->OnKeyPress(keyCode);
}
