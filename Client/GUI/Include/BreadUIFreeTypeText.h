
#ifndef _BreadUIFreeTypeText_h_
#define _BreadUIFreeTypeText_h_

#include "BreadFreeType.h"
#include "BreadUiPrimitive.h"

namespace Bread
{
	/*
		@date 2010.09.06
		@auth prodongi
		@desc 
		@todo 
	*/
	class cUIFreeTypeText
	{
	public:
		cUIFreeTypeText();
		~cUIFreeTypeText();

		void setText(sFreeTypeFontDesc const& desc, std_string const& str);
		void setPrimitive(float x, float y);
		void render(LPDIRECT3DDEVICE9 device);

	private:
		cFreeTypeLine m_ftLine;
		std::vector<sUiPlane> m_pvList;
	};
}



#endif