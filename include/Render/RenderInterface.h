#ifndef RENDER_INTERFACE_H_INCLUDED
#define RENDER_INTERFACE_H_INCLUDED


#include "include/Utils/Utils.h"
#include "include/Render/RenderMisc.h"
#include "include/Render/RenderParams.h"

namespace SE
{

class TRendererInterface
{
protected:
	int ScreenWidth;
	int ScreenHeight;

	float MatrixWidth;
	float MatrixHeight;

	mat4 ProjectionModelviewMatrix;
	
	std::stack<mat4> ProjectionMatrixStack;
	std::stack<mat4> ModelviewMatrixStack;

	virtual void TryEnableVertexAttribArrays();
	virtual void TryDisableVertexAttribArrays();
	virtual void DrawQuad(const T2DQuad& quad) = 0;
public:
	TRendererInterface();
	virtual ~TRendererInterface() { }

	virtual void InitOpenGL(int screenWidth, int screenHeight, float matrixWidth, float matrixHeight) = 0;

	virtual int GetScreenWidth();
	virtual int GetScreenHeight();

	virtual float GetMatrixWidth();
	virtual float GetMatrixHeight();

	virtual void SetMatrixWidth(float matrixWidth);
	virtual void SetMatrixHeight(float matrixHeight);

	virtual void SetScreenWidthHeight(int screenWidth, int screenHeight);

	virtual void SetUniforms() = 0;

	virtual void PushMatrix();
	virtual void LoadIdentity();
	virtual void TranslateMatrix(const vec3& p);
	virtual void ScaleMatrix(float scale);
	virtual void ScaleMatrix(const vec3& scale);
	virtual void RotateMatrix(const vec4& q);
	virtual void PushSpecialMatrix(const mat4& m);
    virtual void PopMatrix();

	virtual void SetProjectionMatrix(float width, float height);
    virtual void PushProjectionMatrix(float width, float height);
    virtual void PopProjectionMatrix();

	virtual void SetFrameViewport(const std::string& frameName);
	virtual void SetFullScreenViewport();

	virtual void PushShader(const std::string& shaderName);
	virtual void PopShader();

	virtual void DrawRect(const vec2& p1, const vec2& p2);
	virtual void DrawRect(const vec2& p1, const vec2& p2, const vec2& t1, const vec2& t2);
	virtual void DrawTriangleList(const TTriangleList& triangleList) = 0;

	virtual void SwitchToScreen() = 0;
	virtual void SwitchToFrameBuffer(const std::string& frameName) = 0;

	void DrawFrameFullScreen(const std::string& frameName);
	void DrawFramePartScreen(const std::string& frameName, vec2 posFrom, vec2 posTo); //To draw water. posFrom and posTo goes from 0 to 1

};

} //namespace SE

#endif