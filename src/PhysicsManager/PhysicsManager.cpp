#include "include/PhysicsManager/PhysicsManager.h"
#include "include/ModelManager/NewModelManager.h"
#include "include/ShaderManager/ShaderManager.h"

namespace SE
{

	const int CONST_MAX_VOLUME_STACK = 64;

void TVolumeBody::AssignFromFlexModel(TFlexModel& flexModel)
{
	SmpTriangleArr.clear();

	TDataTriangleList& dataTriangleList = flexModel.GetDataTriangleList();

	if (dataTriangleList.Vec3CoordArr.count(CONST_STRING_POSITION_ATTRIB) == 0)
	{
		throw ErrorToLog("Error in AssignFromFlexModel - empty flexModel!");
	}

	if (dataTriangleList.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].size() == 0)
	{
		throw ErrorToLog("Error in AssignFromFlexModel - empty flexModel! size == 0!");
	}

	DimensionalMinPos = dataTriangleList.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB][0];
	DimensionalMaxPos = dataTriangleList.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB][0];


	for (size_t j = 0; j < dataTriangleList.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB].size(); j+=3)
	{
		TSmpTriangle smpTriangle; 

		smpTriangle.p[0] = dataTriangleList.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB][j];
		smpTriangle.p[1] = dataTriangleList.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB][j + 1];
		smpTriangle.p[2] = dataTriangleList.Vec3CoordArr[CONST_STRING_POSITION_ATTRIB][j + 2];

		smpTriangle.v = smpTriangle.p[1] - smpTriangle.p[0];
		smpTriangle.w = smpTriangle.p[2] - smpTriangle.p[1];

		smpTriangle.n = CrossProduct(smpTriangle.v, smpTriangle.w);

		SmpTriangleArr.push_back(smpTriangle);

		DimensionalMinPos.v[0] = min(min(min(DimensionalMinPos.v[0], smpTriangle.p[0].v[0]), smpTriangle.p[1].v[0]), smpTriangle.p[2].v[0]);
		DimensionalMinPos.v[1] = min(min(min(DimensionalMinPos.v[1], smpTriangle.p[0].v[1]), smpTriangle.p[1].v[1]), smpTriangle.p[2].v[1]);
		DimensionalMinPos.v[2] = min(min(min(DimensionalMinPos.v[2], smpTriangle.p[0].v[2]), smpTriangle.p[1].v[2]), smpTriangle.p[2].v[2]);

		DimensionalMaxPos.v[0] = max(max(max(DimensionalMaxPos.v[0], smpTriangle.p[0].v[0]), smpTriangle.p[1].v[0]), smpTriangle.p[2].v[0]);
		DimensionalMaxPos.v[1] = max(max(max(DimensionalMaxPos.v[1], smpTriangle.p[0].v[1]), smpTriangle.p[1].v[1]), smpTriangle.p[2].v[1]);
		DimensionalMaxPos.v[2] = max(max(max(DimensionalMaxPos.v[2], smpTriangle.p[0].v[2]), smpTriangle.p[1].v[2]), smpTriangle.p[2].v[2]);
	}
	
}

void TVolumeBody::PushData()
{
	if (SmpTriangleArrStack.size() >= CONST_MAX_VOLUME_STACK)
	{
		throw ErrorToLog("TVolumeBody::PushData - stack overflow");
	}

	SmpTriangleArrStack.push(SmpTriangleArr);
}

void TVolumeBody::PopData()
{
	if (SmpTriangleArrStack.size() <= 0)
	{
		throw ErrorToLog("TVolumeBody::PushData - stack underflow");
	}

	SmpTriangleArr = SmpTriangleArrStack.top();

	SmpTriangleArrStack.pop();
}


void TVolumeBody::Move(const vec3& v)
{
	BOOST_FOREACH(TSmpTriangle& smpTriangle, SmpTriangleArr)
	{
		smpTriangle.p[0] -= v;
		smpTriangle.p[1] -= v;
		smpTriangle.p[2] -= v;
	}
}

void TVolumeBody::Rotate(const mat3& r)
{
	throw ErrorToLog("TVolumeBody::Rotate not implemented yet!");
}

void TVolumeBody::Scale(float s)
{
	BOOST_FOREACH(TSmpTriangle& smpTriangle, SmpTriangleArr)
	{
		smpTriangle.p[0] *= s;
		smpTriangle.p[1] *= s;
		smpTriangle.p[2] *= s;
	}
}

void TVolumeBody::Scale(const vec3& s)
{
	BOOST_FOREACH(TSmpTriangle& smpTriangle, SmpTriangleArr)
	{
		smpTriangle.p[0] *= s.v[0];
		smpTriangle.p[1] *= s.v[1];
		smpTriangle.p[2] *= s.v[2];
	}
}



bool TVolumeBody::CheckCollision(const vec3& a, const vec3& b)
{
	
	for (int i=0; i<3; i++)
	{
		if (max(a.v[i], b.v[i]) < DimensionalMinPos.v[i])
		{
			return false;
		}

		if (min(a.v[i], b.v[i]) > DimensionalMaxPos.v[i])
		{
			return false;
		}
	}

	BOOST_FOREACH(TSmpTriangle& smpTriangle, SmpTriangleArr)
	{
		if (LineCouldCrossTriangle(a, b, smpTriangle))
		{
			if (LineCrossTriangle(a, b, smpTriangle) == 1)
			{
				return true;
			}
		}
	}

	return false;
}



} //namespace SE