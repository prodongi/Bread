
#ifndef _BreadSSEMatrix4_h_
#define _BreadSSEMatrix4_h_

#include <xmmintrin.h>

namespace Bread
{
	/*
		@date 2011.08.16
		@auth prodongi
		@desc SSE�� �̿��� sMatrix4, Game Engine Architecture ����
		@todo 
	*/
	struct sSSEMatrix4
	{
		__m128 row1;
		__m128 row2;
		__m128 row3;
		__m128 row4;
	};
}



#endif