#pragma once

/*
This code combines all headers for Salmon engine into one header file
*/

#include "include/SalmonEngineInterface.h"

namespace SE
{

//==============================================================
//========= GLOBAL VARIABLES - REMEMBER THIS LIST!!! ===========
//==============================================================

extern TFileConsole* Console;
extern TSalmonRenderer* Renderer;
extern TResourceManager* ResourceManager;


extern HGLRC hRC;		//Render context
extern HWND  Hwnd;		//Main window handle
extern HDC hDC;			//Device context


class TApplication : public TApplicationAncestor
{
public:
	
	TApplication();

	~TApplication();

	virtual void UpdateQuick() { };
	//To process input - this method is called more frequently than Update()  

	virtual void OnMouseWheel(short int delta) { }
	//To do on mouse wheel move

	virtual void OnKeyPress(cardinal key); //Try not to override this. But if you need to override, call ancestor!
};

bool CreateEngine(int width, int height, int x = 0, int y = 0, std::string windowName = "Salmon Engine App", std::string logFileName = "log.txt");
void MainLoop(TApplication* app);
void DestroyEngine();


//APPTYPE must be inherited from TApplication
template<typename APPTYPE>
void OuterMainLoop(int width, int height)
{
	
	if (CreateEngine(width, height))
	{
		TApplication* app = new APPTYPE;		
		
		app->OuterInit(width, height, static_cast<float>(width), static_cast<float>(height));

		MainLoop(app);
			
		app->OuterDeinit();

		DestroyEngine();
	}
}


} //namespace SE

//This file includes templates that call any of three singletones: Console, ResourceManager or Renderer

#include "include/GUIManager/WidgetTemplatesImpl.h"
#include "include/Utils/ThreadUtilsImpl.h"


