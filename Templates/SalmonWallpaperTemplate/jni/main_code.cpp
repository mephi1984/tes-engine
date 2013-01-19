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

extern float lastOffsetX;

#ifdef TARGET_WIN32
bool RedBkgPref = true;

#else
extern bool RedBkgPref;

#endif





TAndroidApplication::TAndroidApplication()
	: TApplication()
	, Inited(false)
	//, LiteModel(NULL)
{
}


void TAndroidApplication::LoadModels()
{

	mat3 m(vec4(0,1*sin(pi/8 + pi/2),0,1*cos(pi/8 + pi/2)));
	/*
	LiteModel = new TLiteModel;
	LiteModel->LoadModel("bt_box_yellow.lm1");
	LiteModel->ScaleModel(10.0f);
	LiteModel->RotateModel(m);
	LiteModel->UpdateVBO();*/

	FlexModel = ResourceManager->FlexModelManager.InstanciateModel("model.xml");

	//FlexModel.MoveModel(vec3(1, -4, -4));
	FlexModel.ScaleModel(1.5f);
	FlexModel.RotateModel(m);
	FlexModel.RefreshBuffer();
}


void TAndroidApplication::DrawAllScene()
{
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	Renderer->PushProjectionMatrix(1,1);
	Renderer->LoadIdentity();

	if (RedBkgPref)
	{
		glBindTexture(GL_TEXTURE_2D, ResourceManager->TexList["bkg_red.png"]);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, ResourceManager->TexList["bkg_blue.png"]);
	}

	Renderer->DrawRect(vec2(0.0f, 0.f), vec2(1.f, 1.f), vec2(0.f, 0.f), vec2(1.f, 1.f));

	Renderer->PopProjectionMatrix();

	glClear(GL_DEPTH_BUFFER_BIT);

	Renderer->SetGLCamView();

	//LiteModel->DrawVBO();
	FlexModel.Draw();
		
}

void TAndroidApplication::InnerInit()
{
	RenderMutex.lock();

	CheckGlError();

	glEnable(GL_BLEND);

	*Console<<"Inner init go!\n";

#ifdef TARGET_ANDROID
	ResourceManager->PathToResources = "";
#endif
#ifdef TARGET_WIN32
	ResourceManager->PathToResources = "../../assets/";
#endif
	ResourceManager->ShaderManager.AddShader("DefaultShader", "gui_transparent.vertex", "gui_transparent.fragment");
	Renderer->PushShader("DefaultShader");

	ResourceManager->TexList.AddTexture("bkg_red.png");
	ResourceManager->TexList.AddTexture("bkg_blue.png");

	CheckGlError();

	Renderer->MovePhi(pi/32);

	//ResourceManager->ModelManager.AddLiteModel("bt_box_yellow.lm1");

	
	ResourceManager->TexList.AddTexture("bt_box_yellow.bmp");
	ResourceManager->FlexModelManager.LoadModelFromXml("model.xml");


	LoadModels();

	if (Width > Height)
	{
		Renderer->MoveDist(30.f);
	}

	else
	{
		Renderer->MoveDist(45.f);
	}

	CheckGlError();

	Renderer->SetPerspectiveFullScreenViewport();

	*Console<<"Inner init end!\n";

	RenderMutex.unlock();
}



void TAndroidApplication::InnerDeinit()
{
	RenderMutex.lock();
	Inited = false;
	/*
	if (LiteModel != NULL)
	{
		LiteModel->FreeModel();
		delete LiteModel;
		LiteModel = NULL;
	}*/

	RenderMutex.unlock();
}


void TAndroidApplication::InnerDraw()
{
	RenderMutex.lock();
	
	DrawAllScene();
	
	RenderMutex.unlock();

}


void TAndroidApplication::InnerUpdate(cardinal dt)
{
	
}

void TAndroidApplication::InnerOnMove(vec2 shift)
{
	Renderer->MoveAlpha(-pi*shift.v[0]*0.1f);
}

void TAndroidApplication::OnMouseDown(TMouseState& mouseState)
{
	
}