#ifndef CAMERAS_H_INCLUDED
#define CAMERAS_H_INCLUDED

#include "include/Utils/Utils.h"

namespace SE
{

	
struct TCameraInterface
{
	Vector3f CamVec;

	Vector3f CamShift;

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

	void SetCamShift(const Vector3f& camShift);
};

/*
struct TPitCamera : public TCameraInterface
{
	Vector4f CameraQuat;
	
	void SetCamView();

	void CalcCamVec();

	void RotateByQuat(Vector4f quat);
};
*/

//typedef boost::variant<TPanoramicCamera, TPitCamera> TCamera;

typedef boost::variant<TPanoramicCamera> TCamera;


struct TSetCameraViewVisitor : public boost::static_visitor<void>
{
	void operator()(TPanoramicCamera& panoramicCamera)
    {
        panoramicCamera.SetCamView();
    }
    /*
    void operator()(TPitCamera& pitCamera)
    {
        pitCamera.SetCamView();
    }*/
};


struct TCalcCamVecVisitor : public boost::static_visitor<void>
{
	void operator()(TPanoramicCamera& panoramicCamera)
    {
        panoramicCamera.CalcCamVec();
    }
    
	/*
    void operator()(TPitCamera& pitCamera)
    {
        pitCamera.CalcCamVec();
    }*/
};


struct TGetCamVecVisitor : public boost::static_visitor<Vector3f&>
{
	Vector3f& operator()(TPanoramicCamera& panoramicCamera) const
    {
        return panoramicCamera.CamVec;
    }
    /*
    Vector3f& operator()(TPitCamera& pitCamera) const
    {
        return pitCamera.CamVec;
    }*/
};

struct TGetCamShiftVisitor : public boost::static_visitor<Vector3f&>
{
	Vector3f& operator()(TPanoramicCamera& panoramicCamera) const
    {
        return panoramicCamera.CamShift;
    }
    /*
    Vector3f& operator()(TPitCamera& pitCamera) const
    {
        return pitCamera.CamShift;
    }*/
};





} //namespace SE

#endif