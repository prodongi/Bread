
#ifndef _BreadVector3_h_
#define _BreadVector3_h_

#include "BreadMath.h"
#include <stdio.h>
#include <tchar.h>

namespace Bread
{
	struct sVector3;

	#define V3(x, y, z)	sVector3(x, y, z)

	/// @brief out = v1 + v2;
	template <class T1, class T2, class T3>	inline void addVec3(T1 const& v1, T2 const& v2, T3& out);
	/// @brief out = v1 - v2;
	template <class T1, class T2, class T3>	inline void subVec3(T1 const& v1, T2 const& v2, T3& out);
	/// @brief out = v1 * v2;
	template <class T1, class T2, class T3>	inline void mulVec3(T1 const& v1, T2 const& v2, T3& out);
	/// @brief out = v1 * s;
	template <class T1, class T3>			inline void mulVec3(T1 const& v1, float s, T3& out);
	/// @brief out = v1/v2;
	template <class T1, class T2, class T3>	inline void divVec3(T1 const& v1, T2 const& v2, T3& out);
	/// @brief out = v1/s;
	template <class T1, class T2, class T3>	inline void divVec3(T1 const& v1, float s, T3& out);
	template <class T1, class T2>			inline float cross2Vec3(T1 const& a, T2 const& b);
	/// @brief out = add_v + mul_s * mul_v;
	template <class T1, class T2, class T3>	inline void addMulVec3(T1 const& v, T2 const& mul_v, float mul_s, T3& out);
	/// @brief out = add_v - mul_s * mul_v;
	template <class T1, class T2, class T3>	inline void subMulVec3(T1 const& v, T2 const& mul_v, float mul_s, T3& out);
	/// @brief 벡트 삼중적, result = u x (v x w)
	template <class T1, class T2, class T3, class T4>	inline void vecTripleProduct(T1 const& u, T2 const& v, T3 const& w, T4& out);
	/// @brief 스칼라 삼중적 result = u . (v x w)
	template <class T1, class T2, class T3>	inline void scalarTripleProduct(T1 const& u, T2 const& v, T3 const& w, float& out);
	/// @brief u축을 기준으로 v와 w의 최단 회전 방향
	template <class T1, class T2, class T3>	inline void getShortAngle(T1 const& u, T2 const& v, T3 const& w, bool& cw);
	template <class T1, class T2, class T3> inline void lerpVec3(T1 const& v1, T2 const& v2, float s, T3& out);
	/// @brief eye, at으로 각 축을 구한다
	template <class T1, class T2, class T3, class T4, class T5> inline void calcAxis(T1 const& eye, T2 const& at, T3& axisX, T4& axisY, T5& axisZ);

	inline sVector3 operator + (sVector3 const& v1, sVector3 const& v2);		/// @brief addVec3을 쓰는걸 추천함
	inline sVector3 operator - (sVector3 const& v1, sVector3 const& v2);		/// @brief subVec3을 쓰는걸 추천함
	inline sVector3 operator * (sVector3 const& v1, sVector3 const& v2);		/// @brief mulVec3을 쓰는걸 추천함
	inline sVector3 operator / (sVector3 const& v1, sVector3 const& v2);		/// @brief divVec3을 쓰는걸 추천함
	inline sVector3 operator * (float s, sVector3 const& v);						/// @brief mulVec3을 쓰는걸 추천함
	inline sVector3 operator / (float s, sVector3 const& v);						/// @brief divVec3을 쓰는걸 추천함
	/*
	 *	
	 */
	struct sVector3
	{
	public:
		sVector3()
		{
		}

		sVector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
		{
		}

		sVector3(sVector3 const& v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
		}
		
		template <class T> 
		sVector3(T const& t)
		{
			x = t.x;
			y = t.y;
			z = t.z;
		}

		~sVector3()
		{
		}

		inline sVector3 const& operator = (sVector3 const& v)
		{
			x = v.x;
			y = v.y;
			z = v.z;

			return *this;
		}

		template <class T>
		inline sVector3 const& operator += (T const& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;

			return *this;
		}

		inline sVector3 const& operator -= (sVector3 const& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;

			return *this;
		}

		inline sVector3 const& operator *= (sVector3 const& v)
		{
			x *= v.x;
			y *= v.y;
			z *= v.z;

			return *this;
		}

		inline sVector3 const& operator /= (sVector3 const& v)
		{
			if (cMath::isZero(v.x) || cMath::isZero(v.y) || cMath::isZero(v.z))
			{
				assert(0 && "division zero");
				return *this;
			}

			x /= v.x;
			y /= v.y;
			z /= v.z;

			return *this;
		}

		inline sVector3 const& operator *= (float s)
		{
			x *= s;
			y *= s;
			z *= s;

			return *this;
		}

		inline sVector3 const& operator /= (float s)
		{
			float inv = cMath::inv(s);

			x *= inv;
			y *= inv;
			z *= inv;

			return *this;
		}

		/// @brief addVec3을 쓰는걸 추천함
		inline sVector3 operator + (sVector3 const& v)
		{
			return sVector3(x+v.x, y+v.y, z+v.z);
		}

		/// @brief subVec3을 쓰는걸 추천함
		inline sVector3 operator - (sVector3 const& v)
		{
			return sVector3(x-v.x, y-v.y, z-v.z);
		}

		/// @brief mulVec3을 쓰는걸 추천함
		sVector3 operator * (sVector3 const& v)
		{
			return sVector3(x*v.x, y*v.y, z*v.z);
		}

		/// @brief divVec3을 쓰는걸 추천함
		sVector3 operator / (sVector3 const& v)
		{
			return sVector3(x*cMath::inv(v.x, 1.0f), y*cMath::inv(v.y, 1.0f), z*cMath::inv(v.z, 1.0f));
		}
		
		/// @brief mulVec3을 쓰는걸 추천함
		inline sVector3 operator * (float s)
		{
			return sVector3(s*x, s*y, s*z);
		}

		/// @brief divVec3을 쓰는걸 추천함
		inline sVector3 operator / (float s)
		{
			float invs = cMath::inv(s, 1.0f);
			return sVector3(invs*x, invs*y, invs*z);
		}
		
		inline float& operator [] (unsigned int i)
		{ 
			return (&x)[i]; 
		}
		inline float operator [] (unsigned int i) const 
		{ 
			return (&x)[i]; 
		}

		inline float length() const
		{
			return cMath::sqrtf(lengthSquare());
		}
		inline float lengthSquare() const
		{
			return x*x + y*y + z*z;
		}
		inline float length(const sVector3& v) const
		{
			sVector3 l;
			subVec3(v, *this, l);
			return l.length();
		}
		inline float lengthSquare(const sVector3& v) const
		{
			sVector3 l;
			subVec3(v, *this, l);
			return l.lengthSquare();
		}

		inline void normalize()
		{
			float inv = cMath::inv(length());

			x *= inv;
			y *= inv;
			z *= inv;
		}

		inline float dot(sVector3 const& v) const
		{
			return x * v.x + y * v.y + z * v.z;
		}

		/**
		 * @brief w위에 현재 vector를 투영
		 */
		inline void proj(sVector3 const& w, sVector3& out) const
		{
			float inv = cMath::inv(w.lengthSquare());
			float a = dot(w) * inv;
			mulVec3(w, a, out);
		}
		/**
		 * @brief w에서 this로 향하는 벡터를 구한다
		 */
		inline void perp(sVector3 const& w, sVector3& out) const
		{
			sVector3 vecProj;
			proj(w, vecProj);
			subVec3(*this, vecProj, out);
		}
		/*
			@brief this -> v로, 왼손
		*/
		template <typename T1, typename T2>
		inline void cross(T1 const& v, T2& out) const
		{
			out.x = y * v.z - z * v.y;
			out.y = z * v.x - x * v.z;
			out.z = x * v.y - y * v.x;
		}
		/*
		*/
		template <typename T>
		void to3(T& t) const
		{
			t.x = x;
			t.y = y;
			t.z = z;
		}
		/*
		*/
		template <typename T>
		void to4(T& t) const
		{
			t.x = x;
			t.y = y;
			t.z = z;
			t.w = 1.0f;	// 0.0f 또는 1.0f 둘중에 뭘로 해야 되나?
		}

		/**
		 * @brief 두 벡터의 각을 구한다, this와 v는 단위 벡터이어야 한다, up 벡터가 y축 벡터일 때만 쓰임
		 */
		inline float angle(sVector3 const& v) const
		{
			float d = dot(v);
			float crossY = z * v.x - x * v.z;
			float ret = cMath::acosf(d);
			return (crossY > 0.0f) ? -ret : ret;
		}
		/*
		 * @brief 두 벡터의 각을 구한다, this와 v는 단위 벡터이어야 한다,
		*/
		inline float angle(sVector3 const& v, sVector3 const& up) const
		{
			float d = dot(v);
			float ret = cMath::acosf(d);

			sVector3 crossV;
			cross(v, crossV);
			float dotUp = crossV.dot(up);
			return (dotUp > 0.0f) ? -ret : ret;
		}
		/**
		 * 
		 * @brief u를 축으로 해서 v->w로 회전할 때, 최단 회전 방향
		 * @param u 회전 축
		 * @param v 
		 * @param w
		 * @return 0: ccw, 1: cw
		 */
		static int determinShortAngle(const sVector3& u, const sVector3& v, const sVector3& w)
		{
			if (sVector3::scalarTripleProduct(u, v, w) > 0.0f)
				return 0;
			return 1;
		}
		/**
		* @return u, v, w가 이루는 박스의 부피와 같다, 
		*/
		static float scalarTripleProduct(const sVector3& u, const sVector3& v, const sVector3& w)
		{
			sVector3 crossVec;
			v.cross(w, crossVec);
			return u.dot(crossVec);
		}
		/**
		 * @brief u, v, w에 수직인 적규직교 기저를 얻는다
		 */
		static sVector3 vectorTripleProduct(sVector3 const& u, sVector3 const& v, sVector3 const& w, sVector3& out)
		{
			sVector3 crossVec;
			v.cross(w, crossVec);
			u.cross(crossVec, out);
		}

		/**
		 * @brief p0, p1, p2가 이루는 삼각형 안에 현재의 vector가 포함되어 있는지 아닌지,
		 */
		inline bool inTriangle(const sVector3& p0, const sVector3& p1, const sVector3& p2) const
		{
			sVector3 v0, v1, v2;
			subVec3(p1, p0, v0);
			subVec3(p2, p1, v1);
			subVec3(p0, p2, v2);

			sVector3 w0, w1, w2;
			subVec3(*this, p0, w0);
			subVec3(*this, p1, w1);
			subVec3(*this, p2, w2);

			sVector3 triangleNorm;
			v0.cross(v1, triangleNorm);

			sVector3 n0, n1, n2;
			v0.cross(w0, n0);
			v1.cross(w1, n1);
			v2.cross(w2, n2);

			float r0 = triangleNorm.dot(n0);
			float r1 = triangleNorm.dot(n1);
			float r2 = triangleNorm.dot(n2);

			if ((r0 < 0.0f && r1 < 0.0f && r2 < 0.0f) || 
				(r0 >= 0.0f && r1 >= 0.0f && r2 >= 0.0f))
				return true;

			return false;
		}
		/*
		 *	
		 */
		inline bool equal(sVector3 const& v) const
		{
			if (cMath::equal(x, v.x) && cMath::equal(y, v.y) && cMath::equal(z, v.z))
				return true;
			return false;
		}
		/*
		 *	@brief 두 직선 사이의 라디안 각도를 구한다(두 벡터 모두 정규화 되어져 있어야 한다)
		 */
		inline float getRadian(sVector3 const& norm1) const
		{
			float _dot = dot(norm1);
			float radian = cMath::acosf(_dot);
			if (norm1.z > z)
				radian = cMath::TWOPI - radian;
			return radian;
		}
		/*
		 *	
		 */
		inline void toString(TCHAR* str) const
		{
			_stprintf(str, _T("x:%f y:%f z:%f"), x, y, z);
		}
		/*
		*/
		inline void negate()
		{
			x = -x;
			y = -y;
			z = -z;
		}
		/*
		*/
		inline bool isZero() const
		{
			if (cMath::isZero(x) && cMath::isZero(y) && cMath::isZero(z))
				return true;
			return false;
		}

	public:
		float x;
		float y;
		float z;

		static sVector3 ZERO;
		static sVector3 X_BASIS;
		static sVector3 Y_BASIS;
		static sVector3 Z_BASIS;
	};

	

	/*
	 *	
	 */
	template <class T1, class T2, class T3>
	void addVec3(T1 const& v1, T2 const& v2, T3& out)
	{
		out.x = v1.x + v2.x;
		out.y = v1.y + v2.y;
		out.z = v1.z + v2.z;
	}
	/*
	 *	
	 */
	template <class T1, class T2, class T3>
	void subVec3(T1 const& v1, T2 const& v2, T3& out)
	{
		out.x = v1.x - v2.x;
		out.y = v1.y - v2.y;
		out.z = v1.z - v2.z;
	}
	/*
	 *	
	 */
	template <class T1, class T2, class T3>
	void mulVec3(T1 const& v1, T2 const& v2, T3& out)
	{
		out.x = v1.x * v2.x;
		out.y = v1.y * v2.y;
		out.z = v1.z * v2.z;
	}
	/*
	 *	
	 */
	template <class T1, class T3>
	void mulVec3(T1 const& v1, float s, T3& out)
	{
		out.x = v1.x * s;
		out.y = v1.y * s;
		out.z = v1.z * s;
	}
	/*
	 *	
	 */
	template <class T1, class T2, class T3>
	void divVec3(T1 const& v1, T2 const& v2, T3& out)
	{
		if (cMath::isZero(v2.x) || cMath::isZero(v2.y) || cMath::isZero(v2.z))
		{
			assert(0 && "division zero");
			return ;
		}

		out.x = v1.x / v2.x;
		out.y = v1.y / v2.y;
		out.z = v1.z / v2.z;
	}
	/*
	 *	
	 */
	template <class T1, class T2, class T3>
	void divVec3(T1 const& v1, float s, T3& out)
	{
		float inv = cMath::inv(s);
		mulVec3(v1, inv, out);
	}
	/*
	 *	
	 */
	template <class T1, class T2, class T3>
	void addMulVec3(T1 const& v, T2 const& mul_v, float mul_s, T3& out)
	{
		mulVec3(mul_v, mul_s, out);
		out += v;
	}
	/*
	 *	
	 */
	template <class T1, class T2, class T3>
	void subMulVec3(T1 const& v, T2 const& mul_v, float mul_s, T3& out)
	{
		mulVec3(mul_v, mul_s, out);
		out.x = v.x - out.x;
		out.y = v.y - out.y;
		out.z = v.z - out.z;
	}
	/*
	 *	
	 */
	template <class T1, class T2>
	float cross2Vec3(T1 const& a, T2 const& b)
	{
		return a.x*b.z - a.z*b.x;
	}
	/*
	 *	
	 */
	template <class T1, class T2, class T3, class T4>
	void vecTripleProduct(T1 const& u, T2 const& v, T3 const& w, T4& out)
	{
		sVector3 vw;
		v.cross(w, vw);
		u.cross(vw, out);
	}
	/*
	 *	
	 */
	template <class T1, class T2, class T3>
	void scalarTripleProduct(T1 const& u, T2 const& v, T3 const& w, float& out)
	{
		sVector3 vw;
		v.cross(w, vw);
		out = u.dot(vw);
	}
	/*
	 *	
	 */
	template <class T1, class T2, class T3>
	void getShortAngle(T1 const& u, T2 const& v, T3 const& w, bool& cw)
	{
		float out;
		scalarTripleProduct(u, v, w, out);
		cw = (out < 0.0f) ? true : false;
	}
	/*
	*/
	template <class T1, class T2, class T3> 
	inline void lerpVec3(T1 const& s, T2 const& e, float t, T3& out)
	{
		out.x = cMath::lerp(s.x, e.x, t);
		out.y = cMath::lerp(s.y, e.y, t);
		out.z = cMath::lerp(s.z, e.z, t);
	}
	/*
	*/
	template <class T1, class T2, class T3, class T4, class T5> 
	inline void calcAxis(T1 const& eye, T2 const& at, T3& axisX, T4& axisY, T5& axisZ)
	{
		// z
		subVec3(at, eye, axisZ);
		axisZ.normalize();
		// x
		sVector3 up(0.0f, 1.0f, 0.0f);
		up.cross(axisZ, axisX);
		axisX.normalize();
		// y
		axisZ.cross(axisX, axisY);
		axisY.normalize();
	}
	/*
	*/
	sVector3 operator + (sVector3 const& v1, sVector3 const& v2)
	{
		return sVector3(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z);
	}
	/*
	*/
	sVector3 operator - (sVector3 const& v1, sVector3 const& v2)
	{
		return sVector3(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z);
	}	
	/*
	*/
	sVector3 operator * (sVector3 const& v1, sVector3 const& v2)
	{
		return sVector3(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z);
	}
	/*
	*/
	sVector3 operator / (sVector3 const& v1, sVector3 const& v2)
	{
		return sVector3(v1.x*cMath::inv(v2.x, 1.0f), v1.y*cMath::inv(v2.y, 1.0f), v1.z*cMath::inv(v2.z, 1.0f));
	}
	/*
	*/
	sVector3 operator * (float s, sVector3 const& v)
	{
		return sVector3(s*v.x, s*v.y, s*v.z);
	}
	/*
	*/
	sVector3 operator / (float s, sVector3 const& v)
	{
		float invs = cMath::inv(s, 1.0f);
		return sVector3(invs*v.x, invs*v.y, invs*v.z);
	}
}








#endif





