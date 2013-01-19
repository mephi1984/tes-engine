#ifndef SALMON_RENDER_GLES20_H_INCLUDED
#define SALMON_RENDER_GLES20_H_INCLUDED

#include "include/Render/SalmonRender/SalmonRenderInterface.h"

#ifdef TARGET_ANDROID
#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

namespace SE
{

class TSalmonRendererGLES20 : public TSalmonRendererInterface
{
protected:

	virtual void DrawQuad(const T2DQuad& quad);
public:

	void DrawTriangleList(const TTriangleList& triangleList);

	
};

} //namespace SE

#endif