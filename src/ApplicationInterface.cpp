#include "include/Engine.h"


namespace SE
{

TApplicationInterface::TApplicationInterface()
	: IsConsoleOut(false) 
{
}

void TApplicationInterface::OuterInit(int screenWidth, int screenHeight, float matrixWidth, float matrixHeight)
{
	ResourceManager->ScriptManager.BindBasicFunctions();
	InnerInit();
#ifdef TARGET_HALIBUT
	Renderer->InitOpenGL(screenWidth, screenHeight, matrixWidth, matrixHeight);
#endif
#ifdef TARGET_SALMON
	Renderer->InitOpenGL(screenWidth, screenHeight, matrixWidth, matrixHeight);
#endif
    
    CheckGlError();
    
    srand(static_cast<unsigned int>(time(0)));
}

void TApplicationInterface::OuterDeinit()
{
	CheckGlError("OuterDeinit");
	InnerDeinit();
}


} //namespace SE