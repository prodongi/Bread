
#include "BreadFontManager.h"
#include "BreadDeclaration.h"
#include "BreadFreeTypeGlyph.h"

namespace Bread
{
	/*
	*/
	cFontManager::cFontManager() : m_d3dFont(NULL), m_freeType(NULL)
	{
	}
	/*
	*/
	cFontManager::~cFontManager()
	{
	}
	/*
	*/
	bool cFontManager::initialize(LPDIRECT3DDEVICE9 device)
	{
		assert(!m_d3dFont);
		assert(!m_freeType);

		m_d3dFont = new cD3DFont;
		m_d3dFont->createDefaultFont(device, _T("Arial"));

		m_freeType = new cFreeType;
		m_freeType->initialize();

		return true;
	}
	/*
	*/
	void cFontManager::finalize()
	{
		safeDelete(m_d3dFont);
		safeDelete(m_freeType, true);
	}
	/*
	*/
	void cFontManager::drawD3DText(int x, int y, TCHAR const* str, int height, D3DXCOLOR const& color, int weight)
	{
		assert(m_d3dFont);
		m_d3dFont->drawText(x, y, str, height, color, weight);
	}
	/*
	*/
	bool cFontManager::loadFreeTypeFont(std_string const& filename)
	{
		return m_freeType->loadFont(filename);
	}
	/*
	*/
	void cFontManager::makeFreeTypeLine(sFreeTypeFontDesc const& desc, std_string const& str, cFreeTypeLine& line)
	{
		// check default font name
		sFreeTypeFontDesc _desc;
		_desc.m_size = desc.m_size;
		if (0 == desc.m_name.compare(_T("default")))
			_desc.m_name = m_defaultFont;
		else
			_desc.m_name = desc.m_name;

		m_freeType->makeLine(_desc, str, line);
	}
	/*
	*/
	cFreeTypeGlyph const* cFontManager::getFreeTypeGlyph(BUID const& buid) const
	{
		return m_freeType->getGlyph(buid);
	}
}