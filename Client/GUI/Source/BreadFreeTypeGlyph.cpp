
#include "BreadFreeTypeGlyph.h"
#include "BreadMath.h"
#include "BreadD3DSystemClassify.h"
#include "BreadStringHelper.h"
#include "BreadCore.h"
#include "BreadGui.h"

namespace Bread
{
	/*
	*/
	bool cFreeTypeGlyph::load(FT_Face& face, TCHAR c)
	{
		unsigned int index = FT_Get_Char_Index(face, c);
		if (0 == index)
		{
			assert(0 && _T("FT_Get_Char_Index return is zero"));
			return false;
		}
		FT_Load_Glyph(face, index, FT_LOAD_DEFAULT | FT_LOAD_NO_BITMAP);
		FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

		m_desc.m_offsetX = face->glyph->bitmap_left;
		m_desc.m_offsetY = face->glyph->bitmap_top;
		m_desc.m_spaceX = face->glyph->advance.x >> 6;
		m_desc.m_spaceY = face->glyph->advance.y >> 6;

		uint bmpWidth = face->glyph->bitmap.width;
		uint bmpHeight = face->glyph->bitmap.rows;
		uint tw = cMath::getLargestPow2(bmpWidth);
		uint th = cMath::getLargestPow2(bmpHeight);

		HRESULT hr = cD3DSystem::getD3DDevice()->CreateTexture(tw, th, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_desc.m_tex, NULL);
		if (!checkHResultReturn(hr))
			return false;

		m_desc.m_texWidth = (float)tw;
		m_desc.m_texHeight = (float)th;

		D3DLOCKED_RECT rect;
		hr = m_desc.m_tex->LockRect(0, &rect, NULL, D3DLOCK_DISCARD);
		if (!checkHResultReturn(hr))
			return false;

		uint i, j, bmpC;
		D3DCOLOR color;
		int pitch = rect.Pitch / 4;
		int bmpPitch = face->glyph->bitmap.pitch;
		int pixelMode = face->glyph->bitmap.pixel_mode;

		for (j = 0; j < bmpHeight; ++j)
		{
			for (i = 0; i < bmpWidth; ++i)
			{
				int texIndex = j * pitch + i;
				int bmpIndex = j * bmpPitch + i;
				bmpC = face->glyph->bitmap.buffer[bmpIndex];
				
				if (FT_PIXEL_MODE_GRAY == pixelMode)
				{
					color = D3DCOLOR_ARGB(bmpC, bmpC, bmpC, bmpC);
					((D3DCOLOR*)rect.pBits)[texIndex] = color;
				}
				else if (FT_PIXEL_MODE_MONO == pixelMode)
				{
					bmpC = face->glyph->bitmap.buffer[i/8];
					unsigned char value = 0;
					if ((bmpC << (i%8)) & 0x80)
						value = 0xff;

					color = D3DCOLOR_ARGB(value, value, value, value);
					((D3DCOLOR*)rect.pBits)[texIndex] = color;
				}
				else
				{
					color = D3DCOLOR_ARGB(bmpC, bmpC, bmpC, bmpC);
					((D3DCOLOR*)rect.pBits)[texIndex] = color;
				}
			}
		}

		if (!checkHResultReturn(m_desc.m_tex->UnlockRect(0)))
			return false;

		return true;
	}
}