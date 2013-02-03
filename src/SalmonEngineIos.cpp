#include "include/SalmonEngineIos.h"

#include "include/Utils/Utils.h"


namespace SE
{

void TApplication::OuterInit(int screenWidth, int screenHeight, float matrixWidth, float matrixHeight)
{
    
	OuterDeinit();
    
    Console = new TIosConsole;
	
	*Console<<std::string("Console successfully started!!!");
	
    ResourceManager = new TResourceManager;
    
    ResourceManager->MainThreadId = boost::this_thread::get_id();

	Renderer = new TSalmonRendererIos;

	ResourceManager->ScriptManager.BindBasicFunctions();

    Renderer->InitOpenGL(screenWidth, screenHeight, matrixWidth, matrixHeight);
    
	InnerInit();
	
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

/*
void TApplication::OuterDraw()
{
	InnerDraw();
}

void TApplication::OuterUpdate(cardinal timer)
{
	InnerUpdate(timer);
}*/


} //namespace SE