#ifndef LIGHT_MANAGER_H_INCLUDED
#define LIGHT_MANAGER_H_INCLUDED


/*
This code contains manager to operate OpenGL lighting

Use global variable ResourceManager to get access to light manager

*/


#include "include/Utils/Utils.h"


namespace SE
{

class TLightManager
{
protected:
	bool LightingEnabled;
	
	Vector3f LightPos;
	Vector3f LightDirection;

	Vector4f LightColor;

public:
	TLightManager();

	void SetLightPos(Vector3f pos);
	void SetLightDirection(Vector3f dir);
	void SetLightColor(Vector4f color);

	void SetLightOn();
	void SetLightOff();

	//To be called from SetUniforms
	void SetLightUniforms();
};


} //namespace SE

#endif