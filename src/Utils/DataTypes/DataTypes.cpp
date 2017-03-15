#include "include/Utils/DataTypes/DataTypes.h"
#include "include/Utils/ErrorTypes/ErrorTypes.h"

#include "boost/algorithm/string.hpp"
namespace SE
{

	/*
Matrix4f InverseModelViewMatrix(const Matrix4f& m)
{
	Matrix4f r;
	r.m[0] = m.m[0];
	r.m[1] = m.m[4];
	r.m[2] = m.m[8];
	r.m[3] = 0.0f;
	r.m[4] = m.m[1];
	r.m[5] = m.m[5];
	r.m[6] = m.m[9];
	r.m[7] = 0.0f;
	r.m[8] = m.m[2];
	r.m[9] = m.m[6];
	r.m[10] = m.m[10];
	r.m[11] = 0.0f;

	r.m[12] = - (m.m[12]*r.m[0] + m.m[13]*r.m[4] + m.m[14]*r.m[8]);
	r.m[13] = - (m.m[12]*r.m[1] + m.m[13]*r.m[5] + m.m[14]*r.m[9]);
	r.m[14] = - (m.m[12]*r.m[2] + m.m[13]*r.m[6] + m.m[14]*r.m[10]);
	r.m[15] = 1.0f;

	return r;

}



Matrix4f SetToNormalMatrix(const Matrix4f& m)
{
	Matrix4f r;
	r.m[0] = m.m[0];
	r.m[1] = m.m[4];
	r.m[2] = m.m[8];
	r.m[3] = 0.0f;
	r.m[4] = m.m[1];
	r.m[5] = m.m[5];
	r.m[6] = m.m[9];
	r.m[7] = 0.0f;
	r.m[8] = m.m[2];
	r.m[9] = m.m[6];
	r.m[10] = m.m[10];
	r.m[11] = 0.0f;

	r.m[12] = 0.0f;
	r.m[13] = 0.0f;
	r.m[14] = 0.0f;
	r.m[15] = 1.0f;

	return r;
}
*/
Matrix4f MakeOrthoMatrix(float width, float height)
{
	Matrix4f r;
	r(0) = 2.f / width;
	r(1) = 0;
	r(2) = 0;
	r(3) = 0;
	
	r(4) = 0;
	r(5) = 2.f / height;
	r(6) = 0;
	r(7) = 0;
	
	r(8) = 0;
	r(9) = 0;
	r(10) = -1;
	r(11) = 0;
	
	r(12) = -1;
	r(13) = -1;
	r(14) = 0;
	r(15) = 1;
	
	return r;
}

Matrix4f MakeFrustumMatrix(float left, float right, float bottom, float top, float nearVal, float farVal)
{
	Matrix4f r;

	float a = (right + left) / (right - left);
	
	float b = (top + bottom) / (top - bottom);

	float c = - (farVal + nearVal) / (farVal - nearVal);

	float d = - (2.f * farVal * nearVal) / (farVal - nearVal);


	r(0) = 2.f*nearVal / (right - left); r(4) = 0.f; r(8) = a; r(12) = 0.f;
	r(1) = 0.f; r(5) = 2.f*nearVal / (top - bottom); r(9) = b; r(13) = 0.f;
	r(2) = 0.f; r(6) = 0.f; r(10) = c; r(14) = d;
	r(3) = 0.f; r(7) = 0.f; r(11) = -1; r(15) = 0.f;
	
	return r;
}

Matrix4f MakePerspectiveMatrix(float angle, float aspect, float zNear, float zFar)
{
	//angle = angle * pi / 180.f;

	float top = zNear * tan(angle / 2.f);
	float bottom = -top;

	float left = bottom * aspect;
	float right = top * aspect;
	

	return MakeFrustumMatrix(left, right, bottom, top, zNear, zFar);
}
/*
Matrix4f MultMatrixMatrix(const Matrix4f& m1, const Matrix4f& m2)
{
    Matrix4f r;

	r.m[0] = m1.m[0]*m2.m[0] + m1.m[4]*m2.m[1] + m1.m[8]*m2.m[2] + m1.m[12]*m2.m[3];
	r.m[1] = m1.m[1]*m2.m[0] + m1.m[5]*m2.m[1] + m1.m[9]*m2.m[2] + m1.m[13]*m2.m[3];
	r.m[2] = m1.m[2]*m2.m[0] + m1.m[6]*m2.m[1] + m1.m[10]*m2.m[2] + m1.m[14]*m2.m[3];
	r.m[3] = m1.m[3]*m2.m[0] + m1.m[7]*m2.m[1] + m1.m[11]*m2.m[2] + m1.m[15]*m2.m[3];

	r.m[4] = m1.m[0]*m2.m[4] + m1.m[4]*m2.m[5] + m1.m[8]*m2.m[6] + m1.m[12]*m2.m[7];
	r.m[5] = m1.m[1]*m2.m[4] + m1.m[5]*m2.m[5] + m1.m[9]*m2.m[6] + m1.m[13]*m2.m[7];
	r.m[6] = m1.m[2]*m2.m[4] + m1.m[6]*m2.m[5] + m1.m[10]*m2.m[6] + m1.m[14]*m2.m[7];
	r.m[7] = m1.m[3]*m2.m[4] + m1.m[7]*m2.m[5] + m1.m[11]*m2.m[6] + m1.m[15]*m2.m[7];


	r.m[8] = m1.m[0]*m2.m[8] + m1.m[4]*m2.m[9] + m1.m[8]*m2.m[10] + m1.m[12]*m2.m[11];
	r.m[9] = m1.m[1]*m2.m[8] + m1.m[5]*m2.m[9] + m1.m[9]*m2.m[10] + m1.m[13]*m2.m[11];
	r.m[10] = m1.m[2]*m2.m[8] + m1.m[6]*m2.m[9] + m1.m[10]*m2.m[10] + m1.m[14]*m2.m[11];
	r.m[11] = m1.m[3]*m2.m[8] + m1.m[7]*m2.m[9] + m1.m[11]*m2.m[10] + m1.m[15]*m2.m[11];
	
	r.m[12] = m1.m[0]*m2.m[12] + m1.m[4]*m2.m[13] + m1.m[8]*m2.m[14] + m1.m[12]*m2.m[15];
	r.m[13] = m1.m[1]*m2.m[12] + m1.m[5]*m2.m[13] + m1.m[9]*m2.m[14] + m1.m[13]*m2.m[15];
	r.m[14] = m1.m[2]*m2.m[12] + m1.m[6]*m2.m[13] + m1.m[10]*m2.m[14] + m1.m[14]*m2.m[15];
	r.m[15] = m1.m[3]*m2.m[12] + m1.m[7]*m2.m[13] + m1.m[11]*m2.m[14] + m1.m[15]*m2.m[15];

    return r;
}
*/

bool IsFloatEqual(float a, float b)
{
	return (fabs(b-a) <= CONST_EPSILON) ? true : false;
}

/*
bool LineCouldCrossTriangle(const Vector3f& a, const Vector3f& b, const TSmpTriangle& tr)
{
	for (int i=0; i<3; i++)
	{
		float min_tr = min(min(tr.p[0].v[i], tr.p[1].v[i]), tr.p[2].v[i]);
	
		if (max(a.v[i], b.v[i]) < min_tr)
		{
			return false;
		}

		float max_tr = max(max(tr.p[0].v[i], tr.p[1].v[i]), tr.p[2].v[i]);
	
		if (min(a.v[i], b.v[i]) > max_tr)
		{
			return false;
		}
	}

	return true;

}


int LineCrossTriangle(const Vector3f& a, const Vector3f& b, const TSmpTriangle& tr)
{
	Vector3f dummy;
	return LineCrossTriangle(a, b, tr, dummy);
}


int LineCrossTriangle(const Vector3f& a, const Vector3f& b, const TSmpTriangle& tr, Vector3f& CrossPoint)
{

	Vector3f h, q, t, vx;
	Matrix3f M;

	h = b - a;

	if (IsFloatEqual(DotProduct(tr.n, h), 0.0f))
	{
		return -1;
	}

	M.m[0] = h(0);
	M.m[3] = h(1);
	M.m[6] = h(2);

	M.m[1] = -tr.v(0);
	M.m[4] = -tr.v(1);
	M.m[7] = -tr.v(2);

	M.m[2] = -tr.w(0);
	M.m[5] = -tr.w(1);
	M.m[8] = -tr.w(2);

	M = InverseMatrix(M);

	vx = tr.p[0] - a;

	t = vx * M;

	if ((t(0)<0)||(t(0)>1))
	{
		return 0;
	}

	q = a + h * t(0);
	
	CrossPoint = q;

	return PointInTriangle(q, tr);


}

int PointInTriangle(const Vector3f& q, const TSmpTriangle& tr)
{
	Vector3f v,h;
	float nf2[3];
	Vector4f quat, inv_quat, p;

	TSmpTriangle trx;

	float sina,cosa,cos2a;

	int i;

	h = Vector3f(0.0f, 1.0f, 0.0f);
	

	if (IsFloatEqual(tr.n(0),0.0f) && (!IsFloatEqual(tr.n(1),0.0f)) && IsFloatEqual(tr.n(2),0.0f))
	{
		quat = Vector4f(1.0f, 0.0f, 0.0f, 0.0f);
	}
	else
	{
		v = CrossProduct(tr.n,h);

		cos2a = DotProduct(Normalize(tr.n), h);

		cos2a *= 0.5f;
		cosa = sqrtf(0.5f + cos2a);
		sina = sqrtf(0.5f - cos2a);

		v = Normalize(v);

		quat(0) = v(0)*sina;
		quat(1) = v(1)*sina;
		quat(2) = v(2)*sina;
		quat.v[3] = cosa;

	}

	inv_quat(0) = - quat(0);
	inv_quat(1) = - quat(1);
	inv_quat(2) = - quat(2);
	inv_quat.v[3] = quat.v[3];

	for (i=0; i<3; i++)
	{
		p = Vector4f(tr.p[i]);

		p = quat * p * inv_quat;

		trx.p[i] = Vector3f(p);
		
	};

	p(0) = q(0);
	p(1) = q(1);
	p(2) = q(2);
	p.v[3] = 0.0f;

	p = quat * p * inv_quat;


	for (i=0;i<2;i++)
		nf2[i] = (p(0)-trx.p[i](0))*(trx.p[i+1](2)-trx.p[i](2))-(p(2)-trx.p[i](2))*(trx.p[i+1](0)-trx.p[i](0));

	//Not to go over array:
	nf2[2] = (p(0)-trx.p[2](0))*(trx.p[0](2)-trx.p[2](2))-(p(2)-trx.p[2](2))*(trx.p[0](0)-trx.p[2](0));

	if (((nf2[0]>=0.0f) && (nf2[1]>=0.0f) && (nf2[2]>=0.0f))
		|| ((nf2[0]<=0.0f) && (nf2[1]<=0.0f) && (nf2[2]<=0.0f)))
	{
		return 1;
	}
	else
	{
		return 0;
	}
	
}


int LineCrossLine2d(const Vector2f& a1, const Vector2f& a2, const Vector2f& b1, const Vector2f& b2, Vector2f& r)
{
	Vector2f v = (a2 - a1);

	Matrix2f m;

	m.m[0] = v(0);
	m.m[1] = b1(0) - b2(0);
	m.m[2] = v(1);
	m.m[3] = b1(1) - b2(1);

	float detm = m.m[0] * m.m[3] - m.m[1] * m.m[2];

	if (fabs(detm) <= 0.005f)
	{
		return -1;
	}

	Matrix2f ad_m;
	ad_m.m[0] = m.m[3];
	ad_m.m[1] = -m.m[2];

	ad_m.m[2] = -m.m[1];
	ad_m.m[3] = m.m[0];

	Matrix2f rev_m;

	rev_m.m[0] = ad_m.m[0]/detm;
	rev_m.m[1] = ad_m.m[2]/detm;

	rev_m.m[2] = ad_m.m[1]/detm;
	rev_m.m[3] = ad_m.m[3]/detm;

	Vector2f t;
	Vector2f vx = (b1 - a1);
	t(0) =  vx(0) * rev_m.m[0] + vx(1) * rev_m.m[1];
	t(1) =  vx(0) * rev_m.m[2] + vx(1) * rev_m.m[3]; 

	//if (t(0) < 0 || t(0) > 1 || t(1) < 0 || t(1) > 1)
	//if (t(0) < 0.01f|| t(0) > 0.99f || t(1) < 0.01f || t(1) > 0.99f)
	if (t(0) < -0.01f || t(0) > 1.01f || t(1) < -0.01f || t(1) > 1.01f)
	{
		return 0;
	}

	r = a1 + v * t(0);

	return 1;

}
*/

float roundf(float r)
{
	return (r > 0.0f) ? floorf(r + 0.5f) : ceilf(r - 0.5f); 
} 

size_t GetGreaterPower2(size_t x)
{
	if (x > 65536)
	{
		throw ErrorToLog("GetGreaterPower2: x("+tostr(x)+") is bigger than 65536");
	}

	size_t result = 1;

	while (x > result)
	{
		result *= 2;
	}

	return result;
}


Vector2f StringToVec2(std::string str)
{
	boost::erase_all(str, " ");

	std::vector<std::string> splitVec;

	boost::split( splitVec, str, boost::is_any_of(",;"), boost::token_compress_on );

	if (splitVec.size() < 2)
	{
		throw ErrorToLog("StringToVec2 - vector is too short: "+str);
	}

	Vector2f result;

	result(0) = boost::lexical_cast<float>(splitVec[0]);
	result(1) = boost::lexical_cast<float>(splitVec[1]);

	return result;
}

Vector3f StringToVec3(std::string str)
{
	boost::erase_all(str, " ");

	std::vector<std::string> splitVec;

	boost::split( splitVec, str, boost::is_any_of(",;"), boost::token_compress_on );

	if (splitVec.size() < 3)
	{
		throw ErrorToLog("StringToVec3 - vector is too short: "+str);
	}

	Vector3f result;

	result(0) = boost::lexical_cast<float>(splitVec[0]);
	result(1) = boost::lexical_cast<float>(splitVec[1]);
	result(2) = boost::lexical_cast<float>(splitVec[2]);

	return result;
}

Vector4f StringToVec4(std::string str)
{
	boost::erase_all(str, " ");

	std::vector<std::string> splitVec;

	boost::split( splitVec, str, boost::is_any_of(",;"), boost::token_compress_on );

	if (splitVec.size() < 4)
	{
		throw ErrorToLog("StringToVec4 - vector is too short: "+str);
	}

	Vector4f result;

	result(0) = boost::lexical_cast<float>(splitVec[0]);
	result(1) = boost::lexical_cast<float>(splitVec[1]);
	result(2) = boost::lexical_cast<float>(splitVec[2]);
	result(3) = boost::lexical_cast<float>(splitVec[3]);

	return result;
}


// ================================================
// ========== wide strings and UTF8 strings =======
// ================================================


void utf8toWStr(std::wstring& dest, const std::string& src){
	dest.clear();
	wchar_t w = 0;
	int bytes = 0;
	wchar_t err = 0xFFFD;
	for (size_t i = 0; i < src.size(); i++){
		unsigned char c = (unsigned char)src[i];
		if (c <= 0x7f){//first byte
			if (bytes){
				dest.push_back(err);
				bytes = 0;
			}
			dest.push_back((wchar_t)c);
		}
		else if (c <= 0xbf){//second/third/etc byte
			if (bytes){
				w = ((w << 6)|(c & 0x3f));
				bytes--;
				if (bytes == 0)
					dest.push_back(w);
			}
			else
				dest.push_back(err);
		}
		else if (c <= 0xdf){//2byte sequence start
			bytes = 1;
			w = c & 0x1f;
		}
		else if (c <= 0xef){//3byte sequence start
			bytes = 2;
			w = c & 0x0f;
		}
		else if (c <= 0xf7){//3byte sequence start
			bytes = 3;
			w = c & 0x07;
		}
		else{
			dest.push_back(err);
			bytes = 0;
		}
	}
	if (bytes)
		dest.push_back(err);
}

void wstrToUtf8(std::string& dest, const std::wstring& src){
	dest.clear();
	for (size_t i = 0; i < src.size(); i++){
		wchar_t w = src[i];
		if (w <= 0x7f)
			dest.push_back((char)w);
		else if (w <= 0x7ff){
			dest.push_back(0xc0 | ((w >> 6)& 0x1f));
			dest.push_back(0x80| (w & 0x3f));
		}
		else if (w <= 0xffff){
			dest.push_back(0xe0 | ((w >> 12)& 0x0f));
			dest.push_back(0x80| ((w >> 6) & 0x3f));
			dest.push_back(0x80| (w & 0x3f));
		}
		else if (w <= 0x10ffff){
			dest.push_back(0xf0 | ((w >> 18)& 0x07));
			dest.push_back(0x80| ((w >> 12) & 0x3f));
			dest.push_back(0x80| ((w >> 6) & 0x3f));
			dest.push_back(0x80| (w & 0x3f));
		}
		else
			dest.push_back('?');
	}
}

std::string wstrToUtf8(const std::wstring& str){
	std::string result;
	wstrToUtf8(result, str);
	return result;
}

std::wstring utf8toWStr(const std::string& str){
	std::wstring result;
	utf8toWStr(result, str);
	return result;
}

std::ostream& operator<<(std::ostream& f, const std::wstring& s){
	std::string s1;
	wstrToUtf8(s1, s);
	f << s1;
	return f;
}

std::istream& operator>>(std::istream& f, std::wstring& s){
	std::string s1;
	f >> s1;
	utf8toWStr(s, s1);
	return f;
}


} //namespace SE