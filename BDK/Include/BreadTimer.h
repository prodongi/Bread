
#ifndef _BreadTimer_h_
#define _BreadTimer_h_

#include "BreadInline.h"
#include "BreadDefinition.h"
#include <assert.h>

#define _getTimer() Bread::cTimer::getInstance()

namespace Bread
{
	struct sClock
	{
		virtual double getCounter() = 0;
		virtual double getFrequency() = 0;
	};
	struct sClockQueryPerformance : public sClock
	{
		virtual double getCounter();
		virtual double getFrequency();
	};
	struct sClockStamp : public sClock
	{
		virtual double getCounter();
		virtual double getFrequency();
	};
	/*
		@date 2011.07.11
		@auth prodongi
		@desc 
		@todo 
	*/
	class cTimer
	{
		declareInstance(cTimer)

	public:
		cTimer();
		~cTimer();

		void initialize();
		void finalize();
		void update();
		float getElapsedTime() const		{	return m_elapsedTime;		}
		float getElapsedTotalTime() const	{	return m_elapsedTotalTime;	}
		double getRealTime();
		/// @brief 호출되면 update()에서 realTime이 한 프레임만큼 증가한다, puase상태일 때만 적용된다.
		void setOneStep()					{	if (m_pause)	m_oneStep = true;		}
		propertyFunc(bool, Pause, m_pause)
		propertyFunc(double, TimeScale, m_timeScale)

	private:
		/// @brief QueryPerformanceFrequency 함수를 사용 가능한지 아닌지
		bool isUsingQueryPerformance();
		void createClock();
		double getCounter();
		double getFrequency();

	private:
		double m_frequencyPerSecond;
		double m_secondPerFrequency;
		double m_realTime;
		float m_elapsedTime;
		float m_elapsedTotalTime;	/// @brief 축적된 m_elapsedTime의 전체 시간(m_realTime대신 float을 쓸려고 할 때 쓰임)
		double m_timeScale;
		/// @brief 한 프레임 증가 했을 때의 경과 시간
		float m_oneStepElapsedTime;
		bool m_pause;
		bool m_oneStep;
		sClock* m_clock;
	};
}



#endif