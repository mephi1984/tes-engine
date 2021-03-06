#include "include/SalmonEngineAndroid.h"

#include "include/Utils/Utils.h"

namespace SE
{


void CreateEngine()
{
	
	Console = new TJavaConsole;
	
	*Console<<std::string("Console successfully started!!!");
	
    ResourceManager = new TResourceManager;
	
	Renderer = new TSalmonRendererAndroid;
}

void DestroyEngine()
{
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

} //namespace SE