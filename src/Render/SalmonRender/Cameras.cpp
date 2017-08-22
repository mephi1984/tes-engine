#include "include/Render/SalmonRender/Cameras.h"
#include "include/Engine.h"

namespace SE
{

TCameraInterface::TCameraInterface()
{
	CamVec = Vector3f(0, 0, -1);
	CamShift = { 0, 0, 0 };
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
	Renderer->CalcCamPos();
	
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
	Renderer->CalcCamPos();
	
}

void TPanoramicCamera::MoveDist(float dDist)
{ 
	CamDist += dDist; 
	
	if (CamDist<CONST_MIN_CAM_DIST) 
	{
		CamDist = CONST_MIN_CAM_DIST; 
	}

	
	//Possibly refactor???
	Renderer->CalcCamPos();
	
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
	Renderer->CalcCamPos();
}

void TPanoramicCamera::MoveForward()
{
	Vector3f mov;
	
	float sina = sinf(CamAlpha);
	float cosa = cosf(CamAlpha);

	mov(0) = sina;
	mov(1) = 0;
	mov(2) = - cosa;

	CamShift = CamShift + mov;
	
	//Possibly refactor???
	Renderer->CalcCamPos();
}

void TPanoramicCamera::MoveBackward()
{
	Vector3f mov;
	
	float sina = sinf(CamAlpha);
	float cosa = cosf(CamAlpha);

	mov(0) = - sina;
	mov(1) = 0;
	mov(2) = cosa;

	CamShift = CamShift + mov;
	//Possibly refactor???
	Renderer->CalcCamPos();
	
}

void TPanoramicCamera::MoveLeft()
{
	Vector3f mov;
	
	float sina = sinf(CamAlpha);
	float cosa = cosf(CamAlpha);

	mov(0) = - cosa;
	mov(1) = 0;
	mov(2) = - sina;

	CamShift = CamShift + mov;
	
	//Possibly refactor???
	Renderer->CalcCamPos();
	
}

void TPanoramicCamera::MoveRight()
{
	Vector3f mov;
	
	float sina = sinf(CamAlpha);
	float cosa = cosf(CamAlpha);

	mov(0) = cosa;
	mov(1) = 0;
	mov(2) = sina;

	CamShift = CamShift + mov;
	
	//Possibly refactor???
	Renderer->CalcCamPos();
	
}


void TPanoramicCamera::CalcCamVec()
{
	Vector3f camPos;
	Vector3f camVec;

	float sina = sinf(CamAlpha);
	float cosa = cosf(CamAlpha);
	float sinp = sinf(CamPhi);
	float cosp = cosf(CamPhi);

	CamVec(0) = CamDist * cosp * sina;
	CamVec(1) = - CamDist * sinp;
	CamVec(2) = - CamDist * cosp * cosa;

}

void TPanoramicCamera::SetCamView()
{
	
	//Renderer->LoadIdentity();
	Renderer->TranslateMatrix(Vector3f(0.0f, 0.0f, -CamDist));
	Renderer->RotateMatrix(Vector4f(1.f * sin(CamPhi/2.f), 0.f, 0.f, 1.f * cos(CamPhi/2.f)));
	Renderer->RotateMatrix(Vector4f(0.f, 1.f * sin(CamAlpha/2.f), 0.f, 1.f * cos(CamAlpha/2.f)));
	Renderer->TranslateMatrix(-CamShift);
}


void TPanoramicCamera::SetCamShift(const Vector3f& camShift)
{
	CamShift = camShift;

	Renderer->CalcCamPos();
}


/*
void TPitCamera::SetCamView()
{
	Renderer->LoadIdentity();
	Renderer->RotateMatrix(InverseQuat(CameraQuat));

	Renderer->TranslateMatrix(-CamShift);
}

void TPitCamera::CalcCamVec()
{
	Vector3f r = Vector3f(0,0,-1);

	CamVec = Normalize(CameraQuat * Vector4f(r) * InverseQuat(CameraQuat));
}

void TPitCamera::RotateByQuat(Vector4f quat)
{
	CameraQuat = quat * CameraQuat;
	//float len = Length(CameraQuat);
}*/


} //namespace SE