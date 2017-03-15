#include "include/Render/SalmonRender/SalmonRenderInterface.h"

#include "include/Utils/Utils.h"
#include "include/Engine.h"

namespace SE
{

//============================================
//============================================
//============================================

TSalmonRendererInterface::TSalmonRendererInterface()
	: GlobalShadowAreaHalfSize(CONST_DEFAULT_GLOBAL_SHADOW_AREA_HALFSIZE)
	, ScreenWidth(0)
	, ScreenHeight(0)
{
	ProjectionMatrixStack.push(IdentityMatrix4);
	
	ModelviewMatrixStack.push(IdentityMatrix4);

}


void TSalmonRendererInterface::SetUniforms()
{
	AssertIfInMainThread();
	//Refactoring!
	

	if (ProjectionMatrixStack.size() <=0)
	{
		throw ErrorToLog("Projection matrix stack out!");
	}
	
	if (ModelviewMatrixStack.size() <=0)
	{
		throw ErrorToLog("Modelview matrix stack out!");
	}
	
	ProjectionModelviewMatrix = MultMatrixMatrix(ProjectionMatrixStack.top(), ModelviewMatrixStack.top());
	RenderUniformMatrix4fv(CONST_STRING_HALIBUT_PROJECTION_MATRIX_UNIFORM, false, ProjectionModelviewMatrix.m);

		//TODO: Make a new name =(
	RenderUniformMatrix4fv("ProjectionMatrix1", false, ProjectionMatrixStack.top().m);

	RenderUniform1i(CONST_STRING_TEXTURE_UNIFORM,0);
	RenderUniform1i(CONST_STRING_NORMALMAP_UNIFORM,1);
	RenderUniform1i(CONST_STRING_SHADOWMAPGLOBAL_UNIFORM,2);
	RenderUniform1i(CONST_STRING_SHADOWMAPLOCAL_UNIFORM,3);

	RenderUniform1i(CONST_STRING_NORMALMAPEXISTS_UNIFORM,1);
	RenderUniform1i(CONST_STRING_ENV_UNIFORM,0);

	//RenderUniform1f(CONST_STRING_SHADOWCLAMPVALUE_UNIFORM, GetShadowClampValue());
	//RenderUniform1i(CONST_STRING_ACTIVELIGHTCOUNT_UNIFORM, ResourceManager->LightManager.GetActiveLightCount());

	//RenderUniform1f(CONST_STRING_FOG_BEGIN_DISTANCE_UNIFORM, GetFogBeginDistance());
	//RenderUniform1f(CONST_STRING_FOG_END_DISTANCE_UNIFORM, GetFogEndDistance());
	//RenderUniform4fv(CONST_STRING_FOG_COLOR_UNIFORM, GetFogColor().v);

	RenderUniform3fv(CONST_STRING_CAMPOS_UNIFORM, GetCamPos().v);

	RenderUniform1f(CONST_STRING_TRANSPARENCY_UNIFORM, 1.f);
	
	RenderUniform4fv(CONST_STRING_MATERIAL_COLOR_UNIFORM, WhiteColor);
	
}


void TSalmonRendererInterface::InitOpenGL(int screenWidth, int screenHeight, float matrixWidth, float matrixHeight)
{
	AssertIfInMainThread();
	
	ModelviewMatrixStack.push(IdentityMatrix4);
	ProjectionMatrixStack.push(IdentityMatrix4);

	ScreenWidth = screenWidth;
	ScreenHeight = screenHeight;

	MatrixWidth = matrixWidth;
	MatrixHeight = matrixHeight;

	glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
	#ifdef TARGET_WIN32
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE_2D);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, WhiteColor);
	
	glPolygonMode(GL_FRONT_AND_BACK , GL_FILL);
	#endif

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	CheckGlError();

}


void TSalmonRendererInterface::TryEnableVertexAttribArrays()
{
	AssertIfInMainThread();

	EnableVertexAttribArray(CONST_STRING_POSITION_ATTRIB);
	EnableVertexAttribArray(CONST_STRING_NORMAL_ATTRIB);
	EnableVertexAttribArray(CONST_STRING_TEXCOORD_ATTRIB);
	EnableVertexAttribArray(CONST_STRING_TANGENT_ATTRIB);
	EnableVertexAttribArray(CONST_STRING_BINORMAL_ATTRIB);
}

void TSalmonRendererInterface::TryDisableVertexAttribArrays()
{
	AssertIfInMainThread();

	DisableVertexAttribArray(CONST_STRING_BINORMAL_ATTRIB);
	DisableVertexAttribArray(CONST_STRING_TANGENT_ATTRIB);	
	DisableVertexAttribArray(CONST_STRING_TEXCOORD_ATTRIB);
	DisableVertexAttribArray(CONST_STRING_NORMAL_ATTRIB);
	DisableVertexAttribArray(CONST_STRING_POSITION_ATTRIB);
}


int TSalmonRendererInterface::GetScreenWidth() 
{ 
	return ScreenWidth; 
}

int TSalmonRendererInterface::GetScreenHeight() 
{ 
	return ScreenHeight; 
}

float TSalmonRendererInterface::GetMatrixWidth()
{
	return MatrixWidth;
}


float TSalmonRendererInterface::GetMatrixHeight()
{
	return MatrixHeight;
}

void TSalmonRendererInterface::SetMatrixWidth(float matrixWidth)
{
	MatrixWidth = matrixWidth;
}

void TSalmonRendererInterface::SetMatrixHeight(float matrixHeight)
{
	MatrixHeight = matrixHeight;
}


vec3 TSalmonRendererInterface::GetCamShift() 
{ 
	return boost::apply_visitor(TGetCamShiftVisitor(), Camera);
}

vec3 TSalmonRendererInterface::GetCamVec() 
{ 
	return boost::apply_visitor(TGetCamVecVisitor(), Camera);
}

vec3 TSalmonRendererInterface::GetCamPos() 
{ 
	return CamPos; 
}

void TSalmonRendererInterface::CalcCamPos()
{
	
	TCalcCamVecVisitor v;

	boost::apply_visitor(v, Camera);

	CamPos = GetCamShift() - GetCamVec();

	RenderUniform3fv(CONST_STRING_CAMPOS_UNIFORM,(float*)&CamPos);
}

void TSalmonRendererInterface::SetPerspectiveFullScreenViewport()
{
	throw ErrorToLog("SetPerspectiveFullScreenViewport is deprecated");

	glViewport(0, 0, ScreenWidth, ScreenHeight);

	SetPerspectiveProjectionMatrix(pi / 6.f, float(MatrixWidth) / float(MatrixHeight), 1.0f, 100.0f);
}

void TSalmonRendererInterface::SetPerspectiveProjection(float angle, float zNear, float zFar)
{
	SetPerspectiveProjectionMatrix(angle, float(MatrixWidth) / float(MatrixHeight), zNear, zFar);
}


void TSalmonRendererInterface::SetOrthoFullScreenViewport()
{
	throw ErrorToLog("SetOrthoFullScreenViewport is deprecated");

	glViewport(0, 0, ScreenWidth, ScreenHeight);

	SetProjectionMatrix(static_cast<float>(MatrixWidth), static_cast<float>(MatrixHeight));
}

void TSalmonRendererInterface::SetOrthoProjection()
{
	SetProjectionMatrix(static_cast<float>(MatrixWidth), static_cast<float>(MatrixHeight));
}


mat4 TSalmonRendererInterface::GetModelviewMatrix()
{
	if (ModelviewMatrixStack.size() == 0)
	{
		throw ErrorToLog("Modelview matrix stack underflow!!!!");
	}
    return ModelviewMatrixStack.top();
}


void TSalmonRendererInterface::PushMatrix()
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

void TSalmonRendererInterface::LoadIdentity()
{
	if (ModelviewMatrixStack.size() == 0)
	{
		throw ErrorToLog("Modelview matrix stack underflow!!!!");
	}

    ModelviewMatrixStack.pop();
    ModelviewMatrixStack.push(IdentityMatrix4);
    
    SetUniforms();
}

void TSalmonRendererInterface::TranslateMatrix(const vec3& p)
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

void TSalmonRendererInterface::ScaleMatrix(float scale)
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

void TSalmonRendererInterface::ScaleMatrix(const vec3& scale)
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

void TSalmonRendererInterface::RotateMatrix(const vec4& q)
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

void TSalmonRendererInterface::PushSpecialMatrix(const mat4& m)
{
	if (ModelviewMatrixStack.size() > 64)
    {
        throw ErrorToLog("Modelview matrix stack overflow!!!!");
    }
    ModelviewMatrixStack.push(m);
    SetUniforms();
}


void TSalmonRendererInterface::PopMatrix()
{
	if (ModelviewMatrixStack.size() == 0)
	{
		throw ErrorToLog("Modelview matrix stack underflow!!!!");
	}
    ModelviewMatrixStack.pop();
    
    SetUniforms();
}

void TSalmonRendererInterface::PushProjectionMatrix(float width, float height)
{
    mat4 m = MakeOrthoMatrix(width, height);
    ProjectionMatrixStack.push(m);
    SetUniforms();
    
    if (ProjectionMatrixStack.size() > 64)
    {
        throw ErrorToLog("Projection matrix stack overflow!!!!");
    }
}

void TSalmonRendererInterface::PopProjectionMatrix()
{
	if (ProjectionMatrixStack.size() == 0)
	{
		throw ErrorToLog("Projection matrix stack underflow!!!!");
	}
    ProjectionMatrixStack.pop();
    SetUniforms();
}

void TSalmonRendererInterface::SetProjectionMatrix(float width, float height)
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

void TSalmonRendererInterface::SetFrameViewport(const std::string& frameName)
{
	AssertIfInMainThread();

	ivec2 frameWidthHeight = ResourceManager->FrameManager.GetFrameWidthHeight(frameName);
	glViewport(0, 0, frameWidthHeight.v[0], frameWidthHeight.v[1]);
}

void TSalmonRendererInterface::SetFullScreenViewport()
{
	AssertIfInMainThread();

	glViewport(0, 0, ScreenWidth, ScreenHeight);
}

void TSalmonRendererInterface::PushShader(const std::string& shaderName)
{

	ResourceManager->ShaderManager.PushShader(shaderName);
	SetUniforms();
	TryEnableVertexAttribArrays();
}

void TSalmonRendererInterface::PopShader()
{

	ResourceManager->ShaderManager.PopShader();
	SetUniforms();
	TryEnableVertexAttribArrays();

}



void TSalmonRendererInterface::SetPerspectiveProjectionMatrix(float angle, float aspect, float zNear, float zFar)
{
	mat4 m = MakePerspectiveMatrix(angle, aspect, zNear, zFar);

	if (ProjectionMatrixStack.size() == 0)
	{
		throw ErrorToLog("Projection matrix stack underflow!!!!");
	}
    ProjectionMatrixStack.pop();
    ProjectionMatrixStack.push(m);
    SetUniforms();
}

void TSalmonRendererInterface::PushPerspectiveProjectionMatrix(float angle, float aspect, float zNear, float zFar)
{
	mat4 m = MakePerspectiveMatrix(angle, aspect, zNear, zFar);
	
    ProjectionMatrixStack.push(m);

	if (ProjectionMatrixStack.size() > 64)
    {
        throw ErrorToLog("Projection matrix stack overflow!!!!");
    }

    SetUniforms();
}





void TSalmonRendererInterface::SetGLCamView()
{
	TSetCameraViewVisitor v;
	boost::apply_visitor(v, Camera);

	CamModelViewMatrix = ModelviewMatrixStack.top();
	CamInversedModelViewMatrix = InverseModelViewMatrix(CamModelViewMatrix);

	SetUniforms();
}

void TSalmonRendererInterface::SetGlIdentityView()
{
	
	LoadIdentity();
	
	CamModelViewMatrix = ModelviewMatrixStack.top();
	CamInversedModelViewMatrix = InverseModelViewMatrix(CamModelViewMatrix);

	SetUniforms();
}

void TSalmonRendererInterface::SetGlPosXView()
{
	LoadIdentity();

	RotateMatrix(vec4(0.f, 1.f * sin(pi / 4.f), 0.f, 1.f * cos(pi / 4.f)));
	RotateMatrix(vec4(1.f * sin(pi / 2.f), 0.f, 0.f, 1.f * cos(pi / 2.f)));
	
	TranslateMatrix(-GetCamShift());
	
	CamModelViewMatrix = ModelviewMatrixStack.top();
	CamInversedModelViewMatrix = InverseModelViewMatrix(CamModelViewMatrix);

	SetUniforms();

}

void TSalmonRendererInterface::SetGlNegXView()
{
	
	LoadIdentity();

	RotateMatrix(vec4(0.f, -1.f * sin(pi / 4.f), 0.f, 1.f * cos(pi / 4.f)));
	RotateMatrix(vec4(1.f * sin(pi / 2.f), 0.f, 0.f, 1.f * cos(pi / 2.f)));
	
	TranslateMatrix(-GetCamShift());
	
	CamModelViewMatrix = ModelviewMatrixStack.top();
	CamInversedModelViewMatrix = InverseModelViewMatrix(CamModelViewMatrix);

	SetUniforms();
}

void TSalmonRendererInterface::SetGlPosYView()
{
	
	LoadIdentity();
	RotateMatrix(vec4(-1.f * sin(pi / 4.f), 0.f, 0.f, 1.f * cos(pi / 4.f)));
	
	TranslateMatrix(-GetCamShift());
	
	CamModelViewMatrix = ModelviewMatrixStack.top();
	CamInversedModelViewMatrix = InverseModelViewMatrix(CamModelViewMatrix);

	SetUniforms();

}

void TSalmonRendererInterface::SetGlNegYView()
{
	
	LoadIdentity();
	RotateMatrix(vec4(1.f * sin(pi / 4.f), 0.f, 0.f, 1.f * cos(pi / 4.f)));
	
	TranslateMatrix(-GetCamShift());
	
	CamModelViewMatrix = ModelviewMatrixStack.top();
	CamInversedModelViewMatrix = InverseModelViewMatrix(CamModelViewMatrix);

	SetUniforms();
}

void TSalmonRendererInterface::SetGlPosZView()
{
	
	LoadIdentity();

	RotateMatrix(vec4(0.f, 1.f * sin(pi / 2.f), 0.f, 1.f * cos(pi / 2.f)));
	RotateMatrix(vec4(0.f, 0.f, 1.f * sin(pi / 2.f), 1.f * cos(pi / 2.f)));
	
	TranslateMatrix(-GetCamShift());
	
	CamModelViewMatrix = ModelviewMatrixStack.top();
	CamInversedModelViewMatrix = InverseModelViewMatrix(CamModelViewMatrix);

	SetUniforms();
}

void TSalmonRendererInterface::SetGlNegZView()
{
	
	LoadIdentity();

	RotateMatrix(vec4(0.f, 0.f, 1.f * sin(pi / 2.f), 1.f * cos(pi / 2.f)));
	
	TranslateMatrix(-GetCamShift());
	
	CamModelViewMatrix = ModelviewMatrixStack.top();
	CamInversedModelViewMatrix = InverseModelViewMatrix(CamModelViewMatrix);

	SetUniforms();

}


void TSalmonRendererInterface::SwitchToScreen()
{
	AssertIfInMainThread();

	glBindFramebuffer(GL_FRAMEBUFFER, CONST_SCREEN_FRAMEBUFFER);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void TSalmonRendererInterface::SwitchToFrameBuffer(const std::string& frameName)
{
	AssertIfInMainThread();
	
	if (ResourceManager->FrameManager.FrameMap.count(frameName) > 0)
	{
		TFrame Frame = ResourceManager->FrameManager.FrameMap[frameName];

		glBindFramebuffer(GL_FRAMEBUFFER, Frame.FrameBuffer);

		SetFrameViewport(frameName);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

void TSalmonRendererInterface::SwitchToCubemapBuffer(const std::string& frameName,size_t cubeSide)
{
	AssertIfInMainThread();
	
	if (ResourceManager->FrameManager.FrameMap.count(frameName) > 0)
	{
		TFrame Frame = ResourceManager->FrameManager.FrameMap[frameName];

		glBindFramebuffer(GL_FRAMEBUFFER, Frame.FrameBuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + cubeSide, Frame.TexID, 0);
	
		SetFrameViewport(frameName);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //IS A MUST!!!
	}
}



void TSalmonRendererInterface::BeginDrawToDepthBufferGlobal(std::string& globalBufferName)
{
	SwitchToFrameBuffer(globalBufferName);
	//SetGLLightProjectionAndView(ZeroVec3, GlobalShadowAreaHalfSize, 0);
}


void TSalmonRendererInterface::BeginDrawToDepthBufferLocal(std::string& localBufferName)
{
	SwitchToFrameBuffer(localBufferName);
	//SetGLLightProjectionAndView(CamShift, GlobalShadowAreaHalfSize/3.0f, 1);
}

void TSalmonRendererInterface::EndDrawToDepthBuffer()
{
	SetPerspectiveFullScreenViewport();
}

void TSalmonRendererInterface::DrawRect(const vec2& p1, const vec2& p2)
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

void TSalmonRendererInterface::DrawRect(const vec2& p1, const vec2& p2, const vec2& t1, const vec2& t2)
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


void TSalmonRendererInterface::DrawFrameFullScreen(const std::string& frameName)
{

	DrawFramePartScreen(frameName, vec2(0, 0), vec2(1, 1));
}

void TSalmonRendererInterface::DrawFramePartScreen(const std::string& frameName, vec2 posFrom, vec2 posTo)
{
	AssertIfInMainThread();

	size_t texID = ResourceManager->FrameManager.GetFrameTexture(frameName.c_str());

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

