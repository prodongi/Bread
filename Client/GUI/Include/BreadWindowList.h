
#ifndef _BreadWindowList_h_
#define _BreadWindowList_h_

#include "BreadD3DHeader.h"
#include "BreadStdHeader.h"
#include "BreadDeclaration.h"

namespace Bread
{
	template <class T>
	class cWindowList
	{
	public:
		typedef std::vector<T*>	vec_window;
		typedef std::map<std_string, T*> map_window;
		typedef typename vec_window::iterator itv_window;
		typedef typename map_window::const_iterator citm_window;

	public:
		cWindowList() {};
		~cWindowList() 
		{
			clear();
		};

		unsigned int getNum() const	{	return (unsigned int)m_list.size();	}
		void add(T* win)			
		{	
			m_list.push_back(win);	
			m_map.insert(std::make_pair(win->getUiName(), win));
		}
		void clear()
		{
			m_map.clear();
			safeClearVector<itv_window>(m_list);
		}
		void render(LPDIRECT3DDEVICE9 device)
		{
			itv_window it = m_list.begin();
			for (; it != m_list.end(); ++it)
			{
				(*it)->render(device);
			}
		}
		void update(float elapsedtime, int x, int y)
		{
			itv_window it = m_list.begin();
			for (; it != m_list.end(); ++it)
			{
				(*it)->update(elapsedtime, x, y);
			}
		}
		T* get(unsigned int seqNum) const
		{
			if (seqNum >= getNum())
			{
				//assert(0 && _T("Invalid sequence Number"));
				return NULL;
			}
			return m_list[seqNum];	
		}
		T* find(std_string const& key) const
		{
			citm_window it = m_map.find(key);
			if (it == m_map.end())
				return NULL;
			return it->second;
		}

	private:
		vec_window m_list;
		map_window m_map;
	};
}












#endif