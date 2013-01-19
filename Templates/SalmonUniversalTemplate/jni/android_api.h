#ifndef ANDROID_API_H_INCLUDED
#define ANDROID_API_H_INCLUDED

#include <jni.h>
#include <android/log.h>

#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "boost/shared_ptr.hpp"

#include "main_code.h"

using namespace SE;


extern "C" {
    JNIEXPORT void JNICALL Java_fishrungames_salmonjnitemplate_JniWrapper_Init(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_fishrungames_salmonjnitemplate_JniWrapper_StopSounds(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_fishrungames_salmonjnitemplate_JniWrapper_Update(JNIEnv * env, jobject obj, long dt);
	JNIEXPORT void JNICALL Java_fishrungames_salmonjnitemplate_JniWrapper_Destroy(JNIEnv * env, jobject obj);
	JNIEXPORT int JNICALL Java_fishrungames_salmonjnitemplate_JniWrapper_IsInited(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_fishrungames_salmonjnitemplate_JniWrapper_OnTapDown(JNIEnv * env, jobject obj, jfloat x, jfloat y, long time);
	JNIEXPORT void JNICALL Java_fishrungames_salmonjnitemplate_JniWrapper_OnTapUp(JNIEnv * env, jobject obj, jfloat x, jfloat y, long time);
	JNIEXPORT void JNICALL Java_fishrungames_salmonjnitemplate_JniWrapper_OnTapMove(JNIEnv * env, jobject obj, jfloat x, jfloat y, long time);
	JNIEXPORT void JNICALL Java_fishrungames_salmonjnitemplate_JniWrapper_OnFling(JNIEnv * env, jobject obj, jfloat velocityX, jfloat velocityY, long time);
	JNIEXPORT void JNICALL Java_fishrungames_salmonjnitemplate_JniWrapper_OnScroll(JNIEnv * env, jobject obj, jfloat distanceX, jfloat distanceY, long time);
	JNIEXPORT void JNICALL Java_fishrungames_salmonjnitemplate_JniWrapper_OnKeyPress(JNIEnv * env, jobject obj, jint keyCode);
};


#endif
