
#ifndef _BreadMatrix3_h_
#define _BreadMatrix3_h_

#include "BreadVector3.h"

namespace Bread
{
	/*
	*/
	struct sMatrix3
	{
	public:
		sMatrix3()
		{
		}
		sMatrix3(float m00, float m01, float m02,
				 float m10, float m11, float m12,
				 float m20, float m21, float m22)
		{
			m[0][0] = m00;
			m[0][1] = m01;
			m[0][2] = m02;

			m[1][0] = m10;
			m[1][1] = m11;
			m[1][2] = m12;

			m[2][0] = m20;
			m[2][1] = m21;
			m[2][2] = m22;
		}
		~sMatrix3()
		{
		}
		inline float* operator [] (int row) const
		{
			return (float*)m[row];
		}
		inline sMatrix3 const& operator = (sMatrix3 const& mat)
		{
			memcpy(m, mat.m, sizeof (float) * 3 * 3);
			return *this;
		}
		inline sMatrix3 operator * (sMatrix3 const& mat)
		{
			sMatrix3 out;

			out.m[0][0] = m[0][0]*mat.m[0][0] + m[0][1]*mat.m[1][0] + m[0][2]*mat.m[2][0];
			out.m[0][1] = m[0][0]*mat.m[0][1] + m[0][1]*mat.m[1][1] + m[0][2]*mat.m[2][1];
			out.m[0][2] = m[0][0]*mat.m[0][2] + m[0][1]*mat.m[1][2] + m[0][2]*mat.m[2][2];

			out.m[1][0] = m[1][0]*mat.m[0][0] + m[1][1]*mat.m[1][0] + m[1][2]*mat.m[2][0];
			out.m[1][1] = m[1][0]*mat.m[0][1] + m[1][1]*mat.m[1][1] + m[1][2]*mat.m[2][1];
			out.m[1][2] = m[1][0]*mat.m[0][2] + m[1][1]*mat.m[1][2] + m[1][2]*mat.m[2][2];

			out.m[2][0] = m[2][0]*mat.m[0][0] + m[2][1]*mat.m[1][0] + m[2][2]*mat.m[2][0];
			out.m[2][1] = m[2][0]*mat.m[0][1] + m[2][1]*mat.m[1][1] + m[2][2]*mat.m[2][1];
			out.m[2][2] = m[2][0]*mat.m[0][2] + m[2][1]*mat.m[1][2] + m[2][2]*mat.m[2][2];

			return out;
		}
		inline sVector3 operator * (sVector3 const& v)
		{
			sVector3 out;
			out.x = (m[0][0]*v.x + m[1][0]*v.y + m[2][0]*v.z);
			out.y = (m[0][1]*v.x + m[1][1]*v.y + m[2][1]*v.z);
			out.z = (m[0][2]*v.x + m[1][2]*v.y + m[2][2]*v.z);

			return out;
		}
		template <typename T>
		inline void transpose(T& out) const
		{
			out.m[0][0] = m[0][0];
			out.m[0][1] = m[1][0];
			out.m[0][2] = m[2][0];

			out.m[1][0] = m[0][1];
			out.m[1][1] = m[1][1];
			out.m[1][2] = m[2][1];

			out.m[2][0] = m[0][2];
			out.m[2][1] = m[1][2];
			out.m[2][2] = m[2][2];
		}

		inline float determinant() const;
		inline void inverse(sMatrix3& out) const;
		void rotationX(float radian);
		void rotationY(float radian);
		void rotationZ(float radian);

		float trace() const
		{
			return m[0][0] + m[1][1] + m[2][2];
		}

	public:
		union
		{
			float m[3][3];
			float m_[9];
		};

		static const sMatrix3 IDENTITY;
	};

	inline sVector3 operator * (sVector3 const& v, sMatrix3 const& m)
	{
		sVector3 out;

		out.x = (m[0][0]*v.x + m[1][0]*v.y + m[2][0]*v.z);
		out.y = (m[0][1]*v.x + m[1][1]*v.y + m[2][1]*v.z);
		out.z = (m[0][2]*v.x + m[1][2]*v.y + m[2][2]*v.z);

		return out;
	}
}



#endif













