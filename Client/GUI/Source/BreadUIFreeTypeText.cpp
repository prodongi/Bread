
#include "BreadUIFreeTypeText.h"
#include "BreadCore.h"
#include "BreadGui.h"
#include "BreadFreeTypeGlyph.h"

namespace Bread
{
	/*
	*/
	cUIFreeTypeText::cUIFreeTypeText()
	{
	}
	/*
	*/
	cUIFreeTypeText::~cUIFreeTypeText()
	{
		m_pvList.clear();
	}
	/*
	*/
	void cUIFreeTypeText::setText(sFreeTypeFontDesc const& desc, std_string const& str)
	{
		_getFontMgr()->makeFreeTypeLine(desc, str, m_ftLine);

		m_pvList.clear();
		m_pvList.reserve(m_ftLine.getGlyphNum());
	}
	/*
	*/
	void cUIFreeTypeText::setPrimitive(float x, float y)
	{
		int spaceX = 0;//, spaceY = 0;
		size_t glyphNum = m_ftLine.getGlyphNum();
		for (size_t gn = 0; gn < glyphNum; ++gn)
		{
			cFreeTypeGlyph const* glyph = _getFontMgr()->getFreeTypeGlyph(m_ftLine.getGlyph((int)gn));
			if (!glyph)
				return ;
			sFreeTypeGlyphDesc const* desc = glyph->getDesc();
			sUiPlane plane;
			plane.set(desc->m_offsetX+spaceX+x, desc->m_spaceY - desc->m_offsetY + y, desc->m_texWidth, desc->m_texHeight);
			m_pvList.push_back(plane);

			spaceX += desc->m_spaceX;
			//spaceY += desc->m_spaceY;
		}
	}
	/*
	*/
	void cUIFreeTypeText::render(LPDIRECT3DDEVICE9 device)
	{
		if (m_pvList.empty())
			return ;

		device->SetVertexShader(NULL);
		device->SetPixelShader(NULL);
		device->SetFVF(sUiPlane::D3DFVF);

		size_t glyphNum = m_ftLine.getGlyphNum();
		for (size_t gn = 0; gn < glyphNum; ++gn)
		{
			cFreeTypeGlyph const* glyph = _getFontMgr()->getFreeTypeGlyph(m_ftLine.getGlyph(gn));
			if (!glyph)
				return ;
			sFreeTypeGlyphDesc const* desc = glyph->getDesc();
			device->SetTexture(0, desc->m_tex);
			cRenderer::renderPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, (void const*)m_pvList[gn].m_vertex, sUiPlane::VERTEX_SIZE);
		}
	}
}