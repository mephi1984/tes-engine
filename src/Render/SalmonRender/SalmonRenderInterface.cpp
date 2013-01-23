#include "include/Render/SalmonRender/SalmonRenderInterface.h"

#include "include/Utils/Utils.h"
#include "include/Engine.h"

namespace SE
{

TCameraInterface::TCameraInterface()
	: CamShift(ZeroVec3)
	, CamVec(ZeroVec3)
{
}

TPanoramicCamera::TPanoramicCamera()
	: CamAlpha(0.0f)
	, CamPhi(0.0f)
	, CamDist(0.0f)
{
}


void TPanoramicCamera::MoveAlpha(float dAlpha) 
{ 
	if (dAlpha == 0.0f)
		return;

	CamAlpha += dAlpha; 

	while (CamAlpha >= 2*pi) 
	{
		CamAlpha -= 2*pi; 
	}

	while (CamAlpha<0.0f)
	{
		CamAlpha += 2*pi;
	}
	
	//Possibly refactor???
	Renderer->CalcCamPosVec();
	
}

void TPanoramicCamera::MovePhi(float dPhi) 
{ 
	if (dPhi == 0.0f)
		return;

	//float oldCamPhi = CamPhi;
	CamPhi += dPhi;
	
	if (CamPhi > CONST_MAX_CAM_PHI) 
	{
		CamPhi = CONST_MAX_CAM_PHI;
	}
	
	if (CamPhi < CONST_MIN_CAM_PHI)
	{
		CamPhi = CONST_MIN_CAM_PHI;
	}
	
	//Possibly refactor???
	Renderer->CalcCamPosVec();
	
}

void TPanoramicCamera::MoveDist(float dDist)
{ 
	CamDist += dDist; 
	
	if (CamDist<CONST_MIN_CAM_DIST) 
	{
		CamDist = CONST_MIN_CAM_DIST; 
	}

	
	//Possibly refactor???
	Renderer->CalcCamPosVec();
	
}

void TPanoramicCamera::SetAlpha(float alpha)
{
	if (alpha == CamAlpha)
		return;

	CamAlpha = alpha; 

	while (CamAlpha >= 2*pi) 
	{
		CamAlpha -= 2*pi; 
	}

	while (CamAlpha<0.0f)
	{
		CamAlpha += 2*pi;
	}
	
	//Possibly refactor???
	Renderer->CalcCamPosVec();
}

void TPanoramicCamera::MoveForward()
{
	vec3 mov;
	
	float sina = sinf(CamAlpha);
	float cosa = cosf(CamAlpha);

	mov.v[0] = sina;
	mov.v[1] = 0;
	mov.v[2] = - cosa;

	CamShift = CamShift + mov;
	
	//Possibly refactor???
	Renderer->CalcCamPosVec();
}

void TPanoramicCamera::MoveBackward()
{
	vec3 mov;
	
	float sina = sinf(CamAlpha);
	float cosa = cosf(CamAlpha);

	mov.v[0] = - sina;
	mov.v[1] = 0;
	mov.v[2] = cosa;

	CamShift = CamShift + mov;
	//Possibly refactor???
	Renderer->CalcCamPosVec();
	
}

void TPanoramicCamera::MoveLeft()
{
	vec3 mov;
	
	float sina = sinf(CamAlpha);
	float cosa = cosf(CamAlpha);

	mov.v[0] = - cosa;
	mov.v[1] = 0;
	mov.v[2] = - sina;

	CamShift = CamShift + mov;
	
	//Possibly refactor???
	Renderer->CalcCamPosVec();
	
}

void TPanoramicCamera::MoveRight()
{
	vec3 mov;
	
	float sina = sinf(CamAlpha);
	float cosa = cosf(CamAlpha);

	mov.v[0] = cosa;
	mov.v[1] = 0;
	mov.v[2] = sina;

	CamShift = CamShift + mov;
	
	//Possibly refactor???
	Renderer->CalcCamPosVec();
	
}


void TPanoramicCamera::CalcCamVec()
{
	vec3 camPos;
	vec3 camVec;

	float sina = sinf(CamAlpha);
	float cosa = cosf(CamAlpha);
	float sinp = sinf(CamPhi);
	float cosp = cosf(CamPhi);

	CamVec.v[0] = CamDist * cosp * sina;
	CamVec.v[1] = - CamDist * sinp;
	CamVec.v[2] = - CamDist * cosp * cosa;

}

void TPanoramicCamera::SetCamView()
{
	
	Renderer->LoadIdentity();
	Renderer->TranslateMatrix(vec3(0.0f, 0.0f, -CamDist));
	Renderer->RotateMatrix(vec4(1.f * sin(CamPhi/2.f), 0.f, 0.f, 1.f * cos(CamPhi/2.f)));
	Renderer->RotateMatrix(vec4(0.f, 1.f * sin(CamAlpha/2.f), 0.f, 1.f * cos(CamAlpha/2.f)));
	Renderer->TranslateMatrix(-CamShift);
}


void TPitCamera::SetCamView()
{
	Renderer->LoadIdentity();
	Renderer->RotateMatrix(InverseQuat(CameraQuat));

	Renderer->TranslateMatrix(-CamShift);
}

void TPitCamera::CalcCamVec()
{
	vec3 r = vec3(0,0,-1);

	CamVec = Normalize(CameraQuat * vec4(r) * InverseQuat(CameraQuat));
}

void TPitCamera::RotateByQuat(vec4 quat)
{
	CameraQuat = quat * CameraQuat;
	//float len = Length(CameraQuat);
}

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

	SetPerspectiveProjectionMatrix(pi / 6.f, 1.0, 1.0f, 100.0f); //Baad =(

	SetPerspectiveFullScreenViewport();

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

void TSalmonRendererInterface::SetOrthoFullScreenViewport()
{

	glViewport(0, 0, ScreenWidth, ScreenHeight);

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
	/*
	glLoadIdentity();

	glGetFloatv(GL_MODELVIEW_MATRIX,CamModelViewMatrix.m);

	CamInversedModelViewMatrix = InverseModelViewMatrix(CamModelViewMatrix);

	RenderUniform3fv(CONST_STRING_CAMPOS_UNIFORM, (float*)ZeroVec3.v);*/

	LoadIdentity();
	
	CamModelViewMatrix = ModelviewMatrixStack.top();
	CamInversedModelViewMatrix = InverseModelViewMatrix(CamModelViewMatrix);

	SetUniforms();
}

void TSalmonRendererInterface::SetGlPosXView()
{
	/*
	glLoadIdentity();
	glRotatef (  90.0, 0.0, 1.0, 0.0 );
    glRotatef ( 180.0, 1.0, 0.0, 0.0 );
	glTranslatef(-CamPos.v[0], -CamPos.v[1], -CamPos.v[2]);

	glGetFloatv(GL_MODELVIEW_MATRIX,CamModelViewMatrix.m);

	CamInversedModelViewMatrix = InverseModelViewMatrix(CamModelViewMatrix);*/

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
	/*
	glLoadIdentity();
	glRotatef ( -90.0, 0.0, 1.0, 0.0 );
    glRotatef ( 180.0, 1.0, 0.0, 0.0 );
	glTranslatef(-CamPos.v[0], -CamPos.v[1], -CamPos.v[2]);

	glGetFloatv(GL_MODELVIEW_MATRIX,CamModelViewMatrix.m);

	CamInversedModelViewMatrix = InverseModelViewMatrix(CamModelViewMatrix);*/

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
	/*
	glLoadIdentity();
	glRotatef ( -90.0, 1.0, 0.0, 0.0 );
	glTranslatef(-CamPos.v[0], -CamPos.v[1], -CamPos.v[2]);

	glGetFloatv(GL_MODELVIEW_MATRIX,CamModelViewMatrix.m);

	CamInversedModelViewMatrix = InverseModelViewMatrix(CamModelViewMatrix);*/

	LoadIdentity();
	RotateMatrix(vec4(-1.f * sin(pi / 4.f), 0.f, 0.f, 1.f * cos(pi / 4.f)));
	
	TranslateMatrix(-GetCamShift());
	
	CamModelViewMatrix = ModelviewMatrixStack.top();
	CamInversedModelViewMatrix = InverseModelViewMatrix(CamModelViewMatrix);

	SetUniforms();

}

void TSalmonRendererInterface::SetGlNegYView()
{
	/*
	glLoadIdentity();
	glRotatef (  90.0, 1.0, 0.0, 0.0 );
	glTranslatef(-CamPos.v[0], -CamPos.v[1], -CamPos.v[2]);

	glGetFloatv(GL_MODELVIEW_MATRIX,CamModelViewMatrix.m);

	CamInversedModelViewMatrix = InverseModelViewMatrix(CamModelViewMatrix);*/

	LoadIdentity();
	RotateMatrix(vec4(1.f * sin(pi / 4.f), 0.f, 0.f, 1.f * cos(pi / 4.f)));
	
	TranslateMatrix(-GetCamShift());
	
	CamModelViewMatrix = ModelviewMatrixStack.top();
	CamInversedModelViewMatrix = InverseModelViewMatrix(CamModelViewMatrix);

	SetUniforms();
}

void TSalmonRendererInterface::SetGlPosZView()
{
	/*
	glLoadIdentity();
	glRotatef ( 180.0, 0.0, 1.0, 0.0 );
	glRotatef ( 180.0, 0.0, 0.0, 1.0 );
	glTranslatef(-CamPos.v[0], -CamPos.v[1], -CamPos.v[2]);

	glGetFloatv(GL_MODELVIEW_MATRIX,CamModelViewMatrix.m);

	CamInversedModelViewMatrix = InverseModelViewMatrix(CamModelViewMatrix);
	*/

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
	/*
	glLoadIdentity();
	glRotatef ( 180.0, 0.0, 0.0, 1.0 );
	glTranslatef(-CamPos.v[0], -CamPos.v[1], -CamPos.v[2]);

	glGetFloatv(GL_MODELVIEW_MATRIX,CamModelViewMatrix.m);

	CamInversedModelViewMatrix = InverseModelViewMatrix(CamModelViewMatrix);*/

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
	SetPerspectiveFullScreenViewport();
	SetGLCamView();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void TSalmonRendererInterface::SwitchToFrameBuffer(const std::string& frameName)
{
	
	if (ResourceManager->FrameManager.FrameMap.count(frameName) > 0)
	{
		TFrame Frame = ResourceManager->FrameManager.FrameMap[frameName];

		glBindFramebuffer(GL_FRAMEBUFFER, Frame.FrameBuffer);

		SetFrameViewport(frameName);

		SetGLCamView();
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
		/*
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(90, 1, 1, 100);
		glViewport(0, 0, Frame.Width, Frame.Height);
		glMatrixMode(GL_MODELVIEW);*/

		SetPerspectiveProjectionMatrix(90, 1, 1, 100);
		
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

