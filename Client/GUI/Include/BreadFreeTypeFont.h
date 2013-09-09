
#ifndef _BreadFreeTypeFont_h_
#define _BreadFreeTypeFont_h_

#include "BreadDeclaration.h"
#include <ft2build.h>
#include FT_FREETYPE_H

namespace Bread
{
	/*
	*/
	struct sFreeTypeFontDesc
	{
		sFreeTypeFontDesc() {}
		sFreeTypeFontDesc(std_string const& name, long size) : m_name(name), m_size(size) {}
		std_string m_name;
		long	   m_size;
	};
	/*
		@date 2010.08.29
		@auth prodongi
		@desc 
		@todo 
	*/
	class cFreeTypeFont
	{
	public:
		cFreeTypeFont();
		~cFreeTypeFont();
		bool loadFont(std_string const& filename, FT_Library const& ftLib);
		bool loadFont(byte const* data, long size, FT_Library const& ftLib);
		void setSize(long size);
		FT_Face& getFace()	{	return m_face;	}

	private:
		FT_Face m_face;
	};
}



#endif