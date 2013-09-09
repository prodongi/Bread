
#include "BreadMatrix4.h"

namespace Bread
{
	/*
	 *	
	 */
	const sMatrix4 sMatrix4::IDENTITY = sMatrix4(1.0f, 0.0f, 0.0f, 0.0f,
												 0.0f, 1.0f, 0.0f, 0.0f,
												 0.0f, 0.0f, 1.0f, 0.0f,
												 0.0f, 0.0f, 0.0f, 1.0f);
	/*
	 *	
	 */
	void sMatrix4::makePerspectiveMatrix(float fovY, float aspect, float zNear, float zFar)
	{
		float h = 1.0f/tanf(fovY/2.0f);
		float w = h / aspect;
		float q = zFar/(zFar-zNear);

		m[0][0] = w;		m[0][1] = 0.0f;	m[0][2] = 0.0f; 	m[0][3] = 0.0f;
		m[1][0] = 0.0f;	m[1][1] = h; 		m[1][2] = 0.0f; 	m[1][3] = 0.0f;
		m[2][0] = 0.0f;	m[2][1] = 0.0f;	m[2][2] = q; 		m[2][3] = 1.0f;
		m[3][0] = 0.0f;	m[3][1] = 0.0f;	m[3][2] = -zNear*q;	m[3][3] = 0.0f;
	}
	/*
	*/
	void sMatrix4::makeOrthoMatrix(float w, float h, float zNear, float zFar)
	{
		identity();
		m[0][0] = 2.0f/w;
		m[1][1] = 2.0f/h;
		m[2][2] = 1.0f/(zFar - zNear);
		m[3][2] = zNear/(zNear - zFar);
	}
	/*
	*/
	void sMatrix4::rotationX(float radian)
	{
		float sinTheta = cMath::sinf(radian);
		float cosTheta = cMath::cosf(radian);

		m_[0] = 1.0f;
		m_[1] = 0.0f;
		m_[2] = 0.0f;
		m_[3] = 0.0f;
		m_[4] = 0.0f;
		m_[5] = cosTheta;
		m_[6] = sinTheta;
		m_[7] = 0.0f;
		m_[8] = 0.0f;
		m_[9] = -sinTheta;
		m_[10] = cosTheta;
		m_[11] = 0.0f;
		m_[12] = 0.0f;
		m_[13] = 0.0f;
		m_[14] = 0.0f;
		m_[15] = 1.0f;
	}
	/*
	*/
	void sMatrix4::rotationY(float radian)
	{
		float sinTheta = cMath::sinf(radian);
		float cosTheta = cMath::cosf(radian);

		m_[0] = cosTheta;
		m_[1] = 0.0f;
		m_[2] = -sinTheta;
		m_[3] = 0.0f;
		m_[4] = 0.0f;
		m_[5] = 1.0f;
		m_[6] = 0.0f;
		m_[7] = 0.0f;
		m_[8] = sinTheta;
		m_[9] = 0.0f;
		m_[10] = cosTheta;
		m_[11] = 0.0f;
		m_[12] = 0.0f;
		m_[13] = 0.0f;
		m_[14] = 0.0f;
		m_[15] = 1.0f;
	}
	/*
	*/
	void sMatrix4::rotationZ(float radian)
	{
		float sinTheta = cMath::sinf(radian);
		float cosTheta = cMath::cosf(radian);

		m_[0] = cosTheta;
		m_[1] = sinTheta;
		m_[2] = 0.0f;
		m_[3] = 0.0f;
		m_[4] = -sinTheta;
		m_[5] = cosTheta;
		m_[6] = 0.0f;
		m_[7] = 0.0f;
		m_[8] = 0.0f;
		m_[9] = 0.0f;
		m_[10] = 1.0f;
		m_[11] = 0.0f;
		m_[12] = 0.0f;
		m_[13] = 0.0f;
		m_[14] = 0.0f;
		m_[15] = 1.0f;
	}
}