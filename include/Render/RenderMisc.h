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
	vec3 VertexCoord[4];
	vec2 TextureCoord[4];
};

struct TDataTriangleList
{
	mutable std::map<std::string, std::vector<vec3>> Vec3CoordArr; //mutable because when you call [] on map, it may create a new vector =)
	mutable std::map<std::string, std::vector<vec2>> Vec2CoordArr; //mutable because when you call [] on map, it may create a new vector =)

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
	cardinal Buffer;

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

    
void FillVertexCoordVec(std::vector<vec3>& coordVec, int pos, vec2 posFrom, vec2 posTo);
    
void FillTexCoordVec(std::vector<vec2>& coordVec, int pos, vec2 texCoordFrom = vec2(0,0), vec2 texCoordTo = vec2(1,1));


std::vector<vec3> MakeVertexCoordVec(vec2 posFrom, vec2 posTo);

std::vector<vec2> MakeTexCoordVec(vec2 texCoordFrom = vec2(0,0), vec2 texCoordTo = vec2(1,1));


TDataTriangleList MakeDataTriangleList(vec2 posFrom, vec2 posTo, vec2 texCoordFrom = vec2(0,0), vec2 texCoordTo = vec2(1,1));
void MoveDataTriangleList(TDataTriangleList& triangleList, vec3 shift);
void RotateDataTriangleList(TDataTriangleList& triangleList, const mat3& m);
void ScaleDataTriangleList(TDataTriangleList& triangleList, float scale);
void ScaleDataTriangleList(TDataTriangleList& triangleList, vec3 scaleVec);
TDataTriangleList& ClearDataTriangleList(TDataTriangleList& triangleList);
TDataTriangleList& InsertIntoDataTriangleList(TDataTriangleList& triangleList, const std::vector<vec3>& vertexArr, const std::vector<vec2>& texCoordArr);

void Replace6PointsInTriangleList(TDataTriangleList& triangleList, int pos, vec2 posFrom, vec2 posTo, vec2 texCoordFrom = vec2(0,0), vec2 texCoordTo = vec2(1,1));


TTriangleList MakeTriangleList(vec2 posFrom, vec2 posTo, vec2 texCoordFrom = vec2(0,0), vec2 texCoordTo = vec2(1,1));

void CheckGlError(const std::string& msg = "");


} //namespace SE

#endif