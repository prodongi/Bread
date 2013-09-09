
#ifndef _BreadStaticText_h_
#define _BreadStaticText_h

#include "BreadWindow.h"
#include "BreadUIFreeTypeText.h"

namespace Bread
{
	class cStaticText : public cWindow
	{
	public:
		cStaticText();
		virtual ~cStaticText();

		virtual void render(LPDIRECT3DDEVICE9 device);

		void setText(sFreeTypeFontDesc const& desc, std_string const& str);
		void setTextPos(float x, float y);

	protected:
		cUIFreeTypeText m_ftText;
	};
}





#endif