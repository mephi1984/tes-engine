#include "include/SalmonEngineAndroid.h"

#include "include/Utils/Utils.h"

namespace SE
{

void TApplication::OuterInit(int screenWidth, int screenHeight, float matrixWidth, float matrixHeight)
{
	OuterDeinit();
    
    Console = new TJavaConsole;
	
	*Console<<std::string("Console successfully started!!!");
	
    ResourceManager = new TResourceManager;

	Renderer = new TSalmonRendererAndroid;

	ResourceManager->ScriptManager.BindBasicFunctions();

	InnerInit();
	
	Renderer->InitOpenGL(screenWidth, screenHeight, matrixWidth, matrixHeight);
	CheckGlError();
}


void TApplication::OuterDeinit()
{

	if (Console != NULL)
	{
	*Console<<"OuterDeinit";
    }
	
	InnerDeinit();

	if (ResourceManager != NULL)
    {
	   delete ResourceManager;
	   ResourceManager = NULL;
    }
	
	if (Console != NULL)
	{
	*Console<<"Resource manager deleted, deleting salmon render";
	}
	
    if (Renderer != NULL)
    {
	   delete Renderer;
	   Renderer = NULL;
    }
	if (Console != NULL)
	{
	*Console<<"salmon render deleted";
	}
    
    
    
    if (Console != NULL)
    {
	   delete Console;
	   Console = NULL;
    }
	
}


void TApplication::OuterDraw()
{
	InnerDraw();
}

void TApplication::OuterUpdate(cardinal timer)
{
	InnerUpdate(timer);
}


} //namespace SE