
#include "BreadMatrix3.h"
#include "BreadMath.h"

namespace Bread
{
	const sMatrix3 sMatrix3::IDENTITY = sMatrix3(1.0f, 0.0f, 0.0f,
												 0.0f, 1.0f, 0.0f,
												 0.0f, 0.0f, 1.0f);

	/**
	 * @brief det(A) = Sum(j=1,n) ai,j(-1)e(i+j) * det(Acoi,j)
	 *		  Ogre3D에서 참조
	 */
	float sMatrix3::determinant() const
	{
		float cofactor00 = m[1][1]*m[2][2] - m[1][2]*m[2][1];
		float cofactor10 = m[1][2]*m[2][0] - m[1][0]*m[2][2];
		float cofactor20 = m[1][0]*m[2][1] - m[1][1]*m[2][0];

		float det = m[0][0]*cofactor00 +
					m[0][1]*cofactor10 +
					m[0][2]*cofactor20;

		return det;
	}
	/**
	 *
	 * Ogre3D에서 참조
	 */
	void sMatrix3::inverse(sMatrix3& out) const
	{
		// Invert a 3x3 using cofactors.  This is about 8 times faster than
		// the Numerical Recipes code which uses Gaussian elimination.

		out[0][0] = m[1][1]*m[2][2] - m[1][2]*m[2][1];
		out[0][1] = m[0][2]*m[2][1] - m[0][1]*m[2][2];
		out[0][2] = m[0][1]*m[1][2] - m[0][2]*m[1][1];
		out[1][0] = m[1][2]*m[2][0] - m[1][0]*m[2][2];
		out[1][1] = m[0][0]*m[2][2] - m[0][2]*m[2][0];
		out[1][2] = m[0][2]*m[1][0] - m[0][0]*m[1][2];
		out[2][0] = m[1][0]*m[2][1] - m[1][1]*m[2][0];
		out[2][1] = m[0][1]*m[2][0] - m[0][0]*m[2][1];
		out[2][2] = m[0][0]*m[1][1] - m[0][1]*m[1][0];

		float det = m[0][0]*out[0][0] +
					m[0][1]*out[1][0] +
					m[0][2]*out[2][0];

		if (cMath::isZero(det))
		{
			out = sMatrix3::IDENTITY;
			return ;
		}

		float invDet = 1.0f/det;
		for (int row = 0; row < 3; ++row)
		{
			for (int col = 0; col < 3; ++col)
				out[row][col] *= invDet;
		}
	}
	/*
	*/
	void sMatrix3::rotationX(float radian)
	{
		float sinTheta = cMath::sinf(radian);
		float cosTheta = cMath::cosf(radian);

		m_[0] = 1.0f;
		m_[1] = 0.0f;
		m_[2] = 0.0f;
		m_[3] = 0.0f;
		m_[4] = cosTheta;
		m_[5] = sinTheta;
		m_[6] = 0.0f;
		m_[7] = -sinTheta;
		m_[8] = cosTheta;
	}
	/*
	*/
	void sMatrix3::rotationY(float radian)
	{
		float sinTheta = cMath::sinf(radian);
		float cosTheta = cMath::cosf(radian);

		m_[0] = cosTheta;
		m_[1] = 0.0f;
		m_[2] = -sinTheta;
		m_[3] = 0.0f;
		m_[4] = 1.0f;
		m_[5] = 0.0f;
		m_[6] = sinTheta;
		m_[7] = 0.0f;
		m_[8] = cosTheta;
	}
	/*
	*/
	void sMatrix3::rotationZ(float radian)
	{
		float sinTheta = cMath::sinf(radian);
		float cosTheta = cMath::cosf(radian);

		m_[0] = cosTheta;
		m_[1] = sinTheta;
		m_[2] = 0.0f;
		m_[3] = -sinTheta;
		m_[4] = cosTheta;
		m_[5] = 0.0f;
		m_[6] = 0.0f;
		m_[7] = 0.0f;
		m_[8] = 1.0f;
	}
}





















