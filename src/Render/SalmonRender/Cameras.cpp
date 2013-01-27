#include "include/Render/SalmonRender/Cameras.h"
#include "include/Engine.h"

namespace SE
{

TCameraInterface::TCameraInterface()
	: CamShift(ZeroVec3)
{
	CamVec = vec3(0,0,-1);
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


} //namespace SE