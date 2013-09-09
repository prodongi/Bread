
#ifndef _BreadFreeType_h_
#define _BreadFreeType_h_

#include "BreadFreeTypeFont.h"
#include "BreadFreeTypeLine.h"

namespace Bread
{
	class cFreeTypeGlyphCash;
	class cFreeTypeFontCash;
	class cFreeTypeGlyph;

	/*
		@date 2010.07.05
		@auth prodongi
		@desc ime 국가별 설정
		@todo zupet님의 오픈 소스에서 참조
	*/
	class cFreeType
	{
	public:
		cFreeType();
		virtual ~cFreeType();
		bool initialize();
		void finalize();
		bool loadFont(std_string const& filename);
		bool loadFont(std_string const& fontname, byte const* data, long size);
		void makeLine(sFreeTypeFontDesc const& desc, std_string const& str, cFreeTypeLine& line);
		cFreeTypeGlyph const* getGlyph(BUID const& buid) const;

	private:
		FT_Library m_library;
		cFreeTypeGlyphCash* m_ftGlyphCash;
		cFreeTypeFontCash* m_ftFontCash;
	};
}



#endif