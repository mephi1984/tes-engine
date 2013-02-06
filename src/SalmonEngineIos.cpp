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
    
    
    void AppOnTapDown(int posx, int posy)
    {
        App->OuterOnTapDown(vec2(posx, posy));
    }
    
    
    void AppOnTapUp(int posx, int posy)
    {
        App->OuterOnTapUp(vec2(posx, posy));
    }
    
    
    void AppOnTapUpAfterMove(int posx, int posy)
    {
        App->OuterOnTapUpAfterMove(vec2(posx, posy));
    }
    
    
    void AppOnScroll(int shiftx, int shifty)
    {
        App->OuterOnMove(vec2(shiftx, shifty));
    }
    
    
    void AppOnScale(float scale)
    {
        App->OnScale(scale);
    }
    
    

} //namespace SE
    
   