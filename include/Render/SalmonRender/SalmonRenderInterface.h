#ifndef SALMON_RENDER_INTERFACE_H_INCLUDED
#define SALMON_RENDER_INTERFACE_H_INCLUDED

#include "include/Utils/Utils.h"

#include "include/SimpleLand/SimpleLand.h"

#include "include/Render/RenderMisc.h"

#include "include/Render/SalmonRender/BackgroundCubemap.h"

#include "include/Render/RenderInterface.h"

namespace SE
{

struct TCameraInterface
{
	vec3 CamVec;

	vec3 CamShift;

	TCameraInterface();

	virtual ~TCameraInterface()
	{
	}

};

struct TPanoramicCamera : public TCameraInterface
{

	float CamAlpha;
	float CamPhi;
	float CamDist;

	TPanoramicCamera();

	void MoveAlpha(float dAlpha);
	void MovePhi(float dPhi);
	void MoveDist(float dDist);
	void SetAlpha(float alpha);

	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();

	void CalcCamVec();

	void SetCamView();
};

struct TPitCamera : public TCameraInterface
{
	vec4 CameraQuat;
	
	void SetCamView();

	void CalcCamVec();

	void RotateByQuat(vec4 quat);
};


typedef boost::variant<TPanoramicCamera, TPitCamera> TCamera;


struct TSetCameraViewVisitor : public boost::static_visitor<void>
{
	void operator()(TPanoramicCamera& panoramicCamera)
    {
        panoramicCamera.SetCamView();
    }
    
    void operator()(TPitCamera& pitCamera)
    {
        pitCamera.SetCamView();
    }
};


struct TCalcCamVecVisitor : public boost::static_visitor<void>
{
	void operator()(TPanoramicCamera& panoramicCamera)
    {
        panoramicCamera.CalcCamVec();
    }
    
    void operator()(TPitCamera& pitCamera)
    {
        pitCamera.CalcCamVec();
    }
};


struct TGetCamVecVisitor : public boost::static_visitor<vec3&>
{
	vec3& operator()(TPanoramicCamera& panoramicCamera) const
    {
        return panoramicCamera.CamVec;
    }
    
    vec3& operator()(TPitCamera& pitCamera) const
    {
        return pitCamera.CamVec;
    }
};

struct TGetCamShiftVisitor : public boost::static_visitor<vec3&>
{
	vec3& operator()(TPanoramicCamera& panoramicCamera) const
    {
        return panoramicCamera.CamShift;
    }
    
    vec3& operator()(TPitCamera& pitCamera) const
    {
        return pitCamera.CamShift;
    }
};


class TSalmonRendererInterface : public TRendererInterface
{
protected:
	
	/*
	float CamAlpha;
	float CamPhi;
	float CamDist;
	vec3 CamShift;*/

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