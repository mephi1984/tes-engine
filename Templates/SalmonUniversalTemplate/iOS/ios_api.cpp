#include "include/Engine.h"
#include "main_code.h"

TMyApplication* App;


extern "C" void AppInit()
{
    CreateEngine();
    App = new TMyApplication;
    App->OuterInit(480, 320, 480, 320);
    
}

extern "C" void AppDeinit()
{
    
    App->OuterDeinit();
    delete App;
    DestroyEngine();

}


extern "C" void AppUpdate(int dt)
{
    App->OuterUpdate(dt);
}



extern "C" void AppDraw()
{
    App->OuterDraw();
}

extern "C" void AppOnTapDown(int posx, int posy)
{
    App->OuterOnTapDown(vec2(posx, posy));
}

extern "C" void AppOnTapUp(int posx, int posy)
{
    App->OuterOnTapUp(vec2(posx, posy));
}

extern "C" void AppOnTapUpAfterScroll(int posx, int posy)
{
    App->OuterOnTapUpAfterShift(vec2(posx, posy));
}

extern "C" void AppOnScroll(int shiftx, int shifty)
{
    App->OuterOnMove(vec2(shiftx, shifty));
}
