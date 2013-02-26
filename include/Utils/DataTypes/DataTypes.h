#ifndef DATA_TYPES_H_INCLUDED
#define DATA_TYPES_H_INCLUDED

/*
This code contains additional data types and some const values to use
*/

#include <math.h>
#include <string>
#include <sstream>
#include <stdlib.h>
#include "boost/lexical_cast.hpp"

#include "NewDataTypes.h"

namespace SE
{

typedef unsigned int cardinal;
typedef unsigned short int word;
//typedef unsigned char byte;

typedef cardinal* pcardinal;

//Use mat4 to store projection/modelview matrices (i.e. for shadow mapping)
struct mat4
{
	float m[16];
};

struct TSmpTriangle
{
	vec3 p[3];
	vec3 v;
	vec3 w;
	vec3 n;
};


const float pi = 3.14159265359f;

const float CONST_EPSILON = 0.00001f;

const float WhiteColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
const float RedColor[4] = {1.0f, 0.0f, 0.0f, 1.0f};
const float NoColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};

const mat3 IdentityMatrix(vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f));

const mat4 IdentityMatrix4 = 
	{ 1.f, 0.f, 0.f, 0.f,
	  0.f, 1.f, 0.f, 0.f,
	  0.f, 0.f, 1.f, 0.f,
	  0.f, 0.f, 0.f, 1.f };


const vec3 ZeroVec3(0.0f, 0.0f, 0.0f);

const vec4 ZeroQuat(0.0f, 0.0f, 0.0f, 1.0f);

const std::string fendl="\x0D\x0A"; //Windows-style, for files
const std::string endl="\n";

vec4 InverseQuat(const vec4& q);

//For shadow mapping or for OpenGL ES 2.0
mat4 InverseModelViewMatrix(const mat4& m);
mat4 SetToNormalMatrix(const mat4& m);
mat4 MakeOrthoMatrix(float width, float height);
mat4 MakeFrustumMatrix(float left, float right, float bottom, float top, float nearVal, float farVal);
mat4 MakePerspectiveMatrix(float angle, float aspect, float zNear, float zFar);

mat4 MultMatrixMatrix(const mat4& m1, const mat4& m2);

bool IsFloatEqual(float a, float b);

bool LineCouldCrossTriangle(const vec3& a, const vec3& b, const TSmpTriangle& tr); //overall (dimensional) test

int LineCrossTriangle(const vec3& a, const vec3& b, const TSmpTriangle& tr);

int LineCrossTriangle(const vec3& a, const vec3& b, const TSmpTriangle& tr, vec3& CrossPoint);

int PointInTriangle(const vec3& q, const TSmpTriangle& tr);


template<typename T>
inline std::string tostr(T i)
{
	return boost::lexical_cast<std::string>(i);
}

inline int toint(const char* str)
{
    return boost::lexical_cast<int>(str);
}

inline int toint(const std::string& str)
{
    return boost::lexical_cast<int>(str);
}
//#ifndef TARGET_WIN32 //Already defined in windows.h

template<typename T>
inline T min(T a, T b)
{
	return a < b ? a : b;
}

template<typename T>
inline T max(T a, T b)
{
	return a > b ? a : b;
}

//#endif

template<typename T>
inline T clamp(T a, T c_from, T c_to)
{
	if (a < c_from)
	{
		return c_from;
	}
	
	if (a > c_to)
	{
		return c_to;
	}
	
	return a;
}

template<typename T>
inline T sign(T a)
{
	if (a == 0)
	{
		return 0;
	}
	
	if (a < 0)
	{
		return -1;
	}
	
	
	return 1;
}


float roundf(float r);

template<typename T>
T InterpolateLinear(T a, T b, float t)
{
	return a + (b-a)*t;
}

template<typename T>
T InterpolateSqr(T a, T b, float t)
{
	return a + (b-a)*t*t;
}


template<typename T>
T InterpolateSqrt(T a, T b, float t)
{
	return a + (b-a)*sqrtf(t);
}

template<typename T>
bool IsPower2(T x)
{
	return ( (x > 0) && ((x & (x - 1)) == 0) );
}

cardinal GetGreaterPower2(cardinal x);

vec2 StringToVec2(std::string str);
vec3 StringToVec3(std::string str);
vec4 StringToVec4(std::string str);

// ================================================
// ========== wide strings and UTF8 strings =======
// ================================================


void utf8toWStr(std::wstring& dest, const std::string& src);

void wstrToUtf8(std::string& dest, const std::wstring& src);

std::string wstrToUtf8(const std::wstring& str);

std::wstring utf8toWStr(const std::string& str);

std::ostream& operator<<(std::ostream& f, const std::wstring& s);

std::istream& operator>>(std::istream& f, std::wstring& s);

} //namespace SE

#endif