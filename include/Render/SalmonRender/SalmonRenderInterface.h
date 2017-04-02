#ifndef SALMON_RENDER_INTERFACE_H_INCLUDED
#define SALMON_RENDER_INTERFACE_H_INCLUDED

#include "include/Utils/Utils.h"

#include "include/Render/RenderMisc.h"

#include "include/Render/SalmonRender/Cameras.h"

namespace SE
{



class TSalmonRendererInterface
{
protected:

	int ScreenWidth;
	int ScreenHeight;

	float MatrixWidth;
	float MatrixHeight;

	Matrix4f ProjectionModelviewMatrix;
	
	std::stack<Matrix4f> ProjectionMatrixStack;
	std::stack<Matrix4f> ModelviewMatrixStack;
	
	Matrix4f CamModelViewMatrix;
	Matrix4f CamInversedModelViewMatrix;

	float GlobalShadowAreaHalfSize;

	Vector3f CamPos;  //Do not change - call CalcCamPos instead


	virtual void TryEnableVertexAttribArrays();
	virtual void TryDisableVertexAttribArrays();
	virtual void DrawQuad(const T2DQuad& quad) = 0;

public:

	TCamera Camera;

	TSalmonRendererInterface();
	
	virtual void InitOpenGL(int screenWidth, int screenHeight, float matrixWidth, float matrixHeight);

	Vector3f GetCamShift();

	Vector3f GetCamVec();

	Vector3f GetCamPos();

	void CalcCamPos();

	virtual void SetUniforms();

	virtual int GetScreenWidth();
	virtual int GetScreenHeight();

	virtual float GetMatrixWidth();
	virtual float GetMatrixHeight();

	virtual void SetMatrixWidth(float matrixWidth);
	virtual void SetMatrixHeight(float matrixHeight);

	virtual void PushMatrix();
	virtual void LoadIdentity();
	virtual void TranslateMatrix(const Vector3f& p);
	virtual void ScaleMatrix(float scale);
	virtual void ScaleMatrix(const Vector3f& scale);
	virtual void RotateMatrix(const Vector4f& q);
	virtual void PushSpecialMatrix(const Matrix4f& m);
    virtual void PopMatrix();

	virtual void SetProjectionMatrix(float width, float height);
    virtual void PushProjectionMatrix(float width, float height);
	virtual void PushProjectionMatrix(const Matrix4f& m);
    virtual void PopProjectionMatrix();

	virtual void SetFrameViewport(const std::string& frameName);
	virtual void SetFullScreenViewport();

	void SetPerspectiveFullScreenViewport();
	void SetOrthoFullScreenViewport();

	void SetPerspectiveProjection(float angle = pi/6.f, float zNear = 1.f, float zFar = 100.f);
	void SetOrthoProjection();
	void PushOrthoProjection();

	Matrix4f GetModelviewMatrix();

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


	virtual void SwitchToScreen();
	virtual void SwitchToFrameBuffer(const std::string& frameName);
	void SwitchToCubemapBuffer(const std::string& frameName,size_t cubeSide);
	
	void BeginDrawToDepthBufferGlobal(std::string& globalBufferName);
	void BeginDrawToDepthBufferLocal(std::string& localBufferName);
	void EndDrawToDepthBuffer();

	virtual void PushShader(const std::string& shaderName);
	virtual void PopShader();

	virtual void DrawRect(const Vector2f& p1, const Vector2f& p2);
	virtual void DrawRect(const Vector2f& p1, const Vector2f& p2, const Vector2f& t1, const Vector2f& t2);
	virtual void DrawTriangleList(const TTriangleList& triangleList) = 0;

	void DrawFrameFullScreen(const std::string& frameName);
	void DrawFramePartScreen(const std::string& frameName, Vector2f posFrom, Vector2f posTo); //To draw water. posFrom and posTo goes from 0 to 1
	
};

} //namespace SE

#endif