
#include "include/Engine.h"

namespace SE
{

TApplication* App = NULL;

boost::mutex InitLock;

bool Inited = false;


JNIEXPORT void JNICALL Java_fishrungames_engine_FileWrapper_Update(JNIEnv * env, jobject obj, long dt)
{
	InitLock.lock();
	try
	{
		if (Inited)
		{
			App->OuterDraw();
			App->OuterUpdate(dt);
		}
	
	
	}
	catch (...)
	{
		throw;
	}
	InitLock.unlock();

}

JNIEXPORT int JNICALL Java_fishrungames_engine_FileWrapper_IsInited(JNIEnv * env, jobject obj)
{   
	if (Inited)
	{
		return 1;
	}
	else
	{
		return 0;
	}

}


JNIEXPORT void JNICALL Java_fishrungames_engine_FileWrapper_Destroy(JNIEnv * env, jobject obj)
{
	InitLock.lock();
	try
	{
		if (Inited)
		{
			App->OuterDeinit();
			
			delete App;

			App = NULL;
		
			DestroyEngine();

			Inited = false;
		}
	
	}
	catch (...)
	{
		throw;
	}
	InitLock.unlock();
}

void DestroyThreaded()
{
	
}

JNIEXPORT void JNICALL Java_fishrungames_engine_FileWrapper_OnTapDown(JNIEnv * env, jobject obj, float x, float y, long time)
{
	InitLock.lock();
	try
	{
		if (Inited)
		{
			App->OuterOnTapDown(vec2(x,y));
		}
	}
	catch (...)
	{
		throw;
	}
	InitLock.unlock();
}


JNIEXPORT void JNICALL Java_fishrungames_engine_FileWrapper_OnTapUp(JNIEnv * env, jobject obj, float x, float y, long time)
{
	InitLock.lock();
	try
	{
		if (Inited)
		{
			App->OuterOnTapUp(vec2(x,y));
		}
	}
	catch (...)
	{
		throw;
	}
	InitLock.unlock();
}

JNIEXPORT void JNICALL Java_fishrungames_engine_FileWrapper_OnTapUpAfterMove(JNIEnv * env, jobject obj, jfloat x, jfloat y, long time)
{
	InitLock.lock();
	try
	{
		if (Inited)
		{
			App->OuterOnTapUpAfterMove(vec2(x,y));
		}
	}
	catch (...)
	{
		throw;
	}
	InitLock.unlock();
}

JNIEXPORT void JNICALL Java_fishrungames_engine_FileWrapper_OnTapMove(JNIEnv * env, jobject obj, float x, float y, long time)
{
	InitLock.lock();
	try
	{
		if (Inited)
		{
			App->OuterOnMove(vec2(x,y));
		}
	}
	catch (...)
	{
		throw;
	}
	InitLock.unlock();
}

JNIEXPORT void JNICALL Java_fishrungames_engine_FileWrapper_OnFling(JNIEnv * env, jobject obj, jfloat velocityX, jfloat velocityY, long time)
{
    
}

JNIEXPORT void JNICALL Java_fishrungames_engine_FileWrapper_OnScroll(JNIEnv * env, jobject obj, jfloat distanceX, jfloat distanceY, long time)
{
	InitLock.lock();
	try
	{
		if (Inited)
		{
			App->OuterOnMove(vec2(distanceX,distanceY));
		}
	}
	catch (...)
	{
		throw;
	}
	InitLock.unlock();
}

JNIEXPORT void JNICALL Java_fishrungames_engine_FileWrapper_OnKeyPress(JNIEnv * env, jobject obj, jint keyCode)
{
	InitLock.lock();
	try
	{
		if (Inited)
		{
			App->OnKeyPress(keyCode);
		}
	}
	catch (...)
	{
		throw;
	}
	InitLock.unlock();
}

} // namespace SE
