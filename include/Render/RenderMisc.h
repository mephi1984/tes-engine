#ifndef RENDER_MISC_H_INCLUDED
#define RENDER_MISC_H_INCLUDED


#include "include/Utils/DataTypes/DataTypes.h"
//#include "include/Utils/SerializeInterface/SerializeInterface.h"
#include "boost/shared_ptr.hpp"
#include <vector>
#include <list>
#include <map>



#ifdef TARGET_WIN32

#include "include/OpenGLExt/OpenGlExt.h"

#endif

namespace SE
{

struct T2DQuad
{
	Vector3f VertexCoord[4];
	Vector2f TextureCoord[4];
};

struct TDataTriangleList
{
	mutable std::map<std::string, std::vector<Vector3f>> Vec3CoordArr; //mutable because when you call [] on map, it may create a new vector =)
	mutable std::map<std::string, std::vector<Vector2f>> Vec2CoordArr; //mutable because when you call [] on map, it may create a new vector =)

	TDataTriangleList& operator+=(const TDataTriangleList& dataTriangleList);
};

class TTriangleListAncestor
{
public:

	TDataTriangleList Data;

	virtual void RefreshBuffer() { } //Dummy for Android, but you need this for PC

	TTriangleListAncestor();

	TTriangleListAncestor(const TTriangleListAncestor& c);

	TTriangleListAncestor& operator=(const TTriangleListAncestor& c);

	TTriangleListAncestor(const TDataTriangleList& dataTriangleList);

	TTriangleListAncestor& operator=(const TDataTriangleList& dataTriangleList);

	virtual ~TTriangleListAncestor();
};


#ifdef TARGET_WIN32

class VBOObject //Must stay in shared ptr only!
{
public:
	size_t Buffer;

	VBOObject();

	VBOObject(const VBOObject& c);

	VBOObject& operator=(const VBOObject& c);

	~VBOObject();
};



class TTriangleList : public TTriangleListAncestor //Implementation differs for Windows and Android
{
protected:
	virtual void InnerRefreshBuffer();

	bool NeedPrepareBufferObjects;

	bool NeedRefreshBuffer; //Dummy for Android, but you need this for PC
public:

	mutable std::map<std::string, std::shared_ptr<VBOObject>> VertBufferArr;

	TTriangleList();

	~TTriangleList();

	virtual void RefreshBuffer();
	
};

#endif

#ifdef TARGET_ANDROID

class TTriangleList : public TTriangleListAncestor
{
	//No difference
};

#endif


#ifdef TARGET_IOS

class TTriangleList : public TTriangleListAncestor
{
	//No difference
};

#endif

   

void FillVertexCoordVec(std::vector<Vector3f>& coordVec, int pos, Vector2f posFrom, Vector2f posTo);
//Adds rect points (6 vertices) into coordVec

void FillTexCoordVec(std::vector<Vector2f>& coordVec, int pos, Vector2f texCoordFrom = Vector2f(0,0), Vector2f texCoordTo = Vector2f(1,1));
//Adds rect points (6 tex coords) into coordVec

void FillVertexCoordVec_4Points(std::vector<Vector3f>& coordVec, int pos, Vector2f pos1, Vector2f pos2, Vector2f pos3, Vector2f pos4);
//Adds rect points (6 vertices) into coordVec

void FillTexCoordVec_4Points(std::vector<Vector2f>& coordVec, int pos, Vector2f texCoord1 = Vector2f(0,0), Vector2f texCoord2 = Vector2f(1,0), Vector2f texCoord3 = Vector2f(1,1), Vector2f texCoord4 = Vector2f(0,1));
//Adds rect points (6 tex coords) into coordVec


std::vector<Vector3f> MakeVertexCoordVec(Vector2f posFrom, Vector2f posTo);
//Creates array of rect (6 vertices)

std::vector<Vector2f> MakeTexCoordVec(Vector2f texCoordFrom = Vector2f(0,0), Vector2f texCoordTo = Vector2f(1,1));
//Creates array of rect (6 tex coords)

TDataTriangleList MakeDataTriangleList(Vector2f posFrom, Vector2f posTo, Vector2f texCoordFrom = Vector2f(0,0), Vector2f texCoordTo = Vector2f(1,1));
//Creates a DataTriangleList just containing rect

void MoveDataTriangleList(TDataTriangleList& triangleList, Vector3f shift);
//Translates all points in DataTriangleList with shift vector

void RotateDataTriangleList(TDataTriangleList& triangleList, const Matrix3f& m);
//Rotates all points in triangleList with rotation matrix

void ScaleDataTriangleList(TDataTriangleList& triangleList, float scale);
//Scales all points in triangleList by scale value

void ScaleDataTriangleList(TDataTriangleList& triangleList, Vector3f scaleVec);
//Scales all points in triangleList by scaleVec vector

TDataTriangleList& ClearDataTriangleList(TDataTriangleList& triangleList);
//Clears triangle list, returning itself

TDataTriangleList& InsertIntoDataTriangleList(TDataTriangleList& triangleList, const std::vector<Vector3f>& vertexArr, const std::vector<Vector2f>& texCoordArr);
//Inserts additional points and their tex coords into triangle list

void Replace6PointsInTriangleList(TDataTriangleList& triangleList, int pos, Vector2f posFrom, Vector2f posTo, Vector2f texCoordFrom = Vector2f(0,0), Vector2f texCoordTo = Vector2f(1,1));
//Replaces one rect in triangleList at position pos by new rect. pos is position in array for first vertex of a rectangle

void Replace6PointsInTriangleList_4Points(TDataTriangleList& triangleList, int pos, Vector2f pos1, Vector2f pos2, Vector2f pos3, Vector2f pos4, Vector2f texCoord1 = Vector2f(0,0), Vector2f texCoord2 = Vector2f(1,0), Vector2f texCoord3 = Vector2f(1,1), Vector2f texCoord4 = Vector2f(0,1));
//Replaces one rect in triangleList at position pos by new rect. pos is position in array for first vertex of a rectangle


TTriangleList MakeTriangleList(Vector2f posFrom, Vector2f posTo, Vector2f texCoordFrom = Vector2f(0,0), Vector2f texCoordTo = Vector2f(1,1));
//Creates triangle list containing rect

void CheckGlError(const std::string& msg = "");


} //namespace SE

#endif