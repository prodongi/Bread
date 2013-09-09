
#ifndef _BreadFreeTypeLine_h_
#define _BreadFreeTypeLine_h_

#include "BreadFreeTypeFont.h"

namespace Bread
{
	/*
		@date 2010.07.05
		@auth prodongi
		@desc 
		@todo 
	*/
	class cFreeTypeLine
	{
	public:
		cFreeTypeLine() {}
		~cFreeTypeLine();
		BUID const& getGlyph(int index);
		size_t getGlyphNum() const	{	return m_glyphList.size();	}

	private:
		void setStr(std_string const& str);
		void addGlyph(BUID const& key);

		friend class cFreeType;

	private:
		std_string m_str;
		std::vector<BUID> m_glyphList;
	};
}



#endif