#pragma once

/*
This code is the heart of engine. Here is the render and resource manager
*/


#include "include/Utils/Utils.h"

#include "include/Render/SalmonRender/SalmonRenderInterface.h"


namespace SE
{

class TSalmonRenderer : public TSalmonRendererInterface
{
protected:

	bool IsCameraTransparentToLand; //To make it work you must setup LandToCalcCollision

	float FogBeginDistance;
	float FogEndDistance;
	vec4 FogColor;


	//Other data:

	float ShadowClampValue;

	virtual void DrawQuad(const T2DQuad& quad);

public:
	TSalmonRenderer();
	
	~TSalmonRenderer();

	bool BindOpenGLFunctions();

	float GetShadowClampValue();
	void SetShadowClampValue(float shadowClampValue);
	
	float GetFogBeginDistance();
	float GetFogEndDistance();
	vec4 GetFogColor();

	void DrawTriangleList(const TTriangleList& triangleList);
	

};

} //namespace SE