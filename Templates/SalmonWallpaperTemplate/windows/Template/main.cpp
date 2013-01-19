#include "main.h"


int APIENTRY WinMain(HINSTANCE hCurrentInst, HINSTANCE hPreviousInst,
	LPSTR lpszCmdLine, int nCmdShow)
{
	//Create application
	TAndroidApplication Application;

	Application.Width = 800;
	Application.Height = 480;

	//Start application
	return MainLoop(Application);
}