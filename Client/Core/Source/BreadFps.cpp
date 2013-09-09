
#include "BreadFps.h"
#include "BreadTimer.h"

#include <windows.h>

namespace Bread
{
	/*
	 *	
	 */
	void cFps::update()
	{
		static double lastTime = 0.0f;
		static DWORD frames = 0;
		double _time = _getTimer()->getRealTime();
		++frames;

		if (_time - lastTime > 1.0f)
		{
			m_fps	 = (float)((double)frames / (_time - lastTime));
			lastTime = _time;
			frames	 = 0;
		}
	}
}