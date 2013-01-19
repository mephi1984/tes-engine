#include "include/LightManager/LightManager.h"

#include "include/Utils/Utils.h"
#include "include/Engine.h"


namespace SE
{

//===================================================
//================ LIGHT MANAGER ====================
//===================================================

TLightManager::TLightManager()
	: LightingEnabled(false)
{
}

void TLightManager::SetLightPos(vec3 pos)
{
	LightPos = pos;
}

void TLightManager::SetLightDirection(vec3 dir)
{
	LightDirection = Normalize(dir);
}

void TLightManager::SetLightColor(vec4 color)
{
	LightColor = color;
}


void TLightManager::SetLightOn()
{
	LightingEnabled = true;
}

void TLightManager::SetLightOff()
{
	LightingEnabled = false;
}

void TLightManager::SetLightUniforms()
{

	RenderUniform3fv(CONST_STRING_LIGHT_POS_UNIFORM, LightPos.v);
	RenderUniform3fv(CONST_STRING_LIGHT_DIRECTION_UNIFORM, LightDirection.v);
	RenderUniform4fv(CONST_STRING_LIGHT_COLOR_UNIFORM, LightColor.v);
}

} //namespace SE