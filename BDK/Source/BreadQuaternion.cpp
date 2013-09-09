
#include "BreadQuaternion.h"
#include "BreadMath.h"


namespace Bread
{
	const sQuaternion sQuaternion::ZERO(0.0f, 0.0f, 0.0f, 0.0f);
	const sQuaternion sQuaternion::IDENTITY(0.0f, 0.0f, 0.0f, 1.0f);

	/**
	*	
	*/
	void sQuaternion::makeFromRotMatrix(const sMatrix3& rot)
	{
		float trace = rot.trace();
		float root;

		if (trace > 0.0f)
		{
			root = cMath::sqrtf(trace + 1.0f);

			w = 0.5f * root;
			x = (rot[2][1]-rot[1][2])*root;
			y = (rot[0][2]-rot[2][0])*root;
			z = (rot[1][0]-rot[0][1])*root;
		}
		else
		{
			// |w| <= 1/2
			static int s_iNext[3] = { 1, 2, 0 };
			int i = 0;
			if ( rot[1][1] > rot[0][0] )
				i = 1;
			if ( rot[2][2] > rot[i][i] )
				i = 2;
			int j = s_iNext[i];
			int k = s_iNext[j];

			root = cMath::sqrtf(rot[i][i]-rot[j][j]-rot[k][k] + 1.0f);
			float* apkQuat[3] = { &x, &y, &z };
			*apkQuat[i] = 0.5f*root;
			root = 0.5f/root;
			w = (rot[k][j]-rot[j][k])*root;
			*apkQuat[j] = (rot[j][i]+rot[i][j])*root;
			*apkQuat[k] = (rot[k][i]+rot[i][k])*root;
		}
	}
	/*
	 *	
	 */
	void sQuaternion::set(sVector3 const& axis, float theta)
	{
		float theta_h = theta * 0.5f;
		float _sin = cMath::sinf(theta_h);
		x = axis.x * _sin;
		y = axis.y * _sin;
		z = axis.z * _sin;
		w = cMath::cosf(theta_h);
	}
	/*
	 *	
	 */
	void sQuaternion::get(sVector3& axis, float& theta)
	{
		theta = cMath::acosf(w);
		float sin_ih = cMath::inv(cMath::sinf(theta));
		axis.x = x * sin_ih;
		axis.y = y * sin_ih;
		axis.z = z * sin_ih;
		theta *= 2.0f;
	}
	/*
	 *	
	 */
	void sQuaternion::fromEuler(float pitch, float yaw, float roll)
	{
		float cos_zh = 0.5f * cMath::cosf(roll);
		float cos_yh = 0.5f * cMath::cosf(yaw);
		float cos_xh = 0.5f * cMath::cosf(pitch);

		float sin_zh = 0.5f * cMath::sinf(roll);
		float sin_yh = 0.5f * cMath::sinf(yaw);
		float sin_xh = 0.5f * cMath::sinf(pitch);

		w = cos_zh * cos_yh * cos_xh + sin_zh * sin_yh * sin_xh;
		x = cos_zh * cos_yh * sin_xh + sin_zh * sin_yh * cos_xh;
		y = cos_zh * sin_yh * cos_xh + sin_zh * cos_yh * sin_xh;
		z = sin_zh * cos_yh * cos_xh + cos_zh * sin_yh * sin_xh;
	}
}