
#ifndef _BreadQuaternion_h_
#define _BreadQuaternion_h_

#include "BreadMath.h"
#include "BreadMatrix3.h"
#include "BreadVector3.h"

namespace Bread
{
	struct sQuaternion;

	inline void addQuat(sQuaternion const& quat1, sQuaternion const& quat2, sQuaternion& out);
	inline void subQuat(sQuaternion const& quat1, sQuaternion const& quat2, sQuaternion& out);
	inline void mulQuat(sQuaternion const& quat1, sQuaternion const& quat2, sQuaternion& out);
	inline void mulQuat(sQuaternion const& quat1, sQuaternion const& quat2, sQuaternion const& quat3, sQuaternion& out);
	inline void mulQuat(sQuaternion const& quat1, float scalar, sQuaternion& out);
	/// @brief quat와 quat conju와의 곱
	inline float mulQuatConju(sQuaternion const& quat);
	/// @brief p를 axis축 주위로 theta만큼 회전한 vector를 구한다
	inline void rotQuat(sVector3 const& p, sVector3 const& axis, float theta, sVector3& out);

	/**
	 * @class sQuaternion	
	 */
	struct sQuaternion
	{
	public:
		sQuaternion() : w(1.0f)
		{
		}
		sQuaternion(float _w, float _x, float _y, float _z) : w(_w), x(_x), y(_y), z(_z)
		{
		}
		~sQuaternion()
		{
		}

		inline sQuaternion& operator = (const sQuaternion& quat)
		{
			x = quat.x;	y = quat.y; z = quat.z; w = quat.w;
			return *this;
		}
		inline bool operator == (const sQuaternion& quat) const
		{
			if (cMath::isZero(x - quat.x) && cMath::isZero(y - quat.y) &&
				cMath::isZero(z - quat.z) && cMath::isZero(w - quat.w))
				return true;
			return false;
		}
		inline bool operator != (const sQuaternion& quat) const
		{
			return !operator==(quat);
		}

		void conjuntion(sQuaternion& conju) const
		{
			conju.x = -x;
			conju.y = -y;
			conju.z = -z;
			conju.w = w;
		}
		float normSqr() const
		{
			return x*x + y*y + z*z + w*w;
		}
		float norm() const
		{
			return cMath::sqrtf(normSqr());
		}
		void inverse(sQuaternion& quat) const
		{
			float n = norm();
			if (n > 0.0f)
			{
				float invNorm = cMath::inv(n);
				sQuaternion conju;
				conjuntion(conju);
				mulQuat(conju, invNorm, quat);
			}
		}
		void normalize()
		{
			float inv_n = cMath::inv(norm());
			x *= inv_n;
			y *= inv_n;
			z *= inv_n;
			w *= inv_n;
		}
		// @brief 리턴값은 cos값이다
		float dot(sQuaternion const& quat) const
		{
			return x*quat.x + y*quat.y + z*quat.z + w*quat.w;
		}
		void set(float _w, float _x, float _y, float _z)
		{
			w = _w;	x = _x;	y = _y;	z = _z;
		}
		/// @param axis : 회전축 단위벡터, theta : 회전 각도(radian)
		/// @brief q = cos(radian/2) + sin(radian/2) * axis
		void set(sVector3 const& axis, float theta);
		/// @brief 쿼터니온으로 부터 회전축(axis)와 회전각도(radian)을 구한다
		void get(sVector3& axis, float& theta);
		/// @param pitch : radian_x, yaw : y, roll : z
		void fromEuler(float pitch, float yaw, float roll);
		/// @brief 선형 보간
		template <typename T1, typename T2>
		void lerp(T1 const& s, T2 const& e, float st, float et)
		{
			x = s.x*st + e.x*et;
			y = s.y*st + e.y*et;
			z = s.z*st + e.z*et;
			w = s.w*st + e.w*et;
		}
		/// @brief 선형 보간 q1(t) ~ q2(1.0f-t)
		template <typename T1, typename T2>
		void lerp(T1 const& sq, T2 const& eq, float t)
		{
			float cosTheta = sq.dot(eq);
			float st, et;

			et = t;
			if (cosTheta >= cMath::EPSILON)
			{
				st = 1.0f - t;
			}
			else
			{
				st = t - 1.0f;
			}

			lerp(sq, eq, st, et);
		}
		/// @brief 구면 선형 보간 q1(t) ~ q2(1.0f-t)
		template <typename T1, typename T2>
		void slerp(T1 const& sq, T2 const& eq, float t)
		{
			float cosTheta = sq.dot(eq);
			float st, et;

			if (cosTheta >= cMath::EPSILON)
			{
				if ((1.0f - cosTheta) > cMath::EPSILON)
				{
					float theta = cMath::acosf(cosTheta);
					float recipSinTheta = cMath::inv(cMath::sinf(theta));

					st = cMath::sinf((1.0f-t)*theta)*recipSinTheta;
					et = cMath::sinf(t*theta)*recipSinTheta;
				}
				else
				{
					st = 1.0f - t;
					et = t;
				}
			}
			else
			{
				if ((1.0f + cosTheta) > cMath::EPSILON)
				{
					float theta = cMath::acosf(-cosTheta);
					float recipSinTheta = cMath::inv(cMath::sinf(theta));

					st = cMath::sinf((t-1.0f)*theta)*recipSinTheta;
					et = cMath::sinf(t*theta)*recipSinTheta;
				}
				else
				{
					st = t - 1.0f;
					et = t;
				}
			}

			lerp(sq, eq, st, et);
		}
		/*	@brief Jonathan Blow method
		*/
		template <typename T1, typename T2>
		void slerpFast(T1 const& sq, T2 const& eq, float t)
		{
			float cosTheta = sq.dot(eq);

			float temp = 1.0f - 0.7878088f * cosTheta;
			float k = 0.5069269f * pow(temp, 2);

			float _t = 2.0f * k * pow(t, 3) - 3.0f * k * pow(t, 2) + (1.0f + k) * t;

			float st, et;
			et = _t;
			if (cosTheta >= cMath::EPSILON)
			{
				st = 1.0f - _t;
			}
			else
			{
				st = _t - 1.0f;
			}

			if (0.5f > t)
			{
				lerp(sq, eq, st, et);
			}
			else
			{
				lerp(eq, sq, st, et);
			}
		}
		/// @brief v0에서 v1로 회전하는 쿼터니언 생성, gpg1 2.10에서 갖고옴
		template <typename T>
		void makeRotation(T v0, T v1)
		{
			v0.normalize();
			v1.normalize();
			T c;
			v0.cross(v1, c);
			float d = v0.dot(v1);
			float s = cMath::sqrtf((1.0f+d)*2.0f);
			x = c.x/s;
			y = c.y/s;
			z = c.z/s;
			w = s/2.0f;
		}
		/*
		 *	
		 */
		template <typename T>
		inline void to(T& t) const
		{
			t.w = w;
			t.x = x;
			t.y = y;
			t.z = z;
		}
		/*
		*/
		template <typename T>
		inline void toMatrix(T& t) const
		{
			float x2 = x * x;
			float y2 = y * y;
			float z2 = z * z;

			t.m[0][0] = 1.0f - 2.0f * y2 - 2.0f * z2;
			t.m[0][1] = 2.0f * x * y - 2.0f * w * z;
			t.m[0][2] = 2.0f * x * z + 2.0f * w * y;
			t.m[0][3] = 0.0f;

			t.m[1][0] = 2.0f * x * y + 2.0f * w * z;
			t.m[1][1] = 1.0f - 2.0f * x2 - 2.0f * z2;
			t.m[1][2] = 2.0f * y * z - 2.0f * w * x;
			t.m[1][3] = 0.0f;

			t.m[2][0] = 2.0f * x * z - 2.0f * w * y;
			t.m[2][1] = 2.0f * y * z + 2.0f * w * x;
			t.m[2][2] = 1.0f - 2.0f * x2 - 2.0f * y2;
			t.m[2][3] = 0.0f;

			t.m[3][0] = 0.0f;
			t.m[3][1] = 0.0f;
			t.m[3][2] = 0.0f;
			t.m[3][3] = 1.0f;
		}
		/*
		 *	
		 */
		inline void toString(TCHAR* str) const
		{
			_stprintf(str, _T("w:%f x:%f y:%f z:%f"), w, x, y, z);
		}

		sQuaternion exp();
		sQuaternion log();

		inline void makeFromRotMatrix(const sMatrix3& mat);

	public:
		float x, y, z, w;

		static const sQuaternion ZERO;
		static const sQuaternion IDENTITY;

	};

	/*
	 *	
	 */
	void addQuat(sQuaternion const& quat1, sQuaternion const& quat2, sQuaternion& out)
	{
		out.x = quat1.x + quat2.x;
		out.y = quat1.y + quat2.y;
		out.z = quat1.z + quat2.z;
		out.w = quat1.w + quat2.w;
	}
	/*
	 *	
	 */
	void subQuat(sQuaternion const& quat1, sQuaternion const& quat2, sQuaternion& out)
	{
		out.x = quat1.x - quat2.x;
		out.y = quat1.y - quat2.y;
		out.z = quat1.z - quat2.z;
		out.w = quat1.w - quat2.w;
	}
	/*
	 *	
	 */
	void mulQuat(sQuaternion const& quat1, sQuaternion const& quat2, sQuaternion& out)
	{
		out.x = quat1.w * quat2.x + quat1.x * quat2.w + quat1.y * quat2.z - quat1.z * quat2.y;
		out.y = quat1.w * quat2.y + quat1.y * quat2.w + quat1.z * quat2.x - quat1.x * quat2.z;
		out.z = quat1.w * quat2.z + quat1.z * quat2.w + quat1.x * quat2.y - quat1.y * quat2.x;
		out.w = quat1.w * quat2.w - quat1.x * quat2.x - quat1.y * quat2.y - quat1.z * quat2.z;
	}
	/*
	 *	
	 */
	void mulQuat(sQuaternion const& quat1, sQuaternion const& quat2, sQuaternion const& quat3, sQuaternion& out)
	{
		sQuaternion quat12;
		mulQuat(quat1, quat2, quat12);
		mulQuat(quat12, quat3, out);
	}
	/*
	 *	
	 */
	void mulQuat(sQuaternion const& quat1, float scalar, sQuaternion& out)
	{
		out.x = quat1.x * scalar;
		out.y = quat1.y * scalar;
		out.z = quat1.z * scalar;
		out.w = quat1.w * scalar;
	}
	/*
	 *	
	 */
	float mulQuatConju(sQuaternion const& quat)
	{
		return quat.x*quat.x + quat.y*quat.y + quat.z*quat.z + quat.w*quat.w;
	}
	/*
	 *	
	 */
	void rotQuat(sVector3 const& p, sVector3 const& axis, float theta, sVector3& out)
	{
		sQuaternion v, qr, qr_conju, qout;

		v.set(0.0f, p.x, p.y, p.z);
		qr.set(axis, theta);
		qr.conjuntion(qr_conju);
		mulQuat(qr, v, qr_conju, qout);
		out.x = qout.x;
		out.y = qout.y;
		out.z = qout.z;
	}
}












#endif











