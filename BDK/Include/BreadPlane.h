
#ifndef _BreadPlane_h_
#define _BreadPlane_h_

#include "BreadVector3.h"

namespace Bread
{
	/** 
	 * @class sPlane 
	   @brief ax + by + cz + d = 0;
	 */
	struct sPlane
	{
	public:
		sPlane() {}
		/**
		 *
		 * \param a normal x
		 * \param b normal y
		 * \param c normal z
		 * \param d 
		 */
		sPlane(float _a, float _b, float _c, float _d)
		{
			a = _a;
			b = _b;
			c = _c;
			d = _d;
		}
		sPlane(sPlane const& plane)
		{
			a = plane.a;
			b = plane.b;
			c = plane.c;
			d = plane.d;
		}
		template <typename T>
		sPlane(T const& t)
		{
			a = t.a;
			b = t.b;
			c = t.c;
			d = t.d;
		}
		~sPlane()	{}

		inline float& operator [] (unsigned int i)
		{ 
			return (&a)[i]; 
		}
		inline float operator [] (unsigned int i) const 
		{ 
			return (&a)[i]; 
		}

		float test(const sVector3& point) const
		{
			sVector3 normal(a, b, c);
			return normal.dot(point) + d;
		}

		/// 세 점을 이용해서 평면의 방정식을 만든다
		void make(sVector3 const& v1, sVector3 const& v2, sVector3 const& v3)
		{
			sVector3 v12, v32, normal;
			subVec3(v1, v2, v12);
			subVec3(v3, v2, v32);
			v12.cross(v32, normal);
			normal.normalize();

			a = normal.x;
			b = normal.y;
			c = normal.z;
			d = normal.dot(v1);
		}
		
		float dotCoord(sVector3 const& v) const
		{
			return a*v.x + b*v.y + c*v.z + d;
		}

		void normalize()
		{
			float inv_len = cMath::invSqrtf(a*a + b*b + c*c);
			a *= inv_len;
			b *= inv_len;
			c *= inv_len;
			d *= inv_len;
		}
			
		template <typename T>
		void toD3D(T& t) const
		{
			t.a = a;
			t.b = b;
			t.c = c;
			t.d = d;
		}

	public:
		float a;
		float b;
		float c;
		float d;
	};
}




#endif









