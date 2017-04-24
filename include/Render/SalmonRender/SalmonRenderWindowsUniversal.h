#pragma once

#include "include/Render/SalmonRender/SalmonRenderGLES20.h"

namespace SE
{

class TSalmonRendererWindowsUniversal : public TSalmonRendererGLES20
{
public:
	void DrawTriangleList(const TTriangleList& triangleList);
	void DrawTriangleList(const TTriangleList& triangleList, GLenum mode);
};


} //namespace SE
