
#ifndef _BreadMath_h_
#define _BreadMath_h_

#include <stdlib.h>
#include <time.h>
#include <cassert>
#include <math.h>
#include <float.h>

namespace Bread
{
	struct sVector3;
	struct sRand;

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
	 * \date 2010-01-20
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
	class cMath
	{
	public:

		cMath()		{}
		~cMath()	{}
		static void clear();

		#define DEG_TO_RAD				0.01745329f//(PI/(float)180.0f)
		#define RAD_TO_DEG				57.29577951f//((float)180.0f/PI)

		/// @param retZero v가 0일 때의 리턴 값
		static inline float inv(float v, float retZero = 0.0f)
		{
			if (cMath::isZero(v))
			{
				//assert (0 && "v == 0.0f");
				return retZero;
			}

			return 1.0f/v;
		}
		static inline float sqrtf(float v)				{	return ::sqrtf(v);				}
		static inline float invSqrtf(float v)			{	return inv(cMath::sqrtf(v));	}
		static inline float acosf(float v)				{	return ::acosf(v);				}
		static inline float asinf(float v)				{	return ::asinf(v);				}
		static inline float atanf(float y, float x)		{	return ::atan2f(y, x);			}
		static inline float sinf(float v)				{	return ::sinf(v);				}
		static inline float cosf(float v)				{	return ::cosf(v);				}
		static inline float tanf(float v)				{	return ::tanf(v);				}
		static inline float fabsf(float v)				{	return ::fabsf(v);				}
		static inline bool isZero(float value)			{	return cMath::fabsf(value) <= cMath::EPSILON;	}
		static inline float degToRad(float deg)			{	return deg*DEG_TO_RAD;			}
		static inline float radToDeg(float rad)			{	return rad*RAD_TO_DEG;			}
		static inline bool equal(float x1, float x2)	{	return isZero(x2 - x1);			}
		static bool isInFace(sVector3 const* face, sVector3 const& p);
		static bool isInFace(sVector3 const& face0, sVector3 const& face1, sVector3 const& face2, sVector3 const& p);
		static inline void getHeightAtFace(sVector3 const& face0, sVector3 const& face1, sVector3 const& face2, sVector3& p);
		static unsigned int getLargestPow2(unsigned int num);
		static void initRand(int type = 2);	/// @param type 0: linear congruential, 1: mersenne twister, 2:wellRng512
		static unsigned long rand();
		static unsigned long rand(unsigned long range);
		static float rand(float amount, float range);	/// @param 0.0f <= value <= amount 값을 리턴
		static float lerp(float s, float e, float t)	{	return s + (e-s)*t;	}
		template <typename T> static T _max(T const& a, T const& b)	{	return (a > b) ? a : b;	}
		template <typename T> static T _min(T const& a, T const& b)	{	return (a > b) ? b : a;	}
		template <typename T> static void saturate(T& value, T const& min, T const& max)
		{
			if (min > value)		 value = min;
			else if (max < value) value = max;
		}
		template <typename T> static void wrap(T& value, T const& min, T const& max)
		{
			if (min > value)		 value = max;
			else if (max < value) value = min;
		}
		static inline void wrapRadian(float& radian)
		{
			if (0.0f > radian)
				radian += TWOPI;
			else if (TWOPI < radian)
				radian -= TWOPI;
		}

	public:
		static const float EPSILON;
		static const float PI;
		static const float HALFPI;
		static const float TWOPI;
	private:
		static sRand* randGenerator;
	};
}










#endif













