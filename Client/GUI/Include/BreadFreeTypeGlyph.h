
#ifndef _BreadFreeTypeGlyph_h_
#define _BreadFreeTypeGlyph_h_

#include "BreadFreeTypeFont.h"

namespace Bread
{
	struct sFreeTypeGlyphDesc
	{
		sFreeTypeGlyphDesc() : m_tex(NULL) {}
		~sFreeTypeGlyphDesc()
		{
			safeRelease(m_tex);
		}
		int m_offsetX;
		int m_offsetY;
		int m_spaceX;
		int m_spaceY;
		float m_texWidth;
		float m_texHeight;

		LPDIRECT3DTEXTURE9 m_tex;

	};
	/*
		@date 2010.09.03
		@auth prodongi
		@desc 
		@todo 
	*/
	class cFreeTypeGlyph
	{
	public:
		cFreeTypeGlyph() {}
		~cFreeTypeGlyph() {}
		bool load(FT_Face& face, TCHAR c);
		sFreeTypeGlyphDesc const* getDesc() const	{	return &m_desc;	}

	private:
		sFreeTypeGlyphDesc m_desc;
	};
}



#endif