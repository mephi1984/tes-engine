#include "include/Engine.h"

namespace SE
{

#ifdef TARGET_WIN32

VBOObject::VBOObject()
{
	AssertIfInMainThread();

	glGenBuffers(1, &Buffer);
}

VBOObject::VBOObject(const VBOObject& c)
{
	throw ErrorToLog("Copy constructor for VBOObject called\n");
}

VBOObject& VBOObject::operator=(const VBOObject& c)
{
	throw ErrorToLog("operator= for VBOObject called\n");
	return *this;
}

VBOObject::~VBOObject()
{
	AssertIfInMainThread();

	glDeleteBuffers(1, &Buffer);
}


#endif

TDataTriangleList& TDataTriangleList::operator+=(const TDataTriangleList& dataTriangleList)
{
	BOOST_FOREACH(auto& i, dataTriangleList.Vec3CoordArr)
	{
		if (Vec3CoordArr.count(i.first) == 0)
		{
			Vec3CoordArr[i.first] = i.second;
		}
		else
		{
			Vec3CoordArr[i.first].insert(Vec3CoordArr[i.first].end(), i.second.begin(), i.second.end());
		}
	}

	BOOST_FOREACH(auto& i, dataTriangleList.Vec2CoordArr)
	{
		if (Vec2CoordArr.count(i.first) == 0)
		{
			Vec2CoordArr[i.first] = i.second;
		}
		else
		{
			Vec2CoordArr[i.first].insert(Vec2CoordArr[i.first].end(), i.second.begin(), i.second.end());
		}
	}

	return *this;
}


TTriangleListAncestor::TTriangleListAncestor()
	: NeedRefreshBuffer(false)
{
}

TTriangleListAncestor::TTriangleListAncestor(const TTriangleListAncestor& c)
	: NeedRefreshBuffer(false)
{
	Data = c.Data;
}

TTriangleListAncestor& TTriangleListAncestor::operator=(const TTriangleListAncestor& c)
{
	Data = c.Data;
	NeedRefreshBuffer = c.NeedRefreshBuffer;

	return *this;
}

TTriangleListAncestor::TTriangleListAncestor(const TDataTriangleList& dataTriangleList)
	: NeedRefreshBuffer(false)
{
	Data = dataTriangleList;
}

TTriangleListAncestor& TTriangleListAncestor::operator=(const TDataTriangleList& dataTriangleList)
{
	Data = dataTriangleList;
	NeedRefreshBuffer = true;

	return *this;
}

TTriangleListAncestor::~TTriangleListAncestor()
{
}


#ifdef TARGET_WIN32

TTriangleList::TTriangleList()
	: TTriangleListAncestor()
	, NeedPrepareBufferObjects(true)
{
}

TTriangleList::~TTriangleList()
{
}

void TTriangleList::RefreshBuffer()
{
	AssertIfInMainThread();


	if (NeedPrepareBufferObjects && Data.Vec2CoordArr.size() > 0 && Data.Vec3CoordArr.size() > 0)
	{

		NeedPrepareBufferObjects = false;

        BOOST_FOREACH(auto& i, Data.Vec2CoordArr)
		{
			if (VertBufferArr[i.first] == std::shared_ptr<VBOObject>())
			{
				VertBufferArr[i.first] = std::shared_ptr<VBOObject>(new VBOObject);
				glBindBuffer(GL_ARRAY_BUFFER, VertBufferArr[i.first]->Buffer);

				//What to do if buffer becomes empty?
				if (i.second.size() > 0)
				{
					glBufferData(GL_ARRAY_BUFFER, i.second.size() * 8, &i.second[0], GL_STATIC_DRAW);
				}
				else
				{
					glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW);
				}
			}
		
		}

        BOOST_FOREACH(auto& i, Data.Vec3CoordArr)
		//for (std::map<std::string, std::vector<vec3> >::iterator i = Data.Vec3CoordArr.begin(); i != Data.Vec3CoordArr.end(); ++i )
		{
			if (VertBufferArr[i.first] == std::shared_ptr<VBOObject>())
			{
				VertBufferArr[i.first] = std::shared_ptr<VBOObject>(new VBOObject);
				glBindBuffer(GL_ARRAY_BUFFER, VertBufferArr[i.first]->Buffer);
				
				//What to do if buffer becomes empty?
				if (i.second.size() > 0)
				{
					glBufferData(GL_ARRAY_BUFFER, i.second.size() * 12, &i.second[0], GL_STATIC_DRAW);
				}
				else
				{
					glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW);
				}
			}

		
		}

	}

	
	BOOST_FOREACH(auto& i, Data.Vec2CoordArr)
    {
		glBindBuffer(GL_ARRAY_BUFFER, VertBufferArr[i.first]->Buffer);
		RefreshAttribBuffer2fv(i.first, Data.Vec2CoordArr);
	}

	BOOST_FOREACH(auto& i, Data.Vec3CoordArr)
    {
		glBindBuffer(GL_ARRAY_BUFFER, VertBufferArr[i.first]->Buffer);
		RefreshAttribBuffer3fv(i.first, Data.Vec3CoordArr);
	}


}

#endif
    
void FillVertexCoordVec(std::vector<vec3>& coordVec, int pos, vec2 posFrom, vec2 posTo)
{
    vec2 pos1 = posFrom;
	vec2 pos2 = vec2(posFrom.v[0], posTo.v[1]);
	vec2 pos3 = posTo;
	vec2 pos4 = vec2(posTo.v[0], posFrom.v[1]);
    
    coordVec[pos] = vec3(pos1, 0);
    coordVec[pos+1] = vec3(pos2, 0);
    coordVec[pos+2] = vec3(pos3, 0);
    coordVec[pos+3] = vec3(pos3, 0);
    coordVec[pos+4] = vec3(pos4, 0);
    coordVec[pos+5] = vec3(pos1, 0);
 
}
    
void FillTexCoordVec(std::vector<vec2>& coordVec, int pos, vec2 texCoordFrom, vec2 texCoordTo)
{
    vec2 tex1 = texCoordFrom;
	vec2 tex2 = vec2(texCoordFrom.v[0], texCoordTo.v[1]);
	vec2 tex3 = texCoordTo;
	vec2 tex4 = vec2(texCoordTo.v[0], texCoordFrom.v[1]);
    
    
    coordVec[pos] = tex1;
    coordVec[pos+1] = tex2;
    coordVec[pos+2] = tex3;
    coordVec[pos+3] = tex3;
    coordVec[pos+4] = tex4;
    coordVec[pos+5] = tex1;

}


std::vector<vec3> MakeVertexCoordVec(vec2 posFrom, vec2 posTo)
{
	std::vector<vec3> r;
    r.resize(6);
    
    FillVertexCoordVec(r, 0, posFrom, posTo);

	return r;
}

std::vector<vec2> MakeTexCoordVec(vec2 texCoordFrom, vec2 texCoordTo)
{
	std::vector<vec2> r;
    r.resize(6);
	FillTexCoordVec(r, 0, texCoordFrom, texCoordTo);
    
    return r;
}


TDataTriangleList MakeDataTriangleList(vec2 posFrom, vec2 posTo, vec2 texCoordFrom, vec2 texCoordTo)
{
	TDataTriangleList triangleList;

	triangleList.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB] = MakeVertexCoordVec(posFrom, posTo);
	triangleList.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB] = MakeTexCoordVec(texCoordFrom, texCoordTo);

	return triangleList;
}

void MoveDataTriangleList(TDataTriangleList& triangleList, vec3 shift)
{
	if (triangleList.Vec3CoordArr.count(CONST_STRING_POSITION_ATTRIB) == 0)
	{
		return;
	}

    BOOST_FOREACH(auto& i, triangleList.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB])
	{
		i += shift;
	}
}

void RotateDataTriangleList(TDataTriangleList& triangleList, const mat3& m)
{
	if (triangleList.Vec3CoordArr.count(CONST_STRING_POSITION_ATTRIB) == 0)
	{
		return;
	}

    BOOST_FOREACH(vec3& i, triangleList.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB])
	{
		i = i * m;
	}
}

void ScaleDataTriangleList(TDataTriangleList& triangleList, float scale)
{
	if (triangleList.Vec3CoordArr.count(CONST_STRING_POSITION_ATTRIB) == 0)
	{
		return;
	}

    BOOST_FOREACH(vec3& i, triangleList.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB])
	{
		i.v[0] *= scale;
		i.v[1] *= scale;
		i.v[2] *= scale;
	}
}

void ScaleDataTriangleList(TDataTriangleList& triangleList, vec3 scaleVec)
{
	if (triangleList.Vec3CoordArr.count(CONST_STRING_POSITION_ATTRIB) == 0)
	{
		return;
	}

    BOOST_FOREACH(vec3& i, triangleList.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB])
	{
		i.v[0] *= scaleVec.v[0];
		i.v[1] *= scaleVec.v[1];
		i.v[2] *= scaleVec.v[2];
	}
}

TDataTriangleList& ClearDataTriangleList(TDataTriangleList& triangleList)
{

	BOOST_FOREACH(auto& i, triangleList.Vec2CoordArr)
	{
		i.second.clear();
	}

	BOOST_FOREACH(auto& i, triangleList.Vec3CoordArr)
	{
		i.second.clear();
	}

	return triangleList;
}

TDataTriangleList& InsertIntoDataTriangleList(TDataTriangleList& triangleList, const std::vector<vec3>& vertexArr, const std::vector<vec2>& texCoordArr)
{
	triangleList.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].insert(triangleList.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].end(), vertexArr.begin(), vertexArr.end());
	triangleList.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].insert(triangleList.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].end(), texCoordArr.begin(), texCoordArr.end());
				
	return triangleList;
}


TTriangleList MakeTriangleList(vec2 posFrom, vec2 posTo, vec2 texCoordFrom, vec2 texCoordTo)
{
	TTriangleList triangleList;

	triangleList.Data = MakeDataTriangleList(posFrom, posTo, texCoordFrom, texCoordTo);

	triangleList.RefreshBuffer();

	return triangleList;

}
    
void Replace6PointsInTriangleList(TDataTriangleList& triangleList, int pos, vec2 posFrom, vec2 posTo, vec2 texCoordFrom, vec2 texCoordTo)
{
    FillVertexCoordVec(triangleList.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB], pos, posFrom, posTo);
	FillTexCoordVec(triangleList.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB], pos, texCoordFrom, texCoordTo);
}



void CheckGlError(const std::string& msg)
{
	AssertIfInMainThread();

	cardinal error = glGetError();
	
	if (error != GL_NO_ERROR)
	{
		if (msg == "")
		{
			throw ErrorToLog("Gl error = "+tostr(error));
		}
		else
		{
			throw ErrorToLog("Gl error = "+tostr(error) + "; with message = " + msg);
		}
	}
}


} //namespace SE