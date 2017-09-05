#ifndef GLOBAL_CONST_H_INCLUDED
#define GLOBAL_CONST_H_INCLUDED

#include "include/Utils/DataTypes/DataTypes.h"

namespace SE
{

/*
This code contains global constant values for engine
*/

const float CONST_MAX_CAM_DIST = 20.0f;
//How far camera could be from game field

//const float CONST_MIN_CAM_DIST = 4.0f;
const float CONST_MIN_CAM_DIST = 0.0f;
//How near camera could be from game field

//const float CONST_MIN_CAM_PHI = pi/48;
const float CONST_MIN_CAM_PHI = -pi/2;

const float CONST_MAX_CAM_PHI = pi/2;

const int CONST_MAX_SHADER_LIGHTS = 8;
//Max light sources count (for shader)

//const size_t CONST_TIMER_INTERVAL = 5;
//Time interval to call Application::Update()

const float CONST_DEFAULT_FOG_BEGIN_DISTANCE = 14.0f;

const float CONST_DEFAULT_FOG_END_DISTANCE = 19.0f;

const Vector4f CONST_DEFAULT_FOG_COLOR(0.2f, 0.5f, 1.0f, 1.0f);

const float CONST_DEFAULT_GLOBAL_SHADOW_AREA_HALFSIZE = 20.0f;

const int CONST_MAX_STACK_SIZE = 100;

const float CONST_CAMERA_VERTICAL_SHIFT = 1.5f;

const float CONST_CAMERA_PHI_ELEVATION_STEP = pi/48;

} //namespace SE

#endif
