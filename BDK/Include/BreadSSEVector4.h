
#ifndef _BreadSSEVector4_h_
#define _BreadSSEVector4_h_

#include <xmmintrin.h>

namespace Bread
{
	/*
		@date 2011.08.16
		@auth prodongi
		@desc SSE를 이용한 sVector4, Game Engine Architecture 참조
		@todo 
	*/
	struct sSSEVector4
	{
		__m128 m;
	};
}



#endif