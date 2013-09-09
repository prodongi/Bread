
#ifndef _BreadD3DFont_h_
#define _BreadD3DFont_h_

#include "BreadD3DHeader.h"
#include "BreadD3DDeclaration.h"

namespace Bread
{
	/*
	 *	
	 */
	class cD3DFont
	{
	public:
		cD3DFont();
		~cD3DFont();

		HRESULT createDefaultFont(LPDIRECT3DDEVICE9 device, TCHAR const* fontName);
		void drawText(int x, int y, TCHAR const* str, int height = HEIGHT_14, D3DXCOLOR const& color = D3DX_WHITE, int weight = WEIGHT_NORMAL);

	public:
		enum
		{
			HEIGHT_8, HEIGHT_9, HEIGHT_10, HEIGHT_11, HEIGHT_12, HEIGHT_13, HEIGHT_14, HEIGHT_15, HEIGHT_16, HEIGHT_17, HEIGHT_18, HEIGHT_19, HEIGHT_20, HEIGHT_NUM 
		};
		enum
		{
			WEIGHT_NORMAL, WEIGHT_BOLD, WEIGHT_NUM
		};

	private:
		ID3DXFont*	m_defaultFont[HEIGHT_NUM][WEIGHT_NUM];
	};
}








#endif