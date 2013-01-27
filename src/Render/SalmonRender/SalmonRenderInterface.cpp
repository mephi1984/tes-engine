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
{
	ProjectionMatrixStack.push(IdentityMatrix4);
	
	ModelviewMatrixStack.push(IdentityMatrix4);

}


void TSalmonRendererInterface::SetUniforms()
{

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
	

	ResourceManager->LightManager.SetLightUniforms();
}


void TSalmonRendererInterface::InitOpenGL(int screenWidth, int screenHeight, float matrixWidth, float matrixHeight)
{
	
	ModelviewMatrixStack.push(IdentityMatrix4);
	ProjectionMatrixStack.push(IdentityMatrix4);

	ScreenWidth = screenWidth;
	ScreenHeight = screenHeight;

	MatrixWidth = matrixWidth;
	MatrixHeight = matrixHeight;

	glEnable(GL_DEPTH_TEST);
	#ifdef TARGET_WIN32
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, WhiteColor);
	
	glPolygonMode(GL_FRONT_AND_BACK , GL_FILL);
	#endif

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	CheckGlError();

}


void TSalmonRendererInterface::CalcCamPosVec()
{
	
	TCalcCamVecVisitor v;

	boost::apply_visitor(v, CameraMover);

	CamPos = GetCamShift() - GetCamVec();

	RenderUniform3fv(CONST_STRING_CAMPOS_UNIFORM,(float*)&CamPos);
}

void TSalmonRendererInterface::SetPerspectiveFullScreenViewport()
{

	glViewport(0, 0, ScreenWidth, ScreenHeight);

	SetPerspectiveProjectionMatrix(pi / 6.f, float(MatrixWidth) / float(MatrixHeight), 1.0f, 100.0f);
}

void TSalmonRendererInterface::SetPerspectiveProjection(float angle, float zNear, float zFar)
{
	SetPerspectiveProjectionMatrix(angle, float(MatrixWidth) / float(MatrixHeight), zNear, zFar);
}


void TSalmonRendererInterface::SetOrthoFullScreenViewport()
{
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
	boost::apply_visitor(v, CameraMover);

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


vec3 TSalmonRendererInterface::GetCamPos() 
{ 
	return CamPos; 
}


void TSalmonRendererInterface::SwitchToScreen()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void TSalmonRendererInterface::SwitchToFrameBuffer(const std::string& frameName)
{
	
	if (ResourceManager->FrameManager.FrameMap.count(frameName) > 0)
	{
		TFrame Frame = ResourceManager->FrameManager.FrameMap[frameName];

		glBindFramebuffer(GL_FRAMEBUFFER, Frame.FrameBuffer);

		SetFrameViewport(frameName);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

void TSalmonRendererInterface::SwitchToCubemapBuffer(const std::string& frameName,cardinal cubeSide)
{

	
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


} //namespace SE

