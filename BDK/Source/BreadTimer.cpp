
#include <windows.h>
#include "BreadTimer.h"

namespace Bread
{
	implementInstance(cTimer)

	/*
	*/
	double sClockQueryPerformance::getCounter()
	{
		LARGE_INTEGER count;
		QueryPerformanceCounter(&count);
		return (double)count.QuadPart;		
	}
	/*
	*/
	double sClockQueryPerformance::getFrequency()
	{
		LARGE_INTEGER freq;
		QueryPerformanceFrequency(&freq);
		return (double)freq.QuadPart;
	}
	/*
	*/
	double sClockStamp::getCounter()
	{
		return (double)timeGetTime()/1000.0f;
	}
	/*
	*/
	double sClockStamp::getFrequency()
	{
		return 1.0f;
	}

	/*
	*/
	cTimer::cTimer() : m_clock(NULL), 
					   m_realTime(0.0f), 
					   m_elapsedTime(0.0f),
					   m_elapsedTotalTime(0.0f),
					   m_pause(false), 
					   m_oneStep(false),
					   m_timeScale(1.0f)
	{
		assignInstance(this)
	}
	/*
	*/
	cTimer::~cTimer()
	{
		assignInstance(NULL)
	}
	/*
	*/
	void cTimer::initialize()
	{
		createClock();
		m_frequencyPerSecond = getFrequency();
		m_secondPerFrequency = 1.0f/m_frequencyPerSecond;
		m_oneStepElapsedTime = 1.0f/30.0f;
	}
	/*
	*/
	void cTimer::finalize()
	{
		safeDelete(m_clock);
	}
	/*
	*/
	void cTimer::createClock()
	{
		if (isUsingQueryPerformance())
			m_clock = new sClockQueryPerformance;
		else
			m_clock = new sClockStamp;
	}
	/*
	*/
	void cTimer::update()
	{
		double oldRealTime = m_realTime * m_timeScale;

		if (!m_pause)
		{
			/// real time
			m_realTime = getRealTime();
		}
		else
		{
			if (m_oneStep)
			{
				m_realTime += m_oneStepElapsedTime * m_timeScale;
				m_oneStep = false;
			}
		}

		/// elapsed time
		m_elapsedTime = (float)(m_realTime - oldRealTime);
		/// m_elapsedTime이 1초를 경과 했을 경우에는 디버깅 중인걸로 간주해서 유효한 값으로 설정한다
		if (m_elapsedTime > 1.0f)
		{
			m_elapsedTime = m_oneStepElapsedTime;
		}

		//
		if (FLT_MAX <= m_elapsedTotalTime + m_elapsedTime)
		{
			m_elapsedTotalTime = m_elapsedTime - (FLT_MAX - m_elapsedTotalTime);
		}
		else
		{
			m_elapsedTotalTime += m_elapsedTime;
		}
		if (FLT_MAX <= m_elapsedTotalTime)
			m_elapsedTotalTime -= FLT_MAX;
	}
	/*
	*/
	bool cTimer::isUsingQueryPerformance()
	{
		LARGE_INTEGER freq;
		BOOL ret = QueryPerformanceFrequency(&freq);
		return ret == TRUE;
	}
	/*
	*/
	double cTimer::getFrequency()
	{
		return m_clock->getFrequency();
	}
	/*
	*/
	double cTimer::getCounter()
	{
		return m_clock->getCounter();
	}
	/*
	*/
	double cTimer::getRealTime()
	{
		return getCounter() * m_secondPerFrequency * m_timeScale;
	};
}