
/*
	BreadLuminanceMapper.cpp
	Copyright (c) prodongi. All rights resserved.
*/

#include "stdafx.h"
#include "BreadLuminanceMapper.h"

namespace Bread
{
	float cLuminanceMapper::Ldmax = 100.0f;

	float cLuminanceMapper::LsavgC = 100.0f;
	float cLuminanceMapper::LsavgR = 100.0f;
	float cLuminanceMapper::k = 1.0f;
	float cLuminanceMapper::mR = 1.0f;
	float cLuminanceMapper::mC = 1.0f;

	void cLuminanceMapper::SetMaxDisplayLuminance(float nits)
	{
		if (nits > 0)
		{
			Ldmax = nits;
		}
	}

	void cLuminanceMapper::SetSceneLogAvg(float rodNits, float coneNits)
	{
		LsavgR = rodNits;
		LsavgC = coneNits;
		ComputeScaleFactors();
	}

	float cLuminanceMapper::GetBurnoutLuminance()
	{
		return Ldmax / (mC + k * mR);
	}

	void cLuminanceMapper::DurandMapperXYZ(sVector3 *XYZ)
	{
		float R = (XYZ->x) * -0.702f + (XYZ->y) * 1.039f + (XYZ->z) * 0.433f;

		if (R < 0) R = 0;

		sVector3 scotopic(R * 0.3f, R * 0.3f, R * 0.4f);

		*XYZ = ((*XYZ) * (1.0f - k) * mC + scotopic * (k * mR)) * (1.0f / Ldmax);
	}

	void cLuminanceMapper::DurandMapper(float *x, float *y, float *Y)
	{
		if ((*y) > 0)
		{
			// From Durand
			const float xBlue = 0.3f;
			const float yBlue = 0.3f;

			float X = (*x) * ((*Y) / (*y));
			float Z = (1.0f - (*x) - (*y)) * ((*Y) / (*y));
			float R = X * -.702f + (*Y) * 1.039f + Z * 0.433f;

			if (R < 0) R = 0;

			// Straight Ferwerda tone mapping - gets us to normalized luminance at estimated
			// display adaptation luminance
			float Ldp = (*Y) * mC;
			float Lds = (R) * mR;

			*Y = Ldp + k * Lds;

			*Y /= Ldmax;

			// Durand's blue-shift for scotopic vision
			*x = (*x) * (1-k) + k * xBlue;
			*y = (*y) * (1-k) + k * yBlue;
		}
	}

	static float RodThreshold(float LaR)
	{
		float logLaR = log(LaR);

		float logEpsR;

		if (logLaR <= -3.94f)
		{
			logEpsR = -2.86f;
		}
		else if (logLaR >= -1.44f)
		{
			logEpsR = logLaR - 0.395f;
		}
		else
		{
			logEpsR = pow(0.405f * logLaR + 1.6f, 2.18f) - 2.86f;
		}

		return exp(logEpsR);
	}

	static float ConeThreshold(float LaC)
	{
		float logLaC = log(LaC);
		float logEpsC;

		if (logLaC <= -2.6f)
		{
			logEpsC = -0.72f;
		}
		else if (logLaC >= 1.9f)
		{
			logEpsC = logLaC - 1.255f;
		}
		else
		{
			logEpsC = pow(0.249f * logLaC + 0.65f, 2.7f) - 0.72f;
		}

		return exp(logEpsC);
	}

	void cLuminanceMapper::ComputeScaleFactors()
	{
		/** Ferwerda operator **/
		float LdaC, LwaR, LwaC;

		static bool first = true;
		if ( first )
		{
			first = false;
		}
		LdaC = Ldmax;

		LwaR = LsavgR;
		LwaC = LsavgC;

		float displayThreshold = ConeThreshold(LdaC);

		mR = displayThreshold / RodThreshold(LwaR);
		mC = displayThreshold / ConeThreshold(LwaC);

		const float sigma = 100.0;
		k = (sigma - 0.25f * LwaR) / (sigma + LwaR);
		if (k < 0) k = 0;
	}
}