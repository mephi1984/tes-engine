#include "android_api.h"

#include "main_code.h"


JNIEXPORT void JNICALL Java_fishrungames_salmonjnitemplate_JniWrapper_Init(JNIEnv * env, jobject obj,  jint width, jint height)
{
	JniInitApp<TMyApplication>(width, height, 480.f, 320.f);
}

