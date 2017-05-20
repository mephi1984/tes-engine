#include "include/SalmonEngineWindowsUniversal.h"

#include "include/Utils/Utils.h"

namespace SE
{


void CreateEngine()
{
	
	Console = new TFileConsole;
	
	*Console<<std::string("Console successfully started!!!");
	
    ResourceManager = new TResourceManager;
	
	Renderer = new TSalmonRendererWindowsUniversal;
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

void TApplication::OnKeyPress(size_t key)
{
	ResourceManager->GUIManager.KeyPressedSignal(static_cast<int>(key));
	ResourceManager->newGuiManager.OnKeyPressed(static_cast<int>(key));
	InnerOnKeyPress(key);
}

} //namespace SE