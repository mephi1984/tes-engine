#ifndef ANDROID_API_H_INCLUDED
#define ANDROID_API_H_INCLUDED

#include <jni.h>
#include <android/log.h>

#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "boost/shared_ptr.hpp"

#include "main_code.h"

extern "C" {

	JNIEXPORT void JNICALL Java_fishrungames_wallpapertemplate_JniWrapper_Init(JNIEnv * env, jobject obj,  jint width, jint height);
    
	JNIEXPORT void JNICALL Java_fishrungames_wallpapertemplate_JniWrapper_Update(JNIEnv * env, jobject obj, long dt);
	
	JNIEXPORT void JNICALL Java_fishrungames_wallpapertemplate_JniWrapper_OnTapMove(JNIEnv * env, jobject obj, jfloat x, jfloat y);
	
	JNIEXPORT void JNICALL Java_fishrungames_wallpapertemplate_JniWrapper_SetOffset(JNIEnv * env, jobject obj, jfloat offsetX, jfloat offsetY);

	JNIEXPORT void JNICALL Java_fishrungames_wallpapertemplate_JniWrapper_SetOrientation(JNIEnv * env, jobject obj, int isLandscape);

	JNIEXPORT void JNICALL Java_fishrungames_wallpapertemplate_JniWrapper_SetRedBkgPref(JNIEnv * env, jobject obj, jboolean r);
	};


#endif
