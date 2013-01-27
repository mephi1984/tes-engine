#ifndef SALMON_RENDER_INTERFACE_H_INCLUDED
#define SALMON_RENDER_INTERFACE_H_INCLUDED

#include "include/Utils/Utils.h"

#include "include/SimpleLand/SimpleLand.h"

#include "include/Render/RenderMisc.h"

#include "include/Render/SalmonRender/BackgroundCubemap.h"

#include "include/Render/RenderInterface.h"

#include "include/Render/SalmonRender/Cameras.h"

namespace SE
{



class TSalmonRendererInterface : public TRendererInterface
{
protected:
	
	mat4 CamModelViewMatrix;
	mat4 CamInversedModelViewMatrix;

	float GlobalShadowAreaHalfSize;

	virtual void DrawQuad(const T2DQuad& quad) = 0;
public:

	TCamera CameraMover;

	//vec3 CamShift;
	vec3 CamPos;  //Do not change - call CalcCamPosVec instead
	//vec3 CamVec;  //Do not change - call CalcCamPosVec instead

	vec3 GetCamShift()
	{
		return boost::apply_visitor(TGetCamShiftVisitor(), CameraMover);
	}

	vec3 GetCamVec()
	{
		return boost::apply_visitor(TGetCamVecVisitor(), CameraMover);
	}

	TSalmonRendererInterface();
	
	virtual void InitOpenGL(int screenWidth, int screenHeight, float matrixWidth, float matrixHeight);

	void CalcCamPosVec();

	virtual void SetUniforms();

	void SetPerspectiveFullScreenViewport();
	void SetOrthoFullScreenViewport();

	void SetPerspectiveProjection(float angle = pi/6.f, float zNear = 1.f, float zFar = 100.f);
	void SetOrthoProjection();

	mat4 GetModelviewMatrix();

	void SetPerspectiveProjectionMatrix(float angle, float aspect, float zNear, float zFar);
    void PushPerspectiveProjectionMatrix(float angle, float aspect, float zNear, float zFar);

	void SetGLCamView();
	void SetGlIdentityView();
	void SetGlPosXView();
	void SetGlNegXView();
	void SetGlPosYView();
	void SetGlNegYView();
	void SetGlPosZView();
	void SetGlNegZView();


	vec3 GetCamPos();

	virtual void SwitchToScreen();
	virtual void SwitchToFrameBuffer(const std::string& frameName);
	void SwitchToCubemapBuffer(const std::string& frameName,cardinal cubeSide);
	
	void BeginDrawToDepthBufferGlobal(std::string& globalBufferName);
	void BeginDrawToDepthBufferLocal(std::string& localBufferName);
	void EndDrawToDepthBuffer();
	
};

} //namespace SE

#endif