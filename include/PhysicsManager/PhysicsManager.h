#ifndef PHYSICS_MANAGER_H_INCLUDED
#define PHYSICS_MANAGER_H_INCLUDED


#include "include/Utils/Utils.h"
#include "include/ModelManager/NewModelManager.h"

namespace SE
{

class TVolumeBody
{
protected:

	std::stack<std::vector<TSmpTriangle>> SmpTriangleArrStack;

	std::vector<TSmpTriangle> SmpTriangleArr;

	vec3 DimensionalMinPos;
	vec3 DimensionalMaxPos;

public:

	void PushData();
	void PopData();

	void Move(const vec3& v);
	void Rotate(const mat3& r);
	void Scale(float s);
	void Scale(const vec3& s);

	void AssignFromFlexModel(TFlexModel& flexModel);

	bool CheckCollision(const vec3& a, const vec3& b);
};


class TPhysicsManager
{
protected:
public:
};



} //namespace SE

#endif


