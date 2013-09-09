
#include "BreadD3DFont.h"
#include "BreadDeclaration.h"


namespace Bread
{
	/*
	 *	
	 */
	cD3DFont::cD3DFont()
	{
		for (int w = 0; w < WEIGHT_NUM; ++w)
		{
			for (int h = 0; h < HEIGHT_NUM; ++h)
			{
				m_defaultFont[h][w] = NULL;
			}
		}
	}
	/*
	 *	
	 */
	cD3DFont::~cD3DFont()
	{
		for (int w = 0; w < WEIGHT_NUM; ++w)
		{
			for (int h = 0; h < HEIGHT_NUM; ++h)
			{
				safeRelease(m_defaultFont[h][w]);
			}
		}
	}
	/*
	 *	
	 */
	HRESULT cD3DFont::createDefaultFont(LPDIRECT3DDEVICE9 device, TCHAR const*  fontName)
	{
		HRESULT hr;
		for (int w = 0; w < WEIGHT_NUM; ++w)
		{
			int weight;
			if (WEIGHT_NORMAL == w)		weight = FW_NORMAL;
			else if (WEIGHT_BOLD == w)	weight = FW_BOLD;
			else						weight = FW_NORMAL;
			for (int h = 0; h < HEIGHT_NUM; ++h)
			{
				if (FAILED(hr = D3DXCreateFont(device, h + 8, 0, weight, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
						DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, fontName, &m_defaultFont[h][w])))
						return hr;
			}
		}
		return S_OK;
	}
	/*
	 *	
	 */
	void cD3DFont::drawText(int x, int y, TCHAR const* str, int height /* = HEIGHT_12 */, D3DXCOLOR const& color /* = D3DXCOLOR */, int weight /* = WEIGHT_THIN */)
	{
		if (m_defaultFont[height][weight])
		{
			RECT rc;
			SetRect(&rc, x, y, 0, 0);
			m_defaultFont[height][weight]->DrawText(NULL, str, -1, &rc, DT_NOCLIP, color);
		}
	}
}
















