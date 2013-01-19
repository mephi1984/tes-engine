#include "main_code.h"

#ifdef TARGET_ANDROID
#include "android_api.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "include/Engine.h"

#include "main_code.h"

void TMyApplication::InnerInit()
{
#ifdef TARGET_ANDROID
    ResourceManager->PathToResources = "";
#endif
#ifdef TARGET_IOS
    ResourceManager->PathToResources = "assets/";
#endif
#ifdef TARGET_WIN32
#ifdef NDEBUG
	ResourceManager->PathToResources = "resources/";
#else
	ResourceManager->PathToResources = "../../../assets/";
#endif
#endif
    
	ResourceManager->ShaderManager.AddShader("DefaultShader", "shader_vertex.txt", "shader_fragment.txt");
	Renderer->PushShader("DefaultShader");


	ResourceManager->TexList.AddTexture(CONST_CONSOLE_TEX_NAME);

	ResourceManager->FontManager.AddFont("droid_sans14", "droid_sans14_font_bitmap.bmp32", "droid_sans14_font_charmap.txt");
	ResourceManager->FontManager.PushFont("droid_sans14");

	//Inited = true; Must be set in android_api.cpp manually
}

void TMyApplication::InnerDeinit()
{
	Inited = false;
}


void TMyApplication::InnerDraw()
{
	//Console->Draw();
}


void TMyApplication::InnerUpdate(cardinal dt)
{

}

