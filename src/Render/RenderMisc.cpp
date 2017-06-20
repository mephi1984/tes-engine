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


#ifdef TARGET_WINDOWS_UNIVERSAL

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
	BOOST_FOREACH(auto& i, dataTriangleList.Vec4CoordArr)
	{
		if (Vec4CoordArr.count(i.first) == 0)
		{
			Vec4CoordArr[i.first] = i.second;
		}
		else
		{
			Vec4CoordArr[i.first].insert(Vec4CoordArr[i.first].end(), i.second.begin(), i.second.end());
		}
	}

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
{
}

TTriangleListAncestor::TTriangleListAncestor(const TTriangleListAncestor& c)
{
	Data = c.Data;
}

TTriangleListAncestor& TTriangleListAncestor::operator=(const TTriangleListAncestor& c)
{
	Data = c.Data;
	RefreshBuffer();
	return *this;
}

TTriangleListAncestor::TTriangleListAncestor(const TDataTriangleList& dataTriangleList)
{
	Data = dataTriangleList;
}

TTriangleListAncestor& TTriangleListAncestor::operator=(const TDataTriangleList& dataTriangleList)
{
	Data = dataTriangleList;
	RefreshBuffer();

	return *this;
}

TTriangleListAncestor::~TTriangleListAncestor()
{
}
    
    
    bool TTriangleListAncestor::VertBufferArrEmpty()
    {
        return false;
    }

#ifdef TARGET_WIN32

TTriangleList::TTriangleList()
	: TTriangleListAncestor()
	, NeedPrepareBufferObjects(true)
	, NeedRefreshBuffer(false)
{
}

TTriangleList::~TTriangleList()
{
}

void TTriangleList::InnerRefreshBuffer()
{
	AssertIfInMainThread();

	if (NeedPrepareBufferObjects /*&& Data.Vec2CoordArr.size() > 0 */&& Data.Vec3CoordArr.size() > 0 && Data.Vec4CoordArr.size() > 0)
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


		BOOST_FOREACH(auto& i, Data.Vec4CoordArr)
		{
			if (VertBufferArr[i.first] == std::shared_ptr<VBOObject>())
			{
				VertBufferArr[i.first] = std::shared_ptr<VBOObject>(new VBOObject);
				glBindBuffer(GL_ARRAY_BUFFER, VertBufferArr[i.first]->Buffer);

				//What to do if buffer becomes empty?
				if (i.second.size() > 0)
				{
					glBufferData(GL_ARRAY_BUFFER, i.second.size() * 16, &i.second[0], GL_STATIC_DRAW);
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

	BOOST_FOREACH(auto& i, Data.Vec4CoordArr)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VertBufferArr[i.first]->Buffer);
		RefreshAttribBuffer4fv(i.first, Data.Vec4CoordArr);
	}

	NeedRefreshBuffer = false;
}


void TTriangleList::RefreshBuffer()
{
	if (!NeedRefreshBuffer)
	{
		NeedRefreshBuffer = true;
		PerformInMainThreadAsync(std::bind(&TTriangleList::InnerRefreshBuffer, this));
	}
}
    
    
    bool TTriangleList::VertBufferArrEmpty()
    {
        return VertBufferArr.size() == 0;
    }

#endif



#ifdef TARGET_WINDOWS_UNIVERSAL

TTriangleList::TTriangleList()
	: TTriangleListAncestor()
	, NeedPrepareBufferObjects(true)
	, NeedRefreshBuffer(false)
{
}

TTriangleList::~TTriangleList()
{
}

void TTriangleList::InnerRefreshBuffer()
{
	AssertIfInMainThread();

	if (NeedPrepareBufferObjects && (Data.Vec2CoordArr.size() > 0 || Data.Vec3CoordArr.size() > 0 || Data.Vec4CoordArr.size() > 0))
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


		BOOST_FOREACH(auto& i, Data.Vec4CoordArr)
		{
			if (VertBufferArr[i.first] == std::shared_ptr<VBOObject>())
			{
				VertBufferArr[i.first] = std::shared_ptr<VBOObject>(new VBOObject);
				glBindBuffer(GL_ARRAY_BUFFER, VertBufferArr[i.first]->Buffer);

				//What to do if buffer becomes empty?
				if (i.second.size() > 0)
				{
					glBufferData(GL_ARRAY_BUFFER, i.second.size() * 16, &i.second[0], GL_STATIC_DRAW);
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

	BOOST_FOREACH(auto& i, Data.Vec4CoordArr)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VertBufferArr[i.first]->Buffer);
		RefreshAttribBuffer4fv(i.first, Data.Vec4CoordArr);
	}

	NeedRefreshBuffer = false;
}


void TTriangleList::RefreshBuffer()
{
	if (!NeedRefreshBuffer)
	{
		NeedRefreshBuffer = true;
		PerformInMainThreadAsync(std::bind(&TTriangleList::InnerRefreshBuffer, this));
	}
}
    
    
    bool TTriangleList::VertBufferArrEmpty()
    {
        return VertBufferArr.size() == 0;
    }

#endif
    
void FillVertexCoordVec(std::vector<Vector3f>& coordVec, int pos, Vector2f posFrom, Vector2f posTo, float zLevel)
{
    Vector2f pos1 = posFrom;
	Vector2f pos2 = Vector2f(posFrom(0), posTo(1));
	Vector2f pos3 = posTo;
	Vector2f pos4 = Vector2f(posTo(0), posFrom(1));
    
    coordVec[pos] << pos1, zLevel;
	coordVec[pos + 1] << pos2, zLevel;
	coordVec[pos + 2] << pos3, zLevel;
	coordVec[pos + 3] << pos3, zLevel;
	coordVec[pos + 4] << pos4, zLevel;
	coordVec[pos + 5] << pos1, zLevel;

}
    
void FillTexCoordVec(std::vector<Vector2f>& coordVec, int pos, Vector2f texCoordFrom, Vector2f texCoordTo)
{
    Vector2f tex1 = texCoordFrom;
	Vector2f tex2 = Vector2f(texCoordFrom(0), texCoordTo(1));
	Vector2f tex3 = texCoordTo;
	Vector2f tex4 = Vector2f(texCoordTo(0), texCoordFrom(1));
    
    
    coordVec[pos] = tex1;
    coordVec[pos+1] = tex2;
    coordVec[pos+2] = tex3;
    coordVec[pos+3] = tex3;
    coordVec[pos+4] = tex4;
    coordVec[pos+5] = tex1;

}

void FillVertexCoordVec_4Points(std::vector<Vector3f>& coordVec, int pos, Vector2f pos1, Vector2f pos2, Vector2f pos3, Vector2f pos4)
{
	coordVec[pos] << pos1, 0;// = Vector3f(pos1, 0);
	coordVec[pos + 1] << pos2, 0; // = Vector3f(pos2, 0);
	coordVec[pos + 2] << pos3, 0; // = Vector3f(pos3, 0);
	coordVec[pos + 3] << pos3, 0; // = Vector3f(pos3, 0);
	coordVec[pos + 4] << pos4, 0;// = Vector3f(pos4, 0);
	coordVec[pos + 5] << pos1, 0; // = Vector3f(pos1, 0);
}

void FillTexCoordVec_4Points(std::vector<Vector2f>& coordVec, int pos, Vector2f texCoord1, Vector2f texCoord2, Vector2f texCoord3, Vector2f texCoord4)
{
	coordVec[pos] = texCoord1;
    coordVec[pos+1] = texCoord2;
    coordVec[pos+2] = texCoord3;
    coordVec[pos+3] = texCoord3;
    coordVec[pos+4] = texCoord4;
    coordVec[pos+5] = texCoord1;
}


std::vector<Vector4f> MakeColorCoordVec(Vector4f color)
{
	std::vector<Vector4f> r;
	r.resize(6);

	r[0] = color;
	r[1] = color;
	r[2] = color;
	r[3] = color;
	r[4] = color;
	r[5] = color;

	return r;
}

std::vector<Vector3f> MakeVertexCoordVec(Vector2f posFrom, Vector2f posTo, float zLevel)
{
	std::vector<Vector3f> r;
    r.resize(6);
    
    FillVertexCoordVec(r, 0, posFrom, posTo, zLevel);

	return r;
}

std::vector<Vector2f> MakeTexCoordVec(Vector2f texCoordFrom, Vector2f texCoordTo)
{
	std::vector<Vector2f> r;
    r.resize(6);
	FillTexCoordVec(r, 0, texCoordFrom, texCoordTo);
    
    return r;
}


TDataTriangleList MakeDataTriangleList(Vector2f posFrom, Vector2f posTo, Vector4f color, float zLevel, Vector2f texCoordFrom, Vector2f texCoordTo)
{
	TDataTriangleList triangleList;

	triangleList.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB] = MakeColorCoordVec(color);
	triangleList.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB] = MakeVertexCoordVec(posFrom, posTo, zLevel);
	triangleList.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB] = MakeTexCoordVec(texCoordFrom, texCoordTo);

	return triangleList;
}

void MoveDataTriangleList(TDataTriangleList& triangleList, Vector3f shift)
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

void RotateDataTriangleList(TDataTriangleList& triangleList, const Matrix3f& m)
{
	if (triangleList.Vec3CoordArr.count(CONST_STRING_POSITION_ATTRIB) == 0)
	{
		return;
	}

    BOOST_FOREACH(Vector3f& i, triangleList.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB])
	{
		//i = i * m;
		i = m * i;
	}
}

void ScaleDataTriangleList(TDataTriangleList& triangleList, float scale)
{
	if (triangleList.Vec3CoordArr.count(CONST_STRING_POSITION_ATTRIB) == 0)
	{
		return;
	}

    BOOST_FOREACH(Vector3f& i, triangleList.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB])
	{
		i(0) *= scale;
		i(1) *= scale;
		i(2) *= scale;
	}
}

void ScaleDataTriangleList(TDataTriangleList& triangleList, Vector3f scaleVec)
{
	if (triangleList.Vec3CoordArr.count(CONST_STRING_POSITION_ATTRIB) == 0)
	{
		return;
	}

    BOOST_FOREACH(Vector3f& i, triangleList.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB])
	{
		i(0) *= scaleVec(0);
		i(1) *= scaleVec(1);
		i(2) *= scaleVec(2);
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

TDataTriangleList& InsertIntoDataTriangleList(TDataTriangleList& triangleList, const std::vector<Vector3f>& vertexArr, const std::vector<Vector2f>& texCoordArr, const std::vector<Vector4f>& colorArr)
{

	triangleList.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB].insert(triangleList.Vec4CoordArr[CONST_STRING_COLOR_ATTRIB].end(), colorArr.begin(), colorArr.end());
	triangleList.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].insert(triangleList.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].end(), vertexArr.begin(), vertexArr.end());
	triangleList.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].insert(triangleList.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB].end(), texCoordArr.begin(), texCoordArr.end());
				
	return triangleList;
}


TTriangleList MakeTriangleList(Vector2f posFrom, Vector2f posTo, Vector4f color, float zLevel, Vector2f texCoordFrom, Vector2f texCoordTo)
{
	TTriangleList triangleList;

	triangleList.Data = MakeDataTriangleList(posFrom, posTo, color, zLevel, texCoordFrom, texCoordTo);

	triangleList.RefreshBuffer();

	return triangleList;

}
    
void Replace6PointsInTriangleList(TDataTriangleList& triangleList, int pos, Vector2f posFrom, Vector2f posTo, Vector2f texCoordFrom, Vector2f texCoordTo)
{
    FillVertexCoordVec(triangleList.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB], pos, posFrom, posTo);
	FillTexCoordVec(triangleList.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB], pos, texCoordFrom, texCoordTo);
}

void Replace6PointsInTriangleList_4Points(TDataTriangleList& triangleList, int pos, Vector2f pos1, Vector2f pos2, Vector2f pos3, Vector2f pos4, Vector2f texCoord1, Vector2f texCoord2, Vector2f texCoord3, Vector2f texCoord4)
{
	FillVertexCoordVec_4Points(triangleList.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB], pos, pos1, pos2, pos3, pos4);
	FillTexCoordVec_4Points(triangleList.Vec2CoordArr[CONST_STRING_TEXCOORD_ATTRIB], pos, texCoord1, texCoord2, texCoord3, texCoord4);
}



void CheckGlError(const std::string& msg)
{
	AssertIfInMainThread();

	size_t error = glGetError();
	
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
