
#ifndef _BreadBucketVector_h_
#define _BreadBucketVector_h_

#include "BreadDefinition.h"
#include "BreadStdHeader.h"

namespace Bread
{
	
	/*
		@date 2010.08.27
		@auth prodongi
		@desc atlmap
		@todo 
	*/
	template <typename V, size_t bucketSize = 0>
	class cBucketVector
	{
	public:
		typedef std::vector<V>					vec_t;
		typedef typename vec_t::iterator		it_t;
		typedef typename vec_t::const_iterator	cit_t;

	public:
		cBucketVector() 
		{
			m_list = new vec_t;
			if (bucketSize)
				m_list->reserve(bucketSize);
		}
		virtual ~cBucketVector() 
		{
			clear();
			safeDelete(m_list);
		}
		V const& get(uint index) const
		{
			if (getBucketSize() <= (size_t)index)
				return NULL;
			return (*m_list)[index];
		}
		void pushBack(V const& t, bool setNull = false)
		{
			unreferencedParameter(setNull);
			m_list->push_back(t);
		}
		void pushBack(vec_t const& v_t)
		{
			m_list->reserve(getBucketSize() + v_t.size());
			m_list->insert(m_list->end(), v_t.begin(), v_t.end());
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
		void begin()					{	m_iter = m_list->begin();		}
		void next()						{	++m_iter;						}
		bool isEnd()					{	return m_iter == m_list->end();	}
		V& getIterData()				{	return (*m_iter);				}
		void erase()					{	m_iter = m_list->erase(m_iter);	}

	protected:
		vec_t* m_list;
		it_t   m_iter;
	};

	/*
		@date 2010.08.27
		@auth prodongi
		@desc atlmap
		@todo 
	*/
	template <typename V, size_t bucketSize>
	class cBucketVector <V*, bucketSize>
	{
	protected:
		typedef std::vector<V*>					vec_t;
		typedef typename vec_t::iterator		it_t;
		typedef typename vec_t::const_iterator	cit_t;

	public:
		cBucketVector() 
		{
			m_list = new vec_t;
			if (bucketSize)
				m_list->reserve(bucketSize);
		}
		virtual ~cBucketVector() 
		{
			safeDeleteVectorPtr<it_t>(m_list);	
		}
		V* get(uint index) const
		{
			if (getBucketSize() <= (size_t)index)
				return NULL;
			return (*m_list)[index];
		}
		void pushBack(V** t, bool setNull = false)
		{
			m_list->push_back(*t);
			if (setNull)
				*t = NULL;
		}
		void pushBack(vec_t const& v_t)
		{
			m_list->reserve(getBucketSize() + v_t.size());
			m_list->insert(m_list->end(), v_t.begin(), v_t.end());
		}
		V* create()
		{
			return new V;
		}
		void clear()
		{
			safeClearVectorPtr<it_t>(m_list);	
		}
		template <class Fn> void forEach(Fn func)
		{
			std::for_each(m_list->begin(), m_list->end(), func);
		}

		size_t getBucketSize() const	{	return m_list->size();			}
		void begin()					{	m_iter = m_list->begin();		}
		void next()						{	++m_iter;						}
		bool isEnd()					{	return m_iter == m_list->end();	}
		V* getIterData()				{	return (*m_iter);				}
		void erase()					{	safeDelete(*m_iter);	m_iter = m_list->erase(m_iter);	}

	protected:
		vec_t* m_list;
		it_t   m_iter;
	};
}








#endif