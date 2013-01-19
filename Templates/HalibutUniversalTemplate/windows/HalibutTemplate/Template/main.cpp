#include "main.h"

#include "main_code.h"

int APIENTRY WinMain(HINSTANCE hCurrentInst, HINSTANCE hPreviousInst,
	LPSTR lpszCmdLine, int nCmdShow)
{
	//Create application
	TMyApplication Application;

	Application.Width = 480;
	Application.Height = 320;

	//Start application
	return MainLoop(Application);
}