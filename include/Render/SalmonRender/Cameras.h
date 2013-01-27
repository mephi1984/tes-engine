#ifndef CAMERAS_H_INCLUDED
#define CAMERAS_H_INCLUDED

#include "include/Utils/Utils.h"

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





} //namespace SE

#endif