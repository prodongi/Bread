
#ifndef _BreadEditBox_h_
#define _BreadEditBox_h_

#include "BreadWindow.h"

namespace Bread
{
	class cEditBox : public cWindow
	{
	public:
		cEditBox();
		virtual ~cEditBox();

		virtual void update(float elapsedtime, int x, int y);
		virtual void enterFocus();

	protected:
	};
}





#endif