
#include "BreadCachePool.h"

namespace Bread
{
	/*
	*/
	cCachePool::cCachePool() : m_maxPoolNum(MAX_POOL_NUM), m_enable(true)
	{
	}
	/*
	*/
	cCachePool::cCachePool(int maxPoolNum) : m_maxPoolNum(maxPoolNum),
											 m_enable(true)
	{
		m_list.reserve(m_maxPoolNum);
	}
	/*
	*/
	cCachePool::~cCachePool()
	{
	}
	/*
	*/
	void* cCachePool::alloc(size_t size)
	{
		if (m_enable)
		{
			void* p = pop(size);
			if (p)
				return p;
		}

		return allocation(size);
	}
	/*
	*/
	void cCachePool::dealloc(void* data)
	{
		if (m_enable)
		{
			if (isFull())
				freeFirst();

			push(data, calcMallocSize(data));
		}
		else
		{
			deallocation(data);
		}
	}
	/*
	*/
	void* cCachePool::allocation(size_t size)
	{
		size_t* data = (size_t*)std::malloc(size + KEEP_SIZE);
		*data = size;
		return (char*)data + KEEP_SIZE;
	}
	/*
	*/
	void cCachePool::deallocation(void* data)
	{
		char* temp = (char*)data - KEEP_SIZE;
		std::free(temp);
	}
	/*
	*/
	size_t cCachePool::calcMallocSize(void* data)
	{
		char* temp = (char*)data - KEEP_SIZE;
		size_t size = *((size_t*)temp);
		return size;
	}
	/*
	*/
	void cCachePool::purge()
	{
		m_enable = false;

		it_cache it = m_list.begin();
		for (; it != m_list.end(); ++it)
			deallocation(it->m_data);
		m_list.clear();
	}
	/*
	*/
	void* cCachePool::pop(size_t size)
	{
		it_cache it = m_list.begin();
		for (; it != m_list.end(); ++it)
		{
			if (it->m_size == size)
			{
				void* data = it->m_data;
				m_list.erase(it);
				return data;
			}
		}

		return NULL;
	}
	/*
	*/
	void cCachePool::push(void* data, size_t size)
	{
		sCache cache;
		cache.m_size = size;
		cache.m_data = data;

		m_list.push_back(cache);
	}
	/*
	*/
	bool cCachePool::isFull() const
	{
		return m_maxPoolNum == (int)m_list.size();
	}
	/*
	*/
	void cCachePool::freeFirst()
	{
		if (m_list.empty())
			return ;
		it_cache it = m_list.begin();
		deallocation(it->m_data);
		m_list.erase(it);
	}
}