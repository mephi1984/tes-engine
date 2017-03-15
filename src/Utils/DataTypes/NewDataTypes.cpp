#include "include/Utils/DataTypes/NewDataTypes.h"
#include "include/Utils/DataTypes/DataTypes.h"
#include "include/Utils/ErrorTypes/ErrorTypes.h"

namespace SE
{
	/*
Matrix3f TransposeMatrix(const Matrix3f& m)
{
	Matrix3f r;
	r.m[0] = m.m[0];
	r.m[1] = m.m[3];
	r.m[2] = m.m[6];
	r.m[3] = m.m[1];
	r.m[4] = m.m[4];
	r.m[5] = m.m[7];
	r.m[6] = m.m[2];
	r.m[7] = m.m[5];
	r.m[8] = m.m[8];
	return r;
}

Matrix3f InverseMatrix(const Matrix3f& m)
{
	float d;
	Matrix3f r;
    
	d = m.m[0]*(m.m[4]*m.m[8] - m.m[5]*m.m[7]);
	d -= m.m[1]*(m.m[3]*m.m[8] - m.m[6]*m.m[5]);
	d += m.m[2]*(m.m[3]*m.m[7] - m.m[6]*m.m[4]);
    
	if (IsFloatEqual(d,0.0f))
	{
		throw ErrorToLog("Error: matrix cannot be inversed!");
        
	}
	else
	{
        
        r.m[0] = (m.m[4]*m.m[8] - m.m[5]*m.m[7])/d;
        r.m[1] = - (m.m[1]*m.m[8] - m.m[2]*m.m[7])/d;
        r.m[2] = (m.m[1]*m.m[5] - m.m[2]*m.m[4])/d;
        
        r.m[3] = - (m.m[3]*m.m[8] - m.m[5]*m.m[6])/d;
        r.m[4] = (m.m[0]*m.m[8] - m.m[2]*m.m[6])/d;
        r.m[5] = - (m.m[0]*m.m[5] - m.m[2]*m.m[3])/d;
        
        r.m[6] = (m.m[3]*m.m[7] - m.m[6]*m.m[4])/d;
        r.m[7] = - (m.m[0]*m.m[7] - m.m[6]*m.m[1])/d;
        r.m[8] = (m.m[0]*m.m[4] - m.m[1]*m.m[3])/d;
        
	};
	return r;
}

Matrix3f CreateZRotationMatrix(float angle)
{
	Matrix3f result = Matrix3f::Identity();

	result.m[0] = cosf(angle);
	result.m[1] = -sinf(angle);
	result.m[3] = sinf(angle);
	result.m[4] = cosf(angle);

	return result;
}*/


} //namespace SE