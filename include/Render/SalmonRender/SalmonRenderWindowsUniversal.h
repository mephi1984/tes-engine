#pragma once

#include "include/Render/SalmonRender/SalmonRenderGLES20.h"

namespace SE
{

class TSalmonRendererWindowsUniversal : public TSalmonRendererGLES20
{
public:
	void DrawTriangleList(const TTriangleList& triangleList);
};


} //namespace SE
