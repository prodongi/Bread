
#ifndef _BreadSSEVector4_h_
#define _BreadSSEVector4_h_

#include <xmmintrin.h>

namespace Bread
{
	/*
		@date 2011.08.16
		@auth prodongi
		@desc SSE�� �̿��� sVector4, Game Engine Architecture ����
		@todo 
	*/
	struct sSSEVector4
	{
		__m128 m;
	};
}



#endif