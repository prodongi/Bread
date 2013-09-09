
#ifndef _BreadFontManager_h_
#define _BreadFontManager_h_

#include "BreadD3DHeader.h"
#include "BreadD3DFont.h"
#include "BreadFreeType.h"

namespace Bread
{
	class cFreeTypeGlyph;

	/*
		@date 2010.08.29
		@auth prodongi
		@desc font manager
		@todo 
	*/
	class cFontManager
	{
	public:
		cFontManager();
		virtual ~cFontManager();
		virtual bool initialize(LPDIRECT3DDEVICE9 device);
		virtual void finalize();

		void setDefaultFont(std_string const& name)	{	m_defaultFont = name;	}
		cFreeTypeGlyph const* getFreeTypeGlyph(BUID const& buid) const;
		bool loadFreeTypeFont(std_string const& filename);
		void makeFreeTypeLine(sFreeTypeFontDesc const& desc, std_string const& str, cFreeTypeLine& line);
		void drawD3DText(int x, int y, TCHAR const* str, int height = cD3DFont::HEIGHT_14, D3DXCOLOR const& color = D3DX_WHITE, int weight = cD3DFont::WEIGHT_NORMAL);

	protected:
		cD3DFont*  m_d3dFont;
		cFreeType* m_freeType;
		std_string m_defaultFont;
	};
}



#endif