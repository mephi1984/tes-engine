#include "include/Render/SalmonRender/SalmonRenderIos.h"

#include "include/Utils/Utils.h"
#include "include/Engine.h"

namespace SE
{
    
    void TSalmonRendererIos::SwitchToScreen()
    {
		AssertIfInMainThread();
	
        IosSwitchToScreen();
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

} //namespace SE