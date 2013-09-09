
#ifndef _BreadD3DBufferManager_h_
#define _BreadD3DBufferManager_h_

#include "BreadDeclaration.h"

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
	 * \date 2009-10-01
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
	 * \todo 
	 *
	 * \bug 
	 *
	 */
	template <typename T>
	class cD3DBufferManager
	{
	protected:
		typedef std::map<BUID, T*>					map_buffer;
		typedef typename map_buffer::iterator		it_buffer;
		typedef typename map_buffer::const_iterator	cit_buffer;

	public:
		cD3DBufferManager() {}
		virtual ~cD3DBufferManager() 
		{
			safeClearMap<it_buffer>(m_bufferList);	
		}

		bool is(BUID const& buid) const
		{
			cit_buffer it = m_bufferList.find(buid);
			return it != m_bufferList.end() ? true : false;
		}
		T* get(BUID const& buid) const
		{
			cit_buffer it = m_bufferList.find(buid);
			if (it == m_bufferList.end())
				return NULL;
			return it->second;
		}
		void insert(BUID const& buid, T* t)
		{
			m_bufferList[buid] = t;
		}

	protected:
		map_buffer m_bufferList;
	};
}








#endif