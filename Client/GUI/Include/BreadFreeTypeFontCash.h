
#ifndef _BreadFreeTypeFontCash_h_
#define _BreadFreeTypeFontCash_h_

#include "BreadDeclaration.h"
#include "BreadFreeTypeFont.h"
#include "BreadBucketHash.h"

namespace Bread
{
	#define FREETYPEFONT_BUCKET_SIZE	10
	/*
		@date 2010.09.03
		@auth prodongi
		@desc font의 key값은 font name 이다
		@todo 
	*/
	class cFreeTypeFontCash : public cBucketHash<BUID, cFreeTypeFont*, FREETYPEFONT_BUCKET_SIZE>
	{
	public:
		cFreeTypeFontCash() {}
		virtual ~cFreeTypeFontCash() {}

		bool loadFont(std_string const& filename, FT_Library const& ftLib);
		bool loadFont(BUID const& key, byte const* data, long size, FT_Library const& ftLib);
	};
}



#endif