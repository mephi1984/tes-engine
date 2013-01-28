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
	
	*Console<<"Inner init go!\n";

#ifdef TARGET_ANDROID
	ResourceManager->PathToResources = "";
#endif
#ifdef TARGET_WIN32
#ifdef NDEBUG
	ResourceManager->PathToResources = "resources/";
#else
	ResourceManager->PathToResources = "../../../assets/";
#endif
#endif
#ifdef TARGET_IOS
	ResourceManager->PathToResources = "assets/";
#endif

	ResourceManager->TexList.AddTexture(CONST_CONSOLE_TEX_NAME);

	ResourceManager->ShaderManager.AddShader("DefaultShader", "gui_transparent.vertex", "gui_transparent.fragment");
    
    Renderer->PushShader("DefaultShader");

	ResourceManager->FontManager.AddFont("droid_sans14", "droid_sans14_font_bitmap.bmp32", "droid_sans14_font_charmap.txt");
	ResourceManager->FontManager.PushFont("droid_sans14");

	ResourceManager->SoundManager.LoadMusic("level1ogg.ogg");
	
    ResourceManager->SoundManager.LoadSound("shot.wav");
	
    boost::get<TPanoramicCamera>(Renderer->Camera).MovePhi(pi/6);
	boost::get<TPanoramicCamera>(Renderer->Camera).MoveDist(6.f);
	
    boost::get<TPanoramicCamera>(Renderer->Camera).CalcCamVec();
    
	ResourceManager->LightManager.SetLightDirection(vec3(-1,0,-1));
	ResourceManager->LightManager.SetLightColor(vec4(1,0,0,1));

	ResourceManager->TexList.AddTexture("bt_box_yellow.bmp");
	ResourceManager->FlexModelManager.LoadModelFromXml("model.xml");

	FlexModel = ResourceManager->FlexModelManager.InstanciateModel("model.xml");

	FlexModel.MoveModel(vec3(1, -4, -4));
	FlexModel.ScaleModel(0.4f);
	FlexModel.RefreshBuffer();

	//Uncomment to start music playing
	//ResourceManager->SoundManager.PlayMusic("level1ogg.ogg");

    Renderer->PushPerspectiveProjectionMatrix(pi/6, Renderer->GetMatrixWidth()/Renderer->GetMatrixHeight(), 1.f, 100.f);
    
	*Console<<"Inner init end!\n";
}

void TMyApplication::InnerDeinit()
{
}


void TMyApplication::InnerDraw()
{
    
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Renderer->SetGLCamView();
	FlexModel.Draw();
	
}


void TMyApplication::InnerUpdate(cardinal dt)
{

}

