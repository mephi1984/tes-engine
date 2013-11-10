#include "include/SalmonEngineIos.h"

#include "include/Utils/Utils.h"


namespace SE
{

void CreateEngine()
{
	DestroyEngine();
	
	Console = new TIosConsole;
	
	*Console<<std::string("Console successfully started!!!");
	
    ResourceManager = new TResourceManager;
	
	Renderer = new TSalmonRendererIos;
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
    
    
    TApplication* App = NULL;
 
    void AppDeinit()
    {
        App->OuterDeinit();
        delete App;
        App = NULL;
        DestroyEngine();
    }
    
    
    void AppUpdate(int dt)
    {
        App->OuterUpdate(dt);
    }
    
    
    void AppDraw()
    {
        App->OuterDraw();
    }
    
    
    void AppOnTapDown(int posx, int posy, int touchNumber)
    {
        App->OuterOnTapDown(vec2(posx, posy), touchNumber);
    }
    
    
    void AppOnTapUp(int posx, int posy, int touchNumber)
    {
        App->OuterOnTapUp(vec2(posx, posy), touchNumber);
    }
    
    
    void AppOnTapUpAfterMove(int posx, int posy, int touchNumber)
    {
        App->OuterOnTapUpAfterMove(vec2(posx, posy), touchNumber);
    }
    
    
    void AppOnScroll(int shiftx, int shifty, int touchNumber)
    {
        App->OuterOnMove(vec2(shiftx, shifty), touchNumber);
    }
    
    
    void AppOnScale(float scale)
    {
        App->OnScale(scale);
    }
    
    

} //namespace SE
    
   