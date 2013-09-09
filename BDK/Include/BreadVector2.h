
#ifndef _BreadVector2_h_
#define _BreadVector2_h_

#include <assert.h>
#include "BreadMath.h"

namespace Bread
{
	#define V2(x, y) sVector2(x, y)

	/**
	 * \ingroup Core
	 *
	 *
	 * \par requirements
	 * win98 or later\n
	 * win2k or later\n
	 * MFC\n
	 *
	 * \version 1.0
	 * first version
	 *
	 * \date 2009-08-22
	 *
	 * \author Prodongi
	 *
	 * \par license
	 * This code is absolutely free to use and modify. The code is provided "as is" with
	 * no expressed or implied warranty. The author accepts no liability if it causes
	 * any damage to your computer, causes your pet to fall ill, increases baldness
	 * or makes your car start emitting strange noises when you start it up.
	 * This code has no bugs, just undocumented features!
	 * 
	 * \todo 
	 *
	 * \bug 
	 *
	 */
	struct sVector2
	{
	public:
		sVector2()
		{
		}

		sVector2(float x, float y) : x(x), y(y)
		{
		}

		sVector2(const sVector2& v)
		{
			x = v.x;
			y = v.y;
		}

		~sVector2()
		{
		}

		inline sVector2& operator = (const sVector2& v)
		{
			x = v.x;
			y = v.y;

			return *this;
		}

		inline sVector2 operator - () const
		{
			return sVector2(-x, -y);
		}

		inline sVector2 operator + (const sVector2& v) const
		{
			return sVector2(x + v.x, y + v.y);
		}

		inline sVector2 operator - (const sVector2& v) const
		{
			return sVector2(x - v.x, y - v.y);
		}

		inline sVector2 operator * (const sVector2& v) const
		{
			return sVector2(x * v.x, y * v.y);
		}

		inline sVector2 operator / (const sVector2& v) const
		{
			assert(!cMath::isZero(v.x) && !cMath::isZero(v.y));

			return sVector2(x / v.x, y / v.y);
		}

		inline const sVector2& operator += (const sVector2& v)
		{
			x += v.x;
			y += v.y;

			return *this;
		}

		inline const sVector2& operator -= (const sVector2& v)
		{
			x -= v.x;
			y -= v.y;

			return *this;
		}

		inline const sVector2& operator *= (const sVector2& v)
		{
			x *= v.x;
			y *= v.y;

			return *this;
		}

		inline const sVector2& operator /= (const sVector2& v)
		{
			assert(!cMath::isZero(v.x) && !cMath::isZero(v.y));

			x /= v.x;
			y /= v.y;

			return *this;
		}

		inline sVector2 operator * (float s) const
		{
			return sVector2(x * s, y * s);
		}

		inline sVector2 operator / (float s) const
		{
			assert(!cMath::isZero(s));

			float inv = 1.0f/s;
			return sVector2(x * inv, y * inv);
		}

		inline const sVector2& operator *= (float s)
		{
			x *= s;
			y *= s;

			return *this;
		}

		inline const sVector2& operator /= (float s)
		{
			assert(!cMath::isZero(s));

			float inv = 1.0f/s;

			x *= inv;
			y *= inv;

			return *this;
		}

		inline friend sVector2 operator * (float s, const sVector2& v)
		{
			return v * s;
		}
		inline friend sVector2 operator / (float s, const sVector2& v)
		{
			assert(!cMath::isZero(s));

			return v / s;
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
			return x*x + y*y;
		}
		inline float length(sVector2 const& v) const
		{
			sVector2 l = v - *this;
			return l.length();
		}
		inline float lengthSquare(sVector2 const& v) const
		{
			sVector2 l = v - *this;
			return l.lengthSquare();
		}
		inline float fastLength() const
		{
			float _max = cMath::_max(cMath::fabsf(x), cMath::fabsf(y));
			float _min = cMath::_min(cMath::fabsf(x), cMath::fabsf(y));

			static float v1 = 1007.0f/1024.0f;
			static float v2 = 441.0f/1024.0f;
			static float v3 = 5.0f/128.0f;

			float a = v1 * _max;
			float b = v2 * _min;
			float c = (_max < 16.0f * _min) ? v3 * _max : 0.0f;
			float ret = a + b - c;
			return ret;
		}
		inline float fastLength(sVector2 const& v) const
		{
			sVector2 l = v - *this;
			return l.fastLength();
		}

		inline void normalize()
		{
			float inv = cMath::inv(length());

			x *= inv;
			y *= inv;
		}

		inline float dot(const sVector2& v) const
		{
			return x * v.x + y * v.y;
		}

		/**
		* @brief w위에 현재 vector를 투영
		*/
		inline sVector2 proj(const sVector2& w) const
		{
			float a = dot(w)/w.lengthSquare();
			return w * a;
		}
		/**
		* @brief w에서 this로 향하는 벡터를 구한다
		*/
		inline sVector2 perp(const sVector2& w) const
		{
			return *this - proj(w);
		}

		/**
		* @brief 두 벡터의 각을 구한다
		*/
		inline float angle(const sVector2& v) const
		{
			float d = dot(v);
			float l = length() * v.length();

			assert(!cMath::isZero(l));

			return cMath::acosf(d/l);
		}

	public:
		float x;
		float y;

		static sVector2 ZERO;
		static sVector2 X_BASIS;
		static sVector2 Y_BASIS;
	};
}








#endif





