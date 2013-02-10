#ifndef JNI_APPLICATION_H_INCLUDED
#define JNI_APPLICATION_H_INCLUDED


#include <jni.h>
#include <android/log.h>

#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "boost/thread.hpp"


namespace SE
{

extern TApplication* App;

extern boost::mutex InitLock;

extern bool Inited;


template<typename APPTYPE>
APPTYPE* JniInitApp(int width, int height, float matrixWidth, float matrixHeight)
{
	InitLock.lock();
	
	APPTYPE* result = NULL;
	
	try
	{
	
		if (Inited)
		{
			App->OuterDeinit();
			delete App;
			App = NULL;
			Inited = false;
			
			DestroyEngine();
		}
	
		CreateEngine();
		
		*Console<<std::string("Console successfully started!!!");
		
		result = new APPTYPE;
		
		App = result;
	
		App->OuterInit(width, height, matrixWidth, matrixHeight);
	
		Inited = true;
	}
	catch (...)
	{
		throw;
	}
	InitLock.unlock();
	
	return result;
}

extern "C" {
    //JNIEXPORT void JNICALL Java_fishrungames_salmonjnitemplate_JniWrapper_Init(JNIEnv * env, jobject obj,  jint width, jint height);
 
	JNIEXPORT void JNICALL Java_fishrungames_engine_EngineWrapper_Update(JNIEnv * env, jobject obj, long dt);
	JNIEXPORT void JNICALL Java_fishrungames_engine_EngineWrapper_Destroy(JNIEnv * env, jobject obj);
	JNIEXPORT int JNICALL Java_fishrungames_engine_EngineWrapper_IsInited(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_fishrungames_engine_EngineWrapper_OnTapDown(JNIEnv * env, jobject obj, jfloat x, jfloat y, long time);
	JNIEXPORT void JNICALL Java_fishrungames_engine_EngineWrapper_OnTapUp(JNIEnv * env, jobject obj, jfloat x, jfloat y, long time);
	JNIEXPORT void JNICALL Java_fishrungames_engine_EngineWrapper_OnTapUpAfterMove(JNIEnv * env, jobject obj, jfloat x, jfloat y, long time);
	JNIEXPORT void JNICALL Java_fishrungames_engine_EngineWrapper_OnTapMove(JNIEnv * env, jobject obj, float x, float y, long time);
	JNIEXPORT void JNICALL Java_fishrungames_engine_EngineWrapper_OnFling(JNIEnv * env, jobject obj, jfloat velocityX, jfloat velocityY, long time);
	JNIEXPORT void JNICALL Java_fishrungames_engine_EngineWrapper_OnScroll(JNIEnv * env, jobject obj, jfloat distanceX, jfloat distanceY, long time);
	JNIEXPORT void JNICALL Java_fishrungames_engine_EngineWrapper_OnKeyPress(JNIEnv * env, jobject obj, jint keyCode);
};


} // namespace SE

#endif