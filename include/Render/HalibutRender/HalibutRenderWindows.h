#pragma once

#include "include/Utils/Utils.h"
#include "include/Render/HalibutRender/HalibutRenderInterface.h"
#include "include/Render/RenderMisc.h"

#include "boost/shared_ptr.hpp"
#include "include/OpenGLExt/OpenGlExt.h"

namespace SE
{

class THalibutRenderer : public THalibutRendererInterface
{
protected:


	void DrawTriangleStripImmediate(const TTriangleList& triangleList);
	void DrawQuad(const T2DQuad& quad);
public:
	THalibutRenderer();
	~THalibutRenderer();

	bool BindOpenGLFunctions(); //Must be called once

	virtual void DrawTriangleList(const TTriangleList& triangleList);

};

} //namespace SE