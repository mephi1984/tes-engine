#ifndef DATA_TYPES_H_INCLUDED
#define DATA_TYPES_H_INCLUDED

/*
This code contains additional data types and some const values to use
*/


#include <string>
#include <sstream>
#include <stdlib.h>
#include "boost/lexical_cast.hpp"

#ifdef TARGET_ANDROID

//Hack to make Eigen compile in Android NDK

#include <cmath>

namespace std {
	/*
	float round(float arg);
	
	double round(double arg);
	long double round(long double arg);
	
	float log1p(float arg);
	double log1p(double arg);
	long double log1p(long double arg);
	
	float expm1(float arg);
	double expm1(double arg);
	long double expm1(long double arg);
	*/
	/*
	template<>
	float round( float arg );
	
	template<>
	long double round( long double arg );
	
	template<>
	float log1p( float arg );
	
	template<>
	long double log1p( long double arg );
	
	template<>
	float expm1( float arg );
	
	template<>
	long double expm1( long double arg );*/
	
}

#endif


#include <Eigen/Geometry>
#include <Eigen/Dense>

namespace SE
{
	using namespace Eigen;


const float pi = 3.14159265359f;

const float CONST_EPSILON = 0.00001f;

const float WhiteColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
const float RedColor[4] = {1.0f, 0.0f, 0.0f, 1.0f};
const float NoColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};

Matrix4f MakeOrthoMatrix(float width, float height, float zNear = 0, float zFar = 1.f);
Matrix4f MakeFrustumMatrix(float left, float right, float bottom, float top, float nearVal, float farVal);
Matrix4f MakePerspectiveMatrix(float angle, float aspect, float zNear, float zFar);

bool IsFloatEqual(float a, float b);
/*
bool LineCouldCrossTriangle(const Vector3f& a, const Vector3f& b, const TSmpTriangle& tr); //overall (dimensional) test

int LineCrossTriangle(const Vector3f& a, const Vector3f& b, const TSmpTriangle& tr);

int LineCrossTriangle(const Vector3f& a, const Vector3f& b, const TSmpTriangle& tr, Vector3f& CrossPoint);

int PointInTriangle(const Vector3f& q, const TSmpTriangle& tr);

int LineCrossLine2d(const Vector2f& a1, const Vector2f& a2, const Vector2f& b1, const Vector2f& b2, Vector2f& r);
*/


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

size_t GetGreaterPower2(size_t x);

Vector2f StringToVec2(std::string str);
Vector3f StringToVec3(std::string str);
Vector4f StringToVec4(std::string str);

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