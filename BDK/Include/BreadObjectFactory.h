
#ifndef _BreadObjectFactory_h_
#define _BreadObjectFactory_h_

#include "BreadStdHeader.h"

namespace Bread
{
	/*
		@date 2010.11.24
		@auth prodongi
		@desc object factory
		@todo 
	*/
	template <typename KEY, typename OBJECT>
	class cObjectFactory
	{
	private:
		typedef OBJECT* (*CreateCallback)();
		typedef std::map<KEY, CreateCallback> mCallBack;

	public:
		cObjectFactory()	{}
		~cObjectFactory()	{	unregisterCallBackAll();	}

		bool registerCallBack(KEY const& key, CreateCallback func)
		{
			if (is(key))
				return false;
			m_list.insert(std::make_pair(key, func));
			return true;
		}
		bool unregisterCallBack(KEY const& key)
		{
			if (!is(key))
				return false;
			m_list.erase(key);
		}
		void unregisterCallBackAll()
		{
			m_list.clear();
		}
		OBJECT* createObject(KEY const& key)
		{
			mCallBack::iterator it = m_list.find(key);
			if (it == m_list.end())
			{
				return NULL;
			}
			return (it->second)();
		}

	private:
		bool is(KEY const& key)
		{
			mCallBack::iterator it = m_list.find(key);
			return it != m_list.end();
		}

	private:
		mCallBack m_list;
	};
}



#endif