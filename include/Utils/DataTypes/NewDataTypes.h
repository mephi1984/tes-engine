#ifndef NEW_DATA_TYPES_H_INCLUDED
#define NEW_DATA_TYPES_H_INCLUDED

#include <math.h>

#include <Eigen/Geometry>
#include <Eigen/Dense>

namespace SE
{

	using namespace Eigen;

//Find this in DataTypes.h
bool IsFloatEqual(float a, float b);


/*
template<typename TYPENAME1>
class tvec4;


template<typename TYPENAME1>
class tmat3;

template<typename TYPENAME>
class tvec2
{
public:
	TYPENAME v[2];
	tvec2() { v[0] = 0; v[1] = 0; }

	template<typename TYPENAME1, typename TYPENAME2>
	tvec2(TYPENAME1 x, TYPENAME2 y) { v[0] = static_cast<TYPENAME>(x); v[1] = static_cast<TYPENAME>(y); }

	tvec2<TYPENAME>& operator=(const tvec2<TYPENAME>& vc);
	bool operator==(const tvec2<TYPENAME>& vc) const;
	bool operator!=(const tvec2<TYPENAME>& vc) const;
	tvec2<TYPENAME>& operator+=(const tvec2<TYPENAME>& vc);
	tvec2<TYPENAME> operator+(const tvec2<TYPENAME>& a) const;
	tvec2<TYPENAME>& operator-=(const tvec2<TYPENAME>& vc);
	tvec2<TYPENAME> operator-(const tvec2<TYPENAME>& a) const;
	tvec2<TYPENAME> operator-();
	tvec2<TYPENAME>& operator*=(TYPENAME c);
};

typedef tvec2<float> Vector2f;
typedef tvec2<int> Vector2i;


Vector2f Normalize(const Vector2f& a);


float Length(const Vector2f& a);


float DotProduct(const Vector2f& a,const Vector2f& b);


template<typename TYPENAME1>
class tvec3
{
public:
	TYPENAME1 v[3];

	tvec3<TYPENAME1>();
	tvec3(TYPENAME1 x, TYPENAME1 y, TYPENAME1 z);
	tvec3(const tvec2<TYPENAME1>& vc, TYPENAME1 z);
	tvec3(const tvec3<TYPENAME1>& vc);
	tvec3(const tvec4<TYPENAME1>& vc);

	bool operator==(const tvec3<TYPENAME1>& vc) const;
	bool operator!=(const tvec3<TYPENAME1>& vc) const;
	tvec3<TYPENAME1>& operator=(const tvec3<TYPENAME1>& vc);
	tvec3<TYPENAME1>& operator+=(const tvec3<TYPENAME1>& vc);
	tvec3<TYPENAME1>& operator-=(const tvec3<TYPENAME1>& vc);
	tvec3<TYPENAME1> operator+(const tvec3<TYPENAME1>& a) const;
	tvec3<TYPENAME1> operator-(const tvec3<TYPENAME1>& a) const;
	tvec3<TYPENAME1> operator-();
	tvec3<TYPENAME1>& operator*=(TYPENAME1 c);
	tvec3<TYPENAME1> operator*(const tmat3<TYPENAME1>& mt) const; //MultRowMatrix()
};

typedef tvec3<float> Vector3f;


float DotProduct(const Vector3f& a,const Vector3f& b);
Vector3f CrossProduct(const Vector3f& a,const Vector3f& b);
Vector3f Normalize(const Vector3f& a);
float Length(const Vector3f& a);

template<typename TYPENAME1>
class tvec4
{
public:
	TYPENAME1 v[4];

	tvec4();
	tvec4(TYPENAME1 x, TYPENAME1 y, TYPENAME1 z, TYPENAME1 w);
	tvec4(const tvec3<TYPENAME1>& vc);
	tvec4(const tvec4<TYPENAME1>& vc);
	tvec4(const tmat3<TYPENAME1>& m);

	bool operator==(const tvec4<TYPENAME1>& vc) const;
	bool operator!=(const tvec4<TYPENAME1>& vc) const;
	tvec4<TYPENAME1>& operator=(const tvec4<TYPENAME1>& vc);
	tvec4<TYPENAME1>& operator+=(const tvec4<TYPENAME1>& vc);
	tvec4<TYPENAME1>& operator-=(const tvec4<TYPENAME1>& vc);
	const tvec4<TYPENAME1> operator+(const tvec4<TYPENAME1>& a) const;
	const tvec4<TYPENAME1> operator-(const tvec4<TYPENAME1>& a) const;
	//Vector4f& operator-(); not so obvious for quaternion :(
	tvec4<TYPENAME1>& operator*=(TYPENAME1 c);

	tvec4<TYPENAME1>& operator*=(const tvec4<TYPENAME1>& q);
	tvec4<TYPENAME1> operator*(const tvec4<TYPENAME1>& q) const;
};


typedef tvec4<float> Vector4f;


Vector4f InverseQuat(const Vector4f& q);
Vector4f NormalizeQuat(const Vector4f& q);


template<typename TYPENAME1>
class tmat2
{
public:
	TYPENAME1 m[4];

	tmat2();
	tmat2(TYPENAME1 alpha);
	tmat2(const tmat2<TYPENAME1>& mt);

	bool operator==(const tmat2<TYPENAME1>& mt) const;
	bool operator!=(const tmat2<TYPENAME1>& mt) const;
	tmat2<TYPENAME1>& operator=(const tmat2<TYPENAME1>& mt);
	tmat2<TYPENAME1>& operator*=(const tmat2<TYPENAME1>& mt);
	tmat2<TYPENAME1> operator*(const tmat2<TYPENAME1>& mt);
	tvec2<TYPENAME1> operator*(const tvec2<TYPENAME1>& vc) const; //MultMatrixCol()

};

typedef tmat2<float> Matrix2f;



template<typename TYPENAME1>
class tmat3
{
public:
	TYPENAME1 m[9];

	tmat3();
	tmat3(const tvec3<TYPENAME1>& col1, const tvec3<TYPENAME1>& col2, const tvec3<TYPENAME1>& col3);
	tmat3(const tmat3<TYPENAME1>& mt);
	tmat3(const tvec4<TYPENAME1>& q);

	bool operator==(const tmat3<TYPENAME1>& mt) const;
	bool operator!=(const tmat3<TYPENAME1>& mt) const;
	tmat3<TYPENAME1>& operator=(const tmat3<TYPENAME1>& mt);
	tmat3<TYPENAME1>& operator*=(const tmat3<TYPENAME1>& mt);
	tmat3<TYPENAME1> operator*(const tmat3<TYPENAME1>& mt);
	tvec3<TYPENAME1> operator*(const tvec3<TYPENAME1>& vc) const; //MultMatrixCol()

};

typedef tmat3<float> Matrix3f;




Matrix3f TransposeMatrix(const Matrix3f& m);
Matrix3f InverseMatrix(const Matrix3f& m);

Matrix3f CreateZRotationMatrix(float angle);

//Outside:
inline Vector2f operator*(const Vector2f& a, float c)
{
	 Vector2f r;
	 r(0) = a(0)*c;
	 r(1) = a(1)*c;
	 return r;
}

inline Vector2f operator*(float c, const Vector2f& a)
{
	 return  a*c;
}

inline Vector3f operator*(const Vector3f& a, float c)
{
	 Vector3f r;
	 r(0) = a(0)*c;
	 r(1) = a(1)*c;
	 r(2) = a(2)*c;
	 return r;
}

inline Vector3f operator*(float c, const Vector3f& a)
{
	 return  a*c;
}

inline Vector4f operator*(const Vector4f& a, float c)
{
	 Vector4f r;
	 r(0) = a(0)*c;
	 r(1) = a(1)*c;
	 r(2) = a(2)*c;
	 r.v[3] = a.v[3]*c;
	 return r;
}

inline Vector4f operator*(float c,const Vector4f& a)
{
	 return  a*c;
}





//============= Vector2f ================

template<typename TYPENAME>
inline tvec2<TYPENAME>& tvec2<TYPENAME>::operator=(const tvec2<TYPENAME>& vc)
{
	//Self-assignment is ok here
	v[0] = vc(0);
	v[1] = vc(1);

	return *this;
}

template<typename TYPENAME>
inline bool tvec2<TYPENAME>::operator==(const tvec2<TYPENAME>& vc) const
{
	if (IsFloatEqual(static_cast<float>(v[0]), static_cast<float>(vc(0))) &&
		IsFloatEqual(static_cast<float>(v[1]), static_cast<float>(vc(1))))
	{
		return true;
	}

	return false;
}

template<typename TYPENAME>
inline bool tvec2<TYPENAME>::operator!=(const tvec2<TYPENAME>& vc) const
{
	return !(*this == vc);
}

template<typename TYPENAME>
inline tvec2<TYPENAME>& tvec2<TYPENAME>::operator+=(const tvec2<TYPENAME>& vc)
{
	v[0] += vc(0);
	v[1] += vc(1);

	return *this;
}

template<typename TYPENAME>
inline tvec2<TYPENAME> tvec2<TYPENAME>::operator+(const tvec2<TYPENAME>& a) const
{
	tvec2<TYPENAME> r = *this;
	r += a;
	return r;
}

template<typename TYPENAME>
inline tvec2<TYPENAME>& tvec2<TYPENAME>::operator-=(const tvec2<TYPENAME>& vc)
{
	v[0] -= vc(0);
	v[1] -= vc(1);

	return *this;
}

template<typename TYPENAME>
inline tvec2<TYPENAME> tvec2<TYPENAME>::operator-(const tvec2<TYPENAME>& a) const
{
	tvec2<TYPENAME> r = *this;
	r -= a;
	return r;
}

template<typename TYPENAME>
inline tvec2<TYPENAME> tvec2<TYPENAME>::operator-()
{
	tvec2<TYPENAME> r = *this;

	r(0) = -v[0];
	r(1) = -v[1];

	return r;
}

template<typename TYPENAME>
inline tvec2<TYPENAME>& tvec2<TYPENAME>::operator*=(TYPENAME c)
{
	v[0] *= c;
	v[1] *= c;

	return *this;
}


inline Vector2f Normalize(const Vector2f& a)
{
	Vector2f r;
	float d = a(0)*a(0)+a(1)*a(1);
	if (IsFloatEqual(d, 0.0f))
	{
		r(0) = 0.0f;
		r(1) = 0.0f;
		
		return r;
	}
	else
	{
		d = sqrtf(d);

		r(0) = a(0)/d;
		r(1) = a(1)/d;

		return r;
	}
}

inline float Length(const Vector2f& a)
{
	return sqrtf(a(0)*a(0) + a(1)*a(1));
}

inline float DotProduct(const Vector2f& a,const Vector2f& b)
{
	return a(0)*b(0) + a(1)*b(1);
}

//============= Vector3f ================


template<typename TYPENAME1>
inline tvec3<TYPENAME1>::tvec3()
{
	v[0] = 0;
	v[1] = 0;
	v[2] = 0;
}

template<typename TYPENAME1>
inline tvec3<TYPENAME1>::tvec3(TYPENAME1 x, TYPENAME1 y, TYPENAME1 z)
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

template<typename TYPENAME1>
inline tvec3<TYPENAME1>::tvec3(const tvec2<TYPENAME1>& vc, TYPENAME1 z)
{
    v[0] = vc(0);
	v[1] = vc(1);
	v[2] = z;
}

template<typename TYPENAME1>
inline tvec3<TYPENAME1>::tvec3(const tvec3<TYPENAME1>& vc)
{
	v[0] = vc(0);
	v[1] = vc(1);
	v[2] = vc(2);
}

template<typename TYPENAME1>
inline tvec3<TYPENAME1>::tvec3(const tvec4<TYPENAME1>& vc)
{
	v[0] = vc(0);
	v[1] = vc(1);
	v[2] = vc(2);
}

template<typename TYPENAME1>
inline bool tvec3<TYPENAME1>::operator==(const tvec3<TYPENAME1>& vc) const
{
	if (IsFloatEqual(static_cast<float>(v[0]), static_cast<float>(vc(0))) &&
		IsFloatEqual(static_cast<float>(v[1]), static_cast<float>(vc(1))) &&
		IsFloatEqual(static_cast<float>(v[2]), static_cast<float>(vc(2))))
	{
		return true;
	}

	return false;
}

template<typename TYPENAME>
inline bool tvec3<TYPENAME>::operator!=(const tvec3<TYPENAME>& vc) const
{
	return !(*this == vc);
}


template<typename TYPENAME1>
inline tvec3<TYPENAME1>& tvec3<TYPENAME1>::operator=(const tvec3<TYPENAME1>& vc)
{
	//Self-assignment is ok here
	v[0] = vc(0);
	v[1] = vc(1);
	v[2] = vc(2);

	return *this;
}

template<typename TYPENAME1>
inline tvec3<TYPENAME1>& tvec3<TYPENAME1>::operator+=(const tvec3<TYPENAME1>& vc)
{
	v[0] += vc(0);
	v[1] += vc(1);
	v[2] += vc(2);

	return *this;
}

template<typename TYPENAME1>
inline tvec3<TYPENAME1>& tvec3<TYPENAME1>::operator-=(const tvec3<TYPENAME1>& vc)
{
	v[0] -= vc(0);
	v[1] -= vc(1);
	v[2] -= vc(2);

	return *this;
}

template<typename TYPENAME1>
inline tvec3<TYPENAME1> tvec3<TYPENAME1>::operator+(const tvec3<TYPENAME1>& a) const
{
	tvec3<TYPENAME1> r = *this;
	r += a;
	return r;
}

template<typename TYPENAME1>
inline tvec3<TYPENAME1> tvec3<TYPENAME1>::operator-(const tvec3<TYPENAME1>& a) const
{
	tvec3<TYPENAME1> r = *this;
	r -= a;
	return r;
}

template<typename TYPENAME1>
inline tvec3<TYPENAME1> tvec3<TYPENAME1>::operator-()
{
	tvec3<TYPENAME1> r = *this;

	r(0) = -v[0];
	r(1) = -v[1];
	r(2) = -v[2];

	return r;
}

template<typename TYPENAME1>
inline tvec3<TYPENAME1>& tvec3<TYPENAME1>::operator*=(TYPENAME1 c)
{
	v[0] *= c;
	v[1] *= c;
	v[2] *= c;

	return *this;
}

template<typename TYPENAME1>
inline tvec3<TYPENAME1> tvec3<TYPENAME1>::operator*(const tmat3<TYPENAME1>& mt)  const
{
	tvec3<TYPENAME1> r = *this;

	r(0) = v[0]*mt.m[0] + v[1]*mt.m[1] + v[2]*mt.m[2];
	r(1) = v[0]*mt.m[3] + v[1]*mt.m[4] + v[2]*mt.m[5];
	r(2) = v[0]*mt.m[6] + v[1]*mt.m[7] + v[2]*mt.m[8];

	return r;
}

inline float DotProduct(const Vector3f& a,const Vector3f& b)
{
	return a(0)*b(0)+a(1)*b(1)+a(2)*b(2);
}

inline Vector3f CrossProduct(const Vector3f& a,const Vector3f& b)
{
	Vector3f r;
	r(0) = a(1)*b(2) - a(2)*b(1);
	r(1) = a(2)*b(0) - a(0)*b(2);
	r(2) = a(0)*b(1) - a(1)*b(0);
	return r;
}

inline Vector3f Normalize(const Vector3f& a)
{
	Vector3f r;
	float d = a(0)*a(0)+a(1)*a(1)+a(2)*a(2);
	if (IsFloatEqual(d,0.0f))
	{
		r(0) = 0.0f;
		r(1) = 0.0f;
		r(2) = 0.0f;
		return r;
	}
	else
	{
		d = sqrtf(d);

		r(0) = a(0)/d;
		r(1) = a(1)/d;
		r(2) = a(2)/d;

		return r;
	}
}

inline float Length(const Vector3f& a)
{
	return sqrtf(a(0)*a(0) + a(1)*a(1) + a(2)*a(2));
}


//============= Vector4f ================


template<typename TYPENAME1>
inline tvec4<TYPENAME1>::tvec4()
{
	v[0] = 0;
	v[1] = 0;
	v[2] = 0;
	v[3] = 1;
}

template<typename TYPENAME1>
inline tvec4<TYPENAME1>::tvec4(TYPENAME1 x, TYPENAME1 y, TYPENAME1 z, TYPENAME1 w)
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
	v[3] = w;
}

template<typename TYPENAME1>
inline tvec4<TYPENAME1>::tvec4(const tvec4<TYPENAME1>& vc)
{
	v[0] = vc(0);
	v[1] = vc(1);
	v[2] = vc(2);
	v[3] = vc.v[3];
}

template<typename TYPENAME1>
inline tvec4<TYPENAME1>::tvec4(const tvec3<TYPENAME1>& vc)
{
	v[0] = vc(0);
	v[1] = vc(1);
	v[2] = vc(2);
	v[3] = 0.0f;
}

template<typename TYPENAME1>
inline tvec4<TYPENAME1>::tvec4(const tmat3<TYPENAME1>& m)
{

	TYPENAME1 f;

	if (m.m[0] >= m.m[4] && m.m[0] >= m.m[8])
	{
		f = sqrtf(1.0f + m.m[0] - m.m[4] - m.m[8]);
		if (f != 0)
		{
			v[3] = (m.m[5] - m.m[7]) / (f + f);
			v[0] = f/2;
			v[1] = (m.m[3] + m.m[1]) / (f + f);
			v[2] = (m.m[6] + m.m[2]) / (f + f);
		}
		else
		{
			v[3] = 1;
			v[2] = 0;
			v[1] = 0;
			v[0] = 0;
		}
	}

	if (m.m[4] >= m.m[0] && m.m[4] >= m.m[8])
	{
		f = sqrtf(1.0f + m.m[4] - m.m[0] - m.m[8]);
		if (f != 0)
		{
			v[3] = (m.m[6] - m.m[2]) / (f + f);
			v[1] = f/2;
			v[0] = (m.m[1] + m.m[3]) / (f + f);
			v[2] = (m.m[7] + m.m[5]) / (f + f);
		}
		else
		{
			v[3] = 1;
			v[2] = 0;
			v[1] = 0;
			v[0] = 0;
		}
	}

	if (m.m[8] >= m.m[4] && m.m[8] >= m.m[0])
	{
		f = sqrtf(1.0f + m.m[8] - m.m[2]);
		if (f != 0)
		{
			v[3] = (m.m[1] - m.m[3]) / (f + f);
			v[2] = f/2;
			v[1] = (m.m[5] + m.m[7]) / (f + f);
			v[0] = (m.m[6] + m.m[2]) / (f + f);
		}
		else
		{
			v[3] = 1;
			v[2] = 0;
			v[1] = 0;
			v[0] = 0;
		}
	}

}

template<typename TYPENAME1>
inline bool tvec4<TYPENAME1>::operator==(const tvec4<TYPENAME1>& vc) const
{
	if (IsFloatEqual(static_cast<float>(v[0]), static_cast<float>(vc(0))) &&
		IsFloatEqual(static_cast<float>(v[1]), static_cast<float>(vc(1))) &&
		IsFloatEqual(static_cast<float>(v[2]), static_cast<float>(vc(2))) &&
		IsFloatEqual(static_cast<float>(v[3]), static_cast<float>(vc.v[3])))
	{
		return true;
	}

	return false;
}

template<typename TYPENAME1>
inline bool tvec4<TYPENAME1>::operator!=(const tvec4<TYPENAME1>& vc) const
{
	return !(*this == vc);
}

template<typename TYPENAME1>
inline tvec4<TYPENAME1>& tvec4<TYPENAME1>::operator=(const tvec4<TYPENAME1>& vc)
{
	//Self-assignment is not actual here...
	v[0] = vc(0);
	v[1] = vc(1);
	v[2] = vc(2);
	v[3] = vc.v[3];

	return *this;
}

template<typename TYPENAME1>
inline tvec4<TYPENAME1>& tvec4<TYPENAME1>::operator+=(const tvec4<TYPENAME1>& vc)
{
	v[0] += vc(0);
	v[1] += vc(1);
	v[2] += vc(2);
	v[3] += vc.v[3];

	return *this;
}

template<typename TYPENAME1>
inline tvec4<TYPENAME1>& tvec4<TYPENAME1>::operator-=(const tvec4<TYPENAME1>& vc)
{
	v[0] -= vc(0);
	v[1] -= vc(1);
	v[2] -= vc(2);
	v[3] -= vc.v[3];
	return *this;
}

template<typename TYPENAME1>
inline const tvec4<TYPENAME1> tvec4<TYPENAME1>::operator+(const tvec4<TYPENAME1>& a) const
{
	tvec4<TYPENAME1> r = *this;
	r += a;
	return r;
}

template<typename TYPENAME1>
inline const tvec4<TYPENAME1> tvec4<TYPENAME1>::operator-(const tvec4<TYPENAME1>& a)  const
{
	tvec4<TYPENAME1> r = *this;
	r -= a;
	return r;
}


template<typename TYPENAME1>
inline tvec4<TYPENAME1>& tvec4<TYPENAME1>::operator*=(TYPENAME1 c)
{
	v[0] *= c;
	v[1] *= c;
	v[2] *= c;
	v[3] *= c;

	return *this;
}

template<typename TYPENAME1>
inline tvec4<TYPENAME1>& tvec4<TYPENAME1>::operator*=(const tvec4<TYPENAME1>& q)
{
	TYPENAME1 A = (v[3] + v[0]) * (q.v[3] + q(0));
	TYPENAME1 B = (v[2] - v[1]) * (q(1) - q(2));
	TYPENAME1 C = (v[0] - v[3]) * (q(1) + q(2));
	TYPENAME1 D = (v[1] + v[2]) * (q(0) - q.v[3]);
	TYPENAME1 E = (v[0] + v[2]) * (q(0) + q(1));
	TYPENAME1 F = (v[0] - v[2]) * (q(0) - q(1));
	TYPENAME1 G = (v[3] + v[1]) * (q.v[3] - q(2));
	TYPENAME1 H = (v[3] - v[1]) * (q.v[3] + q(2));
	
	v[0] = A - ( E + F + G + H) * 0.5f;
	v[1] =-C + ( E - F + G - H) * 0.5f;
	v[2] =-D + ( E - F - G + H) * 0.5f;
	v[3] = B + (-E - F + G + H) * 0.5f;

	return *this;
}

template<typename TYPENAME1>
inline tvec4<TYPENAME1> tvec4<TYPENAME1>::operator*(const tvec4<TYPENAME1>& q)  const
{
	tvec4<TYPENAME1> r = *this;
	r *= q;
	return r;
}


inline Vector4f InverseQuat(const Vector4f& q)
{
	float n;
	Vector4f r;
	n = (q(0)*q(0)+q(1)*q(1)+q(2)*q(2)+q.v[3]*q.v[3]);

	if (n!=0.0f)
	{
		r(0) = -q(0) / n;
		r(1) = -q(1) / n;
		r(2) = -q(2) / n;
		r.v[3] = q.v[3] / n;
	}
	else
	{
		r(0) = 0.0f;
		r(1) = 0.0f;
		r(2) = 0.0f;
		r.v[3] = 1.0f;
	};
	
	return r;

}

inline Vector4f NormalizeQuat(const Vector4f& q)
{
	float n;

	Vector4f r;

	n = (q(0)*q(0)+q(1)*q(1)+q(2)*q(2)+q.v[3]*q.v[3]);

	if (n!=0.0f)
	{
		r(0) = q(0) / n;
		r(1) = q(1) / n;
		r(2) = q(2) / n;
		r.v[3] = q.v[3] / n;
	}
	
	return r;

}
//========== Matrix2f ===========


template<typename TYPENAME1>
inline tmat2<TYPENAME1>::tmat2()
{
	m[0] = 1;
	m[1] = 0;


	m[2] = 0;
	m[3] = 1;

}

template<typename TYPENAME1>
inline tmat2<TYPENAME1>::tmat2(TYPENAME1 alpha)
{
	m[0] = cosf(alpha);
	m[1] = sinf(alpha);

	m[2] = -sinf(alpha);
	m[3] = cosf(alpha);

}

template<typename TYPENAME1>
inline tmat2<TYPENAME1>::tmat2(const tmat2<TYPENAME1>& mt)
{
	m[0] = mt.m[0];
	m[1] = mt.m[1];

	m[2] = mt.m[2];
	m[3] = mt.m[3];

}

template<typename TYPENAME1>
inline bool tmat2<TYPENAME1>::operator==(const tmat2<TYPENAME1>& mt) const
{
	if (IsFloatEqual(static_cast<float>(m[0]), static_cast<float>(mt.m[0])) &&
		IsFloatEqual(static_cast<float>(m[1]), static_cast<float>(mt.m[1])) &&
		IsFloatEqual(static_cast<float>(m[2]), static_cast<float>(mt.m[2])) &&
		IsFloatEqual(static_cast<float>(m[3]), static_cast<float>(mt.m[3])))
	{
		return true;
	}

	return false;
}

template<typename TYPENAME1>
inline bool tmat2<TYPENAME1>::operator!=(const tmat2<TYPENAME1>& mt) const
{
	return !(*this == mt);
}

template<typename TYPENAME1>
inline tmat2<TYPENAME1>& tmat2<TYPENAME1>::operator=(const tmat2<TYPENAME1>& mt)
{
	//Self-assignment is not actual here....
	m[0] = mt.m[0];
	m[1] = mt.m[1];
	m[2] = mt.m[2];
	m[3] = mt.m[3];
	return *this;
}

template<typename TYPENAME1>
inline tmat2<TYPENAME1>& tmat2<TYPENAME1>::operator*=(const tmat2<TYPENAME1>& mt)
{
	tmat2<TYPENAME1> a(*this);
	
	m[0] = a.m[0]*mt.m[0] + a.m[2]*mt.m[1];
	m[1] = a.m[1]*mt.m[0] + a.m[3]*mt.m[1];
	m[2] = a.m[0]*mt.m[2] + a.m[2]*mt.m[3];
	m[3] = a.m[1]*mt.m[2] + a.m[3]*mt.m[3];

	return *this;
}

template<typename TYPENAME1>
inline tmat2<TYPENAME1> tmat2<TYPENAME1>::operator*(const tmat2<TYPENAME1>& mt)
{
	tmat2<TYPENAME1> r = *this;
	r *= mt;
	return r;
}

template<typename TYPENAME1>
inline tvec2<TYPENAME1> tmat2<TYPENAME1>::operator*(const tvec2<TYPENAME1>& vc) const
{
	tvec2<TYPENAME1> r;
	r(0) = m[0]*vc(0) + m[2]*vc(1);
	r(1) = m[1]*vc(0) + m[3]*vc(1);
	return r;
}

//========== Matrix3f ===========


template<typename TYPENAME1>
inline tmat3<TYPENAME1>::tmat3()
{
	m[0] = 1;
	m[1] = 0;
	m[2] = 0;

	m[3] = 0;
	m[4] = 1;
	m[5] = 0;

	m[6] = 0;
	m[7] = 0;
	m[8] = 1;
}

template<typename TYPENAME1>
inline tmat3<TYPENAME1>::tmat3(const tvec3<TYPENAME1>& col1, const tvec3<TYPENAME1>& col2, const tvec3<TYPENAME1>& col3)
{
	m[0] = col1(0);
	m[1] = col1(1);
	m[2] = col1(2);

	m[3] = col2(0);
	m[4] = col2(1);
	m[5] = col2(2);

	m[6] = col3(0);
	m[7] = col3(1);
	m[8] = col3(2);
}

template<typename TYPENAME1>
inline tmat3<TYPENAME1>::tmat3(const tmat3<TYPENAME1>& mt)
{
	m[0] = mt.m[0];
	m[1] = mt.m[1];
	m[2] = mt.m[2];

	m[3] = mt.m[3];
	m[4] = mt.m[4];
	m[5] = mt.m[5];

	m[6] = mt.m[6];
	m[7] = mt.m[7];
	m[8] = mt.m[8];
}

template<typename TYPENAME1>
inline tmat3<TYPENAME1>::tmat3(const tvec4<TYPENAME1>& q)
{
	TYPENAME1 wx, wy, wz, xx, yy, yz, xy, xz, zz,s,x2,y2,z2;

	s=2.0f/(q(0)*q(0)+q(1)*q(1)+q(2)*q(2)+q.v[3]*q.v[3]);
  

	x2=q(0)*s;
	y2=q(1)*s;
	z2=q(2)*s;

	wx=q.v[3]*x2; wy=q.v[3]*y2; wz=q.v[3]*z2;
	xx=q(0)*x2; xy=q(1)*x2; xz=q(2)*x2;
	yy=q(1)*y2; yz=q(2)*y2;
	zz=q(2)*z2;

	m[0] = 1.0f - (yy + zz);
	m[1] = xy + wz;
	m[2] = xz - wy;

	m[3] = xy - wz;
	m[4] = 1.0f - (xx + zz);
	m[5] = yz  + wx;

	m[6] = xz + wy;
	m[7] = yz - wx;
	m[8] = 1.0f - (xx + yy);
  
}

template<typename TYPENAME1>
inline bool tmat3<TYPENAME1>::operator==(const tmat3<TYPENAME1>& mt) const
{
	if (IsFloatEqual(m[0], mt.m[0]) &&
		IsFloatEqual(m[1], mt.m[1]) &&
		IsFloatEqual(m[2], mt.m[2]) &&
		IsFloatEqual(m[3], mt.m[3]) &&
		IsFloatEqual(m[4], mt.m[4]) &&
		IsFloatEqual(m[5], mt.m[5]) &&
		IsFloatEqual(m[6], mt.m[6]) &&
		IsFloatEqual(m[7], mt.m[7]) &&
		IsFloatEqual(m[8], mt.m[8]))
	{
		return true;
	}

	return false;
}

template<typename TYPENAME1>
inline bool tmat3<TYPENAME1>::operator!=(const tmat3<TYPENAME1>& mt) const
{
	return !(*this == mt);
}

template<typename TYPENAME1>
inline tmat3<TYPENAME1>& tmat3<TYPENAME1>::operator=(const tmat3<TYPENAME1>& mt)
{
	//Self-assignment is not actual here....
	m[0] = mt.m[0];
	m[1] = mt.m[1];
	m[2] = mt.m[2];

	m[3] = mt.m[3];
	m[4] = mt.m[4];
	m[5] = mt.m[5];

	m[6] = mt.m[6];
	m[7] = mt.m[7];
	m[8] = mt.m[8];

	return *this;
}

template<typename TYPENAME1>
inline tmat3<TYPENAME1>& tmat3<TYPENAME1>::operator*=(const tmat3<TYPENAME1>& mt)
{
	tmat3<TYPENAME1> a(*this);

	m[0] = a.m[0]*mt.m[0] + a.m[1]*mt.m[3] + a.m[2]*mt.m[6];
	m[1] = a.m[0]*mt.m[1] + a.m[1]*mt.m[4] + a.m[2]*mt.m[7];
	m[2] = a.m[0]*mt.m[2] + a.m[1]*mt.m[5] + a.m[2]*mt.m[8];

	m[3] = a.m[3]*mt.m[0] + a.m[4]*mt.m[3] + a.m[5]*mt.m[6];
	m[4] = a.m[3]*mt.m[1] + a.m[4]*mt.m[4] + a.m[5]*mt.m[7];
	m[5] = a.m[3]*mt.m[2] + a.m[4]*mt.m[5] + a.m[5]*mt.m[8];
  
	m[6] = a.m[6]*mt.m[0] + a.m[7]*mt.m[3] + a.m[8]*mt.m[6];
	m[7] = a.m[6]*mt.m[1] + a.m[7]*mt.m[4] + a.m[8]*mt.m[7];
	m[8] = a.m[6]*mt.m[2] + a.m[7]*mt.m[5] + a.m[8]*mt.m[8];

	return *this;
}

template<typename TYPENAME1>
inline tmat3<TYPENAME1> tmat3<TYPENAME1>::operator*(const tmat3<TYPENAME1>& mt)
{
	tmat3<TYPENAME1> r = *this;
	r *= mt;
	return r;
}

template<typename TYPENAME1>
inline tvec3<TYPENAME1> tmat3<TYPENAME1>::operator*(const tvec3<TYPENAME1>& vc) const
{
	tvec3<TYPENAME1> r;
	r(0) = m[0]*vc(0) + m[3]*vc(1) + m[6]*vc(2);
	r(1) = m[1]*vc(0) + m[4]*vc(1) + m[7]*vc(2);
	r(2) = m[2]*vc(0) + m[5]*vc(1) + m[8]*vc(2);
	return r;
}

Matrix3f TransposeMatrix(const Matrix3f& m);

Matrix3f InverseMatrix(const Matrix3f& m);


*/

} //namespace SE

#endif