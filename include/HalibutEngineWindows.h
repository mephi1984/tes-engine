#pragma once //Windows ok


#include "include/Render/HalibutRender/HalibutRenderWindows.h"

#include "include/HalibutEngineInterface.h"

namespace SE
{

//==============================================================
//========= GLOBAL VARIABLES - REMEMBER THIS LIST!!! ===========
//==============================================================

extern HGLRC hRC;		//Render context
extern HWND  Hwnd;		//Main window handle
extern HDC hDC;			//Device context

extern TFileConsole* Console;
//Console and log for output

extern THalibutRenderer* Renderer;
//OpenGL renderer

extern THalibutResourceManager* ResourceManager;
//Resource Manager



class TApplication : public TApplicationAncestor
{
protected:
	std::string LogFilename; //Log file name
	unsigned char KeyboardBuffer[256];
public:
	int X, Y, Width, Height; //Window position and size
	float MatrixWidth, MatrixHeight; 
	
	std::string WindowName;  //Window name

	TApplication();
	virtual ~TApplication();

	const std::string& GetLogFilename();

	char GetKeyState(cardinal keyNum);

	void SetKeyboardBuffer(const unsigned char* source);

	virtual void OnKeyPress(cardinal key); // If you override this, call ancestor!


};


int MainLoop(TApplication& application);

} //namespace SE

//This file includes templates that call any of three singletones: Console, ResourceManager or Renderer

#include "include/GUIManager/WidgetTemplatesImpl.h"

