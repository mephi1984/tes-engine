
#include "include/Engine.h"

namespace SE {

	TApplication *App = NULL;

	boost::mutex InitLock;

	bool Inited = false;
} // namespace SE

extern "C"
JNIEXPORT void JNICALL Java_fishrungames_salmonengineandroid_EngineWrapper_Update(JNIEnv * env, jobject obj, long dt)
{
	SE::InitLock.lock();
	try
	{
		if (SE::Inited)
		{
			SE::App->OuterDraw();
			SE::App->OuterUpdate(dt);
		}
	
	
	}
	catch (...)
	{
		throw;
	}
	SE::InitLock.unlock();

}

extern "C"
JNIEXPORT int JNICALL Java_fishrungames_salmonengineandroid_EngineWrapper_IsInited(JNIEnv * env, jobject obj)
{   
	if (SE::Inited)
	{
		return 1;
	}
	else
	{
		return 0;
	}

}

extern "C"
JNIEXPORT void JNICALL Java_fishrungames_salmonengineandroid_EngineWrapper_Destroy(JNIEnv * env, jobject obj)
{
	SE::InitLock.lock();
	try
	{
		if (SE::Inited)
		{
			SE::App->OuterDeinit();
			
			delete SE::App;

			SE::App = NULL;

			SE::DestroyEngine();

			SE::Inited = false;
		}
	
	}
	catch (...)
	{
		throw;
	}
	SE::InitLock.unlock();
}

void DestroyThreaded()
{
	
}

extern "C"
JNIEXPORT void JNICALL Java_fishrungames_salmonengineandroid_EngineWrapper_OnTapDown(JNIEnv * env, jobject obj, float x, float y, long time)
{
	SE::InitLock.lock();
	try
	{
		if (SE::Inited)
		{
			SE::App->OuterOnTapDown(Eigen::Vector2f(x,y), time);
		}
	}
	catch (...)
	{
		throw;
	}
	SE::InitLock.unlock();
}

extern "C"
JNIEXPORT void JNICALL Java_fishrungames_salmonengineandroid_EngineWrapper_OnTapUp(JNIEnv * env, jobject obj, float x, float y, long time)
{
	SE::InitLock.lock();
	try
	{
		if (SE::Inited)
		{
			SE::App->OuterOnTapUp(Eigen::Vector2f(x,y), time);
		}
	}
	catch (...)
	{
		throw;
	}
	SE::InitLock.unlock();
}

extern "C"
JNIEXPORT void JNICALL Java_fishrungames_salmonengineandroid_EngineWrapper_OnTapUpAfterMove(JNIEnv * env, jobject obj, jfloat x, jfloat y, long time)
{
	SE::InitLock.lock();
	try
	{
		if (SE::Inited)
		{
			SE::App->OuterOnTapUpAfterMove(Eigen::Vector2f(x,y), time);
		}
	}
	catch (...)
	{
		throw;
	}
	SE::InitLock.unlock();
}

extern "C"
JNIEXPORT void JNICALL Java_fishrungames_salmonengineandroid_EngineWrapper_OnTapMove(JNIEnv * env, jobject obj, float posx, float posy, float shiftx, float shifty, long time)
{
	SE::InitLock.lock();
	try
	{
		if (SE::Inited)
		{
			SE::App->OuterOnMove(Eigen::Vector2f(posx,posy), Eigen::Vector2f(shiftx,shifty), time);
		}
	}
	catch (...)
	{
		throw;
	}
	SE::InitLock.unlock();
}

extern "C"
JNIEXPORT void JNICALL Java_fishrungames_salmonengineandroid_EngineWrapper_OnFling(JNIEnv * env, jobject obj, jfloat velocityX, jfloat velocityY, long time)
{
    
}

extern "C"
JNIEXPORT void JNICALL Java_fishrungames_salmonengineandroid_EngineWrapper_OnScroll(JNIEnv * env, jobject obj, float posx, float posy, jfloat distanceX, jfloat distanceY, long time)
{
	SE::InitLock.lock();
	try
	{
		if (SE::Inited)
		{
			//SE::App->OuterOnMove(Eigen::Vector2f(distanceX,distanceY), time);
			SE::App->OuterOnMove(Eigen::Vector2f(posx,posy), Eigen::Vector2f(distanceX,distanceY), time);
		}
	}
	catch (...)
	{
		throw;
	}
	SE::InitLock.unlock();
}

extern "C"
JNIEXPORT void JNICALL Java_fishrungames_salmonengineandroid_EngineWrapper_OnKeyPress(JNIEnv * env, jobject obj, jint keyCode)
{
	SE::InitLock.lock();
	try
	{
		if (SE::Inited)
		{
			SE::App->OnKeyPress(keyCode);
		}
	}
	catch (...)
	{
		throw;
	}
	SE::InitLock.unlock();
}


