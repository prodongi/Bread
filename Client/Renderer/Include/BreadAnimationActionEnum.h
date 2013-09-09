
#ifndef _BreadAnimationActionEnum_h_
#define _BreadAnimationActionEnum_h_

namespace Bread
{
	enum ANIMATION_ACTION
	{
		ANI_IDLE = 1,	/// 0은 루아에서 테이블을 읽어 올 때, 에러코드로 쓰일 수 있어서 1로 설정함
		ANI_WALK,
		ANI_RUN,
		ANI_ATTACK,
		ANI_DEAD,
		MAX_ANI_ACTION,
	};
}

#endif