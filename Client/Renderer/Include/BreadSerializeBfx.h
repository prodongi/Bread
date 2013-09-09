
#ifndef _BreadSerializeBfx_h_
#define _BreadSerializeBfx_h_

#include "BreadSerializeBase.h"

namespace Bread
{
	/**
	 * \ingroup Renderer
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
	 * \date 2009-10-29
	 *
	 * \author Prodongi
	 *
	 * \par license
	 * This code is absolutely free to use and modify. The code is provided "as is" with
	 * no expressed or implied warranty. The author accepts no liability if it causes
	 * any damage to your computer, causes your pet to fall ill, increases baldness
	 * or makes your car start emitting strange noises when you start it up.
	 * This code has no bugs, just undocumented features!
	 * 
	 * \todo *.bfx 로드 클래스, *.bfx에는 각 매터리얼에 설정된 uber shader의 fragment가 세팅되어있다.
	 *
	 * \bug 
	 *
	 */
	class cSerializeBfx : public cSerializeBase
	{
	public:
		cSerializeBfx();
		virtual ~cSerializeBfx();

		void load(std_string const& filename);
		BUID const& getUberShader(std_string const& mtlName);
	private:
		/// @brief first: material name, second: uber shader fragments
		std::map<std_string, uint>	m_list;
	};
}











#endif