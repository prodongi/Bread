
#include "BreadFreeTypeFont.h"
#include "BreadGui.h"
#include "BreadStringHelper.h"
#include "BreadCore.h"
#include FT_TRUETYPE_IDS_H

namespace Bread
{
	/*
	*/
	cFreeTypeFont::cFreeTypeFont() : m_face(0)
	{
	}
	/*
	*/
	cFreeTypeFont::~cFreeTypeFont()
	{
		if (m_face)
			FT_Done_Face(m_face);
	}
	/*
	*/
	bool cFreeTypeFont::loadFont(std_string const& filename, FT_Library const& ftLib)
	{
		char _filename[128];
		_getStringHelper()->w2m(filename.c_str(), _filename, 128, _getInputSystem()->getCodePage());

		FT_Error error = FT_New_Face(ftLib, _filename, 0, &m_face);
		if (error)
		{
			assert(0 && _T("Failed FT_New_Face"));
			return false;
		}

		return true;
	}
	/*
	*/
	bool cFreeTypeFont::loadFont(byte const* data, long size, FT_Library const& ftLib)
	{
		FT_Error error = FT_New_Memory_Face(ftLib, data, size, 0, &m_face);
		if (error)
			return false;

		return true;
	}
	/*
	*/
	void cFreeTypeFont::setSize(long size)
	{
		//FT_Set_Char_Size(m_face, size << 6, 0, 0, 0);
		FT_Set_Pixel_Sizes(m_face, size, 0);
	}
}