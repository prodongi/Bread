
#include "BreadFreeTypeFontCash.h"
#include "BreadStringHelper.h"

namespace Bread
{
	/*
	*/
	bool cFreeTypeFontCash::loadFont(std_string const& filename, FT_Library const& ftLib)
	{
		BUID key = _getStringHelper()->makeFilenameToNotExp(filename);
		cFreeTypeFont* font = create(key);
		if (!font)
			return false;
		if (!font->loadFont(filename, ftLib))
		{
			assert(0 && _T("Failed load font"));
			safeDelete(font);
			return false;
		}
		insert(key, &font);
		return true;
	}
	/*
	*/
	bool cFreeTypeFontCash::loadFont(BUID const& key, byte const* data, long size, FT_Library const& ftLib)
	{
		cFreeTypeFont* font = create(key);
		if (!font)
			return false;
		if (!font->loadFont(data, size, ftLib))
		{
			safeDelete(font);
			return false;
		}
		insert(key, &font);
		return true;
	}
}