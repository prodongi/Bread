
#ifndef _BreadConsole_h_
#define _BreadConsole_h_

#include "BreadWindow.h"

namespace Bread
{
	/**
		@date 2010.07.05
		@auth prodongi
		@desc command prompt control
	*/
	class cConsole : public cWindow
	{
	public:
		cConsole();
		virtual ~cConsole();
		virtual void initialize(float x, float y, float w, float h);
		virtual void finalize() {}

	protected:
		bool m_active;
	};
}










#endif