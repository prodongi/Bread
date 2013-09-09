
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
		/// @brief ȣ��Ǹ� update()���� realTime�� �� �����Ӹ�ŭ �����Ѵ�, puase������ ���� ����ȴ�.
		void setOneStep()					{	if (m_pause)	m_oneStep = true;		}
		propertyFunc(bool, Pause, m_pause)
		propertyFunc(double, TimeScale, m_timeScale)

	private:
		/// @brief QueryPerformanceFrequency �Լ��� ��� �������� �ƴ���
		bool isUsingQueryPerformance();
		void createClock();
		double getCounter();
		double getFrequency();

	private:
		double m_frequencyPerSecond;
		double m_secondPerFrequency;
		double m_realTime;
		float m_elapsedTime;
		float m_elapsedTotalTime;	/// @brief ������ m_elapsedTime�� ��ü �ð�(m_realTime��� float�� ������ �� �� ����)
		double m_timeScale;
		/// @brief �� ������ ���� ���� ���� ��� �ð�
		float m_oneStepElapsedTime;
		bool m_pause;
		bool m_oneStep;
		sClock* m_clock;
	};
}



#endif