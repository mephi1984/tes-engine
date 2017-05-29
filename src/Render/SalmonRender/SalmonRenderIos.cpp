#include "include/Render/SalmonRender/SalmonRenderIos.h"

#include "include/Utils/Utils.h"
#include "include/Engine.h"


namespace SE
{
    
    void TSalmonRendererIos::InitOpenGL(int screenWidth, int screenHeight, float matrixWidth, float matrixHeight)
    {
        TSalmonRendererGLES20::InitOpenGL(screenWidth, screenHeight, matrixWidth, matrixHeight);
        
        
        //glGetIntegerv(GL_FRAMEBUFFER_BINDING_OES, &this->defaultFBO);

        
    }
    void TSalmonRendererIos::SwitchToScreen()
    {
		AssertIfInMainThread();
	
        IosSwitchToScreen();
        
        //glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

} //namespace SE
