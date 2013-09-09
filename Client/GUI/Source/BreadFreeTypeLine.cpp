
#include "BreadFreeTypeLine.h"

namespace Bread
{
	/*
	*/
	cFreeTypeLine::~cFreeTypeLine()
	{
		m_glyphList.clear();
	}
	/*
	*/
	void cFreeTypeLine::setStr(std_string const& str)
	{
		m_str = str;
	}
	/*
	*/
	void cFreeTypeLine::addGlyph(BUID const& key)
	{
		m_glyphList.push_back(key);
	}
	/*
	*/
	BUID const& cFreeTypeLine::getGlyph(int index)
	{
		assert(index < (int)getGlyphNum());
		return m_glyphList[index];
	}
}