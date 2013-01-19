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
	
	vec3 LightPos;
	vec3 LightDirection;

	vec4 LightColor;

public:
	TLightManager();

	void SetLightPos(vec3 pos);
	void SetLightDirection(vec3 dir);
	void SetLightColor(vec4 color);

	void SetLightOn();
	void SetLightOff();

	//To be called from SetUniforms
	void SetLightUniforms();
};


} //namespace SE

#endif