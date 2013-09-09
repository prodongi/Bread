
#include "BreadFreeType.h"
#include "BreadFreeTypeFontCash.h"
#include "BreadFreeTypeGlyphCash.h"

namespace Bread
{
	/*
	*/
	cFreeType::cFreeType() : m_ftGlyphCash(NULL), m_ftFontCash(NULL)
	{
	}
	/*
	*/
	cFreeType::~cFreeType()
	{
	}
	/*
	*/
	bool cFreeType::initialize()
	{
		FT_Error error = FT_Init_FreeType(&m_library);
		if (error)
			return false;

		assert(!m_ftGlyphCash);
		assert(!m_ftFontCash);

		m_ftGlyphCash = new cFreeTypeGlyphCash;
		m_ftFontCash = new cFreeTypeFontCash;

		return true;
	}
	/*
	*/
	void cFreeType::finalize()
	{
		safeDelete(m_ftGlyphCash);
		safeDelete(m_ftFontCash);
		FT_Done_FreeType(m_library);
	}
	/*
	*/
	bool cFreeType::loadFont(std_string const& filename)
	{
		return m_ftFontCash->loadFont(filename, m_library);
	}
	/*
	*/
	bool cFreeType::loadFont(std_string const& fontname, byte const* data, long size)
	{
		return m_ftFontCash->loadFont(fontname, data, size, m_library);
	}
	/*
	*/
	void cFreeType::makeLine(sFreeTypeFontDesc const& desc, std_string const& str, cFreeTypeLine& line)
	{
		cFreeTypeFont* font = m_ftFontCash->get(desc.m_name);
		if (!font)
		{
			assert(0 && _T("failed find font"));
			return ;
		}
		font->setSize(desc.m_size);

		line.setStr(str);

		TCHAR const* c_str = str.c_str();
		for (size_t i = 0; i < str.length(); ++i)
		{
			line.addGlyph(m_ftGlyphCash->getKey(desc, font->getFace(), c_str[i]));
		}
	}
	/*
	*/
	cFreeTypeGlyph const* cFreeType::getGlyph(BUID const& buid) const
	{
		return m_ftGlyphCash->get(buid);
	}
}