#ifndef HALIBUT_RENDER_GLES20_H_INCLUDED
#define HALIBUT_RENDER_GLES20_H_INCLUDED

#include "include/Utils/Utils.h"

#include "include/Render/HalibutRender/HalibutRenderInterface.h"
#include "include/Render/RenderMisc.h"

namespace SE
{

class THalibutRendererGLES20 : public THalibutRendererInterface
{
protected:

	void DrawQuad(const T2DQuad& quad);

public:
	THalibutRendererGLES20();
	~THalibutRendererGLES20();

	virtual void DrawTriangleList(const TTriangleList& triangleList);
		
};

} //namespace SE

#endif
