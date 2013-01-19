#ifndef SALMON_RENDER_INTERFACE_H_INCLUDED
#define SALMON_RENDER_INTERFACE_H_INCLUDED

#include "include/Utils/Utils.h"

#include "include/SimpleLand/SimpleLand.h"

#include "include/Render/RenderMisc.h"

#include "include/Render/SalmonRender/BackgroundCubemap.h"

#include "include/Render/RenderInterface.h"

namespace SE
{

class TSalmonRendererInterface : public TRendererInterface
{
protected:
	
	float CamAlpha;
	float CamPhi;
	float CamDist;
	vec3 CamShift;
	vec3 CamPos;  //Do not change - call CalcCamPosVec instead
	vec3 CamVec;  //Do not change - call CalcCamPosVec instead

	mat4 CamModelViewMatrix;
	mat4 CamInversedModelViewMatrix;

	float GlobalShadowAreaHalfSize;

	void CalcCamPosVec();

	virtual void DrawQuad(const T2DQuad& quad) = 0;
public:

	TSalmonRendererInterface();
	
	virtual void InitOpenGL(int screenWidth, int screenHeight, float matrixWidth, float matrixHeight);

	virtual void SetUniforms();

	void SetPerspectiveFullScreenViewport();
	void SetOrthoFullScreenViewport();

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


	void MoveAlpha(float dAlpha);
	void MovePhi(float dPhi);
	void MoveDist(float dDist);

	void SetAlpha(float alpha);
	vec3 GetCamPos();

	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();

	virtual void SwitchToScreen();
	virtual void SwitchToFrameBuffer(const std::string& frameName);
	void SwitchToCubemapBuffer(const std::string& frameName,cardinal cubeSide);
	
	void BeginDrawToDepthBufferGlobal(std::string& globalBufferName);
	void BeginDrawToDepthBufferLocal(std::string& localBufferName);
	void EndDrawToDepthBuffer();
	
};

} //namespace SE

#endif