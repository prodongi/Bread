
#include "BreadFreeTypeGlyphCash.h"

namespace Bread
{
	/*
	*/
	BUID cFreeTypeGlyphCash::getKey(sFreeTypeFontDesc const& desc, FT_Face& face, TCHAR c)
	{
		BUID key;
		makeKey(desc, c, key);
		if (!is(key))
		{
			cFreeTypeGlyph* glyph = create();
			glyph->load(face, c);
			insert(key, &glyph);
		}
		return key;
	}
	/*
	*/
	void cFreeTypeGlyphCash::makeKey(sFreeTypeFontDesc const& desc, TCHAR c, BUID& key)
	{
		TCHAR temp[32];
		_stprintf(temp, _T("%d"), desc.m_size);

		key += desc.m_name;
		key += temp;
		key.push_back(c);
	}
}