#ifndef SALMON_RENDER_IOS_H_INCLUDED
#define SALMON_RENDER_IOS_H_INCLUDED

#include "include/Render/SalmonRender/SalmonRenderGLES20.h"

namespace SE
{

class TSalmonRendererIos : public TSalmonRendererGLES20
{
protected:
    
    GLint defaultFBO;

public:
    virtual void InitOpenGL(int screenWidth, int screenHeight, float matrixWidth, float matrixHeight);

    virtual void SwitchToScreen();
};

} //namespace SE

#endif
