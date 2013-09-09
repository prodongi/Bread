
#ifndef _BreadFreeTypeGlyphCash_h_
#define _BreadFreeTypeGlyphCash_h_

#include "BreadDeclaration.h"
#include "BreadBucketHash.h"
#include "BreadFreeTypeGlyph.h"

namespace Bread
{
	#define FREETYPEGLYPH_BUCKET_SIZE	4096
	/*
		@date 2010.09.01
		@auth prodongi
		@desc 
		@todo 
	*/
	class cFreeTypeGlyphCash : public cBucketHash<BUID, cFreeTypeGlyph*, FREETYPEGLYPH_BUCKET_SIZE>
	{
	public:
		cFreeTypeGlyphCash() {}
		virtual ~cFreeTypeGlyphCash() {}
		BUID getKey(sFreeTypeFontDesc const& desc, FT_Face& face, TCHAR c);

	protected:
		void makeKey(sFreeTypeFontDesc const& desc, TCHAR c, BUID& key);
	};
}



#endif