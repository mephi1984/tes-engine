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
protected:
	std::string LogFilename; //Log file name
public:
	int X, Y, Width, Height; //Window position and size
	
	std::string WindowName;  //Window name

	TApplication() : X(0), Y(0), Width(800), Height(600), WindowName("Salmon Engine"), LogFilename("log.txt") { }

	const std::string& GetLogFilename() { return LogFilename; } 

	virtual void InnerInit() = 0;
	//To do on init

	virtual void InnerDeinit() = 0;
	//To do on deinit

	virtual void InnerDraw() = 0;
	//What to draw

	virtual void InnerUpdate(cardinal timer) = 0;
	//To do on update

	virtual void UpdateQuick() { };
	//To process input - this method is called more frequently than Update()  


	virtual void OnMouseWheel(short int delta) { }
	//To do on mouse wheel move

	virtual void OnKeyPress(cardinal key); //Try not to override this. But if you need to override, call ancestor!
};

int MainLoop(TApplication& application);

} //namespace SE