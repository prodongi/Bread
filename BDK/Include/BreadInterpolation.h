
#ifndef _BreadInterpolation_h_
#define _BreadInterpolation_h_

#include "BreadMath.h"

namespace Bread
{
	/*
		@date 2012.
		@auth prodongi
		@desc 선형 보간
		@todo 
	*/
	template <typename T>
	class cBaseInterpolation
	{
	public:
		virtual bool initialize(T const& from, T const& to, float totalTime)
		{
			if (cMath::isZero(totalTime))
				return false;

			m_from = from;
			m_to = to;
			m_interpolation = m_from;
			m_remainTime = totalTime;
			m_normal = to - from;
			m_normal.normalize();

			return true;
		}
		virtual bool update(float /*elapsedTime*/) {}
		void getInterpolationValue(T& value)
		{
			value = m_interpolation;
		}
	protected:
		T m_to;
		T m_from;
		T m_interpolation;
		T m_normal;
		float m_remainTime;
		float m_v;
	};

	template <typename T>
	class cLinearInterpolation : public cBaseInterpolation<T>
	{
	public:
		bool initlaize(T const& from, T const& to, float totalTime)
		{
			if (!cBaseInterpolation::initialize(from, to, totalTime))
				return false;

			T n = to - from;
			m_v = n.length()/totalTime;

			return true;
		}
		bool update(float elapsedTime)
		{
			m_remainTime -= elapsedTime;
			float s = m_v * elapsedTime;
			m_interpolation += m_normal * s;

			return m_remainTime <= 0.0f;
		};
	};
	/*	@brief 선형 가속 감속 보간
	*/
	template <typename T>
	class cLinearAccelInterpolation : public cBaseInterpolation<T>
	{
	public:
		bool initialize(T const& from, T const& to, float totalTime)
		{
			if (!cBaseInterpolation::initialize(from, to, totalTime))
				return false;

			m_v = 0.0f;
			T n = to - from;
			m_accel = (2.0f * n.length())/(totalTime * totalTime);
			m_totalHalfTime = totalTime * 0.5f;

			return true;
		}
		bool update(float elapsedTime)
		{
			m_remainTime -= elapsedTime;
			float v = m_accel * elapsedTime;
			if (m_remainTime < m_totalHalfTime)
			{
				m_v += v;
			}
			else
			{
				m_v -= v;
			}

			float s = m_v * elapsedTime;
			m_interpolation += m_normal * s;

			return m_remainTime <= 0.0f;
		}

	private:
		float m_totalHalfTime;
		float m_accel;
	};
}



#endif