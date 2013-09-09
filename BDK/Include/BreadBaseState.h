
#ifndef _BreadBaseState_h_
#define _BreadBaseState_h_

namespace Bread
{
	/**
	 * \ingroup Core
	 *
	 *
	 * \par requirements
	 * win98 or later\n
	 * win2k or later\n
	 * MFC\n
	 *
	 * \version 1.0
	 * first version
	 *
	 * \date 2009-07-25
	 *
	 * \author prodongi
	 *
	 * \par license
	 * This code is absolutely free to use and modify. The code is provided "as is" with
	 * no expressed or implied warranty. The author accepts no liability if it causes
	 * any damage to your computer, causes your pet to fall ill, increases baldness
	 * or makes your car start emitting strange noises when you start it up.
	 * This code has no bugs, just undocumented features!
	 * 
	 * \todo 상태를 나타내는 기본 인터페이스 클래스
	 *
	 * \bug 
	 *
	 */
	class iBaseState
	{
	public:
		virtual ~iBaseState() {}
		
		/// 이 상태를 활성화
		virtual void enter(const char* prevState) = 0;
		virtual void update(float elapsedtime) = 0;
		/// 상태가 끝날때
		virtual void exit(const char* nextState) = 0;

		/// 상태를 일시정지 시킨다
		virtual void override(const char* nextState) = 0;
		/// 상태를 재개한다
		virtual void resume(const char* prevState) = 0;
	};
}










#endif

















