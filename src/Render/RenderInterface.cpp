#include "include/Render/RenderInterface.h"
#include "include/Engine.h"

namespace SE
{

TRendererInterface::TRendererInterface()
	: ScreenWidth(0)
	, ScreenHeight(0)
{
}

void TRendererInterface::TryEnableVertexAttribArrays()
{
	AssertIfInMainThread();

	EnableVertexAttribArray(CONST_STRING_POSITION_ATTRIB);
	EnableVertexAttribArray(CONST_STRING_NORMAL_ATTRIB);
	EnableVertexAttribArray(CONST_STRING_TEXCOORD_ATTRIB);
	EnableVertexAttribArray(CONST_STRING_TANGENT_ATTRIB);
	EnableVertexAttribArray(CONST_STRING_BINORMAL_ATTRIB);
}

void TRendererInterface::TryDisableVertexAttribArrays()
{
	AssertIfInMainThread();

	DisableVertexAttribArray(CONST_STRING_BINORMAL_ATTRIB);
	DisableVertexAttribArray(CONST_STRING_TANGENT_ATTRIB);	
	DisableVertexAttribArray(CONST_STRING_TEXCOORD_ATTRIB);
	DisableVertexAttribArray(CONST_STRING_NORMAL_ATTRIB);
	DisableVertexAttribArray(CONST_STRING_POSITION_ATTRIB);
}


int TRendererInterface::GetScreenWidth() 
{ 
	return ScreenWidth; 
}

int TRendererInterface::GetScreenHeight() 
{ 
	return ScreenHeight; 
}

float TRendererInterface::GetMatrixWidth()
{
	return MatrixWidth;
}


float TRendererInterface::GetMatrixHeight()
{
	return MatrixHeight;
}

void TRendererInterface::SetMatrixWidth(float matrixWidth)
{
	MatrixWidth = matrixWidth;
}

void TRendererInterface::SetMatrixHeight(float matrixHeight)
{
	MatrixHeight = matrixHeight;
}



void TRendererInterface::SetScreenWidthHeight(int screenWidth, int screenHeight) 
{ 
	ScreenWidth = screenWidth; 
	ScreenHeight = screenHeight; 
}

void TRendererInterface::PushMatrix()
{
	if (ModelviewMatrixStack.size() == 0)
	{
		throw ErrorToLog("Modelview matrix stack underflow!!!!");
	}

    ModelviewMatrixStack.push(ModelviewMatrixStack.top());
    
    if (ModelviewMatrixStack.size() > 64)
    {
        throw ErrorToLog("Modelview matrix stack overflow!!!!");
    }
}

void TRendererInterface::LoadIdentity()
{
	if (ModelviewMatrixStack.size() == 0)
	{
		throw ErrorToLog("Modelview matrix stack underflow!!!!");
	}

    ModelviewMatrixStack.pop();
    ModelviewMatrixStack.push(IdentityMatrix4);
    
    SetUniforms();
}

void TRendererInterface::TranslateMatrix(const vec3& p)
{
    mat4 m = IdentityMatrix4;
    m.m[12] = p.v[0];
    m.m[13] = p.v[1];
    m.m[14] = p.v[2];
    m = MultMatrixMatrix(ModelviewMatrixStack.top(), m);

	if (ModelviewMatrixStack.size() == 0)
	{
		throw ErrorToLog("Modelview matrix stack underflow!!!!");
	}

    ModelviewMatrixStack.pop();
    ModelviewMatrixStack.push(m);
    
    SetUniforms();
}

void TRendererInterface::ScaleMatrix(float scale)
{
    mat4 m = IdentityMatrix4;
    m.m[0] = scale;
    m.m[5] = scale;
    m.m[10] = scale;
    m = MultMatrixMatrix(ModelviewMatrixStack.top(), m);

	if (ModelviewMatrixStack.size() == 0)
	{
		throw ErrorToLog("Modelview matrix stack underflow!!!!");
	}

    ModelviewMatrixStack.pop();
    ModelviewMatrixStack.push(m);
    
    SetUniforms();
}

void TRendererInterface::ScaleMatrix(const vec3& scale)
{
	mat4 m = IdentityMatrix4;
    m.m[0] = scale.v[0];
    m.m[5] = scale.v[1];
    m.m[10] = scale.v[2];
    m = MultMatrixMatrix(ModelviewMatrixStack.top(), m);

	if (ModelviewMatrixStack.size() == 0)
	{
		throw ErrorToLog("Modelview matrix stack underflow!!!!");
	}

    ModelviewMatrixStack.pop();
    ModelviewMatrixStack.push(m);
    
    SetUniforms();
}

void TRendererInterface::RotateMatrix(const vec4& q)
{
    mat3 m3(q);
    mat4 m = IdentityMatrix4;
    m.m[0] = m3.m[0];
    m.m[1] = m3.m[1];
    m.m[2] = m3.m[2];
    
    m.m[4] = m3.m[3];
    m.m[5] = m3.m[4];
    m.m[6] = m3.m[5];
    
    m.m[8] = m3.m[6];
    m.m[9] = m3.m[7];
    m.m[10] = m3.m[8];
    
    m = MultMatrixMatrix(ModelviewMatrixStack.top(), m);

	if (ModelviewMatrixStack.size() == 0)
	{
		throw ErrorToLog("Modelview matrix stack underflow!!!!");
	}

    ModelviewMatrixStack.pop();
    ModelviewMatrixStack.push(m);
    
    SetUniforms();
}

void TRendererInterface::PushSpecialMatrix(const mat4& m)
{
	if (ModelviewMatrixStack.size() > 64)
    {
        throw ErrorToLog("Modelview matrix stack overflow!!!!");
    }
    ModelviewMatrixStack.push(m);
    SetUniforms();
}


void TRendererInterface::PopMatrix()
{
	if (ModelviewMatrixStack.size() == 0)
	{
		throw ErrorToLog("Modelview matrix stack underflow!!!!");
	}
    ModelviewMatrixStack.pop();
    
    SetUniforms();
}

void TRendererInterface::PushProjectionMatrix(float width, float height)
{
    mat4 m = MakeOrthoMatrix(width, height);
    ProjectionMatrixStack.push(m);
    SetUniforms();
    
    if (ProjectionMatrixStack.size() > 64)
    {
        throw ErrorToLog("Projection matrix stack overflow!!!!");
    }
}

void TRendererInterface::PopProjectionMatrix()
{
	if (ProjectionMatrixStack.size() == 0)
	{
		throw ErrorToLog("Projection matrix stack underflow!!!!");
	}
    ProjectionMatrixStack.pop();
    SetUniforms();
}

void TRendererInterface::SetProjectionMatrix(float width, float height)
{
    mat4 m = MakeOrthoMatrix(width, height);
	if (ProjectionMatrixStack.size() == 0)
	{
		throw ErrorToLog("Projection matrix stack underflow!!!!");
	}
    ProjectionMatrixStack.pop();
    ProjectionMatrixStack.push(m);
    SetUniforms();

}

void TRendererInterface::SetFrameViewport(const std::string& frameName)
{
	AssertIfInMainThread();

	ivec2 frameWidthHeight = ResourceManager->FrameManager.GetFrameWidthHeight(frameName);
	glViewport(0, 0, frameWidthHeight.v[0], frameWidthHeight.v[1]);
}

void TRendererInterface::SetFullScreenViewport()
{
	AssertIfInMainThread();

	glViewport(0, 0, ScreenWidth, ScreenHeight);
}

void TRendererInterface::PushShader(const std::string& shaderName)
{

	ResourceManager->ShaderManager.PushShader(shaderName);
	SetUniforms();
	TryEnableVertexAttribArrays();
}

void TRendererInterface::PopShader()
{

	ResourceManager->ShaderManager.PopShader();
	SetUniforms();
	TryEnableVertexAttribArrays();

}


void TRendererInterface::DrawRect(const vec2& p1, const vec2& p2)
{

	T2DQuad quad;
	
	quad.VertexCoord[0] = vec3(p1.v[0], p1.v[1], 0.0f);
	quad.VertexCoord[1] = vec3(p1.v[0], p2.v[1], 0.0f);
	quad.VertexCoord[2] = vec3(p2.v[0], p1.v[1], 0.0f);
	quad.VertexCoord[3] = vec3(p2.v[0], p2.v[1], 0.0f);

	quad.TextureCoord[0] = vec2(0.01f, 0.01f);
	quad.TextureCoord[1] = vec2(0.01f, 0.99f);
	quad.TextureCoord[2] = vec2(0.99f, 0.01f);
	quad.TextureCoord[3] = vec2(0.99f, 0.99f);

	DrawQuad(quad);
}

void TRendererInterface::DrawRect(const vec2& p1, const vec2& p2, const vec2& t1, const vec2& t2)
{

	T2DQuad quad;
	
	quad.VertexCoord[0] = vec3(p1.v[0], p1.v[1], 0.0f);
	quad.VertexCoord[1] = vec3(p1.v[0], p2.v[1], 0.0f);
	quad.VertexCoord[2] = vec3(p2.v[0], p1.v[1], 0.0f);
	quad.VertexCoord[3] = vec3(p2.v[0], p2.v[1], 0.0f);

	quad.TextureCoord[0] = vec2(t1.v[0], t1.v[1]);
	quad.TextureCoord[1] = vec2(t1.v[0], t2.v[1]);
	quad.TextureCoord[2] = vec2(t2.v[0], t1.v[1]);
	quad.TextureCoord[3] = vec2(t2.v[0], t2.v[1]);

	DrawQuad(quad);
}


void TRendererInterface::DrawFrameFullScreen(const std::string& frameName)
{

	DrawFramePartScreen(frameName, vec2(0, 0), vec2(1, 1));
}

void TRendererInterface::DrawFramePartScreen(const std::string& frameName, vec2 posFrom, vec2 posTo)
{
	AssertIfInMainThread();

	cardinal texID = ResourceManager->FrameManager.GetFrameTexture(frameName.c_str());

	if (texID != 0)
	{
		PushProjectionMatrix(1,1);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		LoadIdentity();
		glBindTexture(GL_TEXTURE_2D,texID);

		DrawRect(posFrom, posTo, posFrom, posTo);

		PopProjectionMatrix();

	}

}

} //namespace SE