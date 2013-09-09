
#ifndef _BreadBucketMap_h_
#define _BreadBucketMap_h_

#include <assert.h>

namespace Bread
{
	/*
		@date 2010.00.00
		@auth prodongi
		@desc 
		@todo 
	*/
	template <typename KEY, typename V>
	class cBucketMap
	{
	protected:
		typedef std::map<KEY, V>				map_t;
		typedef typename map_t::iterator		it_t;
		typedef typename map_t::const_iterator	cit_t;

	public:
		cBucketMap() 
		{
			m_list = new map_t;
		}
		virtual ~cBucketMap() 
		{
			m_list->clear();
			safeDelete(m_list);
		}
		bool is(KEY const& key) const
		{
			cit_t it = m_list->find(key);
			return it != m_list->end() ? true : false;
		}
		V& get(KEY const& key)
		{
			it_t it = m_list->find(key);
			if (it == m_list->end())
				return m_null;
			return it->second;
		}
		V& firstData()
		{
			if (m_list->empty())
				return m_null;
			cit_t it = m_list->begin();
			return it->second;
		}
		bool insert(KEY const& key, V const& v, bool setNull = false)
		{
			if (is(key))
			{
				assert(0 && "Failed insert");
				return false;
			}
			unreferencedParameter(setNull);
			m_list->insert(std::make_pair(key, v));
			return true;
		}
		bool remove(KEY const& key)
		{
			it_t it = m_list->erase(key);
			
			return it != m_list->end();
		}
		bool removeNext()
		{
			m_iter = m_list->erase(m_iter);
			return m_iter != m_list->end();
		}
		void clear()
		{
			m_list->clear();
		}

		template <class Fn> void forEach(Fn func)
		{
			std::for_each(m_list->begin(), m_list->end(), func);
		}

		size_t getBucketSize() const	{	return m_list->size();			}
		size_t getCount() const			{	return m_list->count();			}
		void begin()					{	m_iter = m_list->begin();		}
		void next()						{	++m_iter;						}
		bool isEnd()					{	return m_iter == m_list->end();	}
		V& getIterData()				{	return m_iter->second;			}
		KEY const& getIterKey()			{	return m_iter->first;			}

	protected:
		map_t* m_list;
		it_t   m_iter;
		V m_null;
	};

	/*
		@date 2010.00.00
		@auth prodongi
		@desc 
		@todo 
	*/
	template <typename KEY, typename V>
	class cBucketMap<KEY, V*>
	{
	public:
		typedef V valueType;
		typedef V* pointer;
	protected:
		typedef std::map<KEY, V*>				map_t;
		typedef typename map_t::iterator		it_t;
		typedef typename map_t::const_iterator	cit_t;

	public:
		cBucketMap() 
		{
			m_list = new map_t;
		}
		virtual ~cBucketMap() 
		{
			safeDeleteMapPtr<it_t, map_t*>(m_list);
		}
		bool is(KEY const& key) const
		{
			cit_t it = m_list->find(key);
			return it != m_list->end() ? true : false;
		}
		V* get(KEY const& key) const
		{
			cit_t it = m_list->find(key);
			if (it == m_list->end())
				return NULL;
			return it->second;
		}
		V* firstData() const
		{
			if (m_list->empty())
				return NULL;
			cit_t it = m_list->begin();
			return it->second;
		}
		bool insert(KEY const& key, V** v, bool setNull = false)
		{
			if (is(key))
			{
				assert(0 && "Failed insert");
				return false;
			}
			m_list->insert(std::make_pair(key, *v));
			if (setNull)
				*v = NULL;

			return true;
		}
		V* create(KEY const& key)
		{
			if (is(key))
			{
				assert(0 && "Failed created");
				return NULL;
			}

			return new V;
		}
		V* create()
		{
			return new V;
		}
		bool remove(KEY const& key)
		{
			V* v = get(key);
			if (!v)
				return false;
			safeDelete(v);
			it_t it = m_list->erase(key);
			return it != m_list->end();
		}
		bool removeNext()
		{
			V* v = get(key);
			if (!v)
				return true;
			safeDelete(v);
			m_iter = m_list->erase(m_iter);
			return m_iter != m_list->end();
		}
		void clear()
		{
			safeClearMapPtr<it_t>(m_list);	
		}
		template <class Fn> void forEach(Fn func)
		{
			std::for_each(m_list->begin(), m_list->end(), func);
		}
		template <typename T> void copy(T& copyData, uint shareType)
		{
			for (begin(); !isEnd(); next())
			{
				V* data = getIterData();
				T::valueType* v = copyData.create();
				data->copy(*v, shareType);
				copyData.insert(m_iter->first, &v);
			}
		}

		size_t getBucketSize() const	{	return m_list->size();			}
		size_t getCount() const			{	return m_list->count();			}
		void begin()					{	m_iter = m_list->begin();		}
		void next()						{	++m_iter;						}
		bool isEnd()					{	return m_iter == m_list->end();	}
		V* getIterData()				{	return m_iter->second;			}

	protected:
		map_t* m_list;
		it_t   m_iter;
	};
}








#endif