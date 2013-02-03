#include "include/Engine.h"


namespace SE
{

TApplicationInterface::TApplicationInterface()
	: IsConsoleOut(false) 
{
}

void TApplicationInterface::OuterInit(int screenWidth, int screenHeight, float matrixWidth, float matrixHeight)
{
	ResourceManager->MainThreadId = boost::this_thread::get_id();

	ResourceManager->ScriptManager.BindBasicFunctions();

	Renderer->InitOpenGL(screenWidth, screenHeight, matrixWidth, matrixHeight);

	InnerInit();
    
    CheckGlError();
    
    srand(static_cast<unsigned int>(time(0)));
}

void TApplicationInterface::OuterDeinit()
{
	CheckGlError("OuterDeinit");
	InnerDeinit();
}


} //namespace SE