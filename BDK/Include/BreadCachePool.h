
#ifndef _BreadCachePool_h_
#define _BreadCachePool_h_

#include "BreadStdHeader.h"

namespace Bread
{
#define declareCachePool()																		\
	public:																						\
		void* operator new[](size_t size)					{	return m_pool.alloc(size);	}	\
		void operator delete[] (void* p, size_t /*size*/)	{	m_pool.dealloc(p);	}			\
		static void purgePool()								{	m_pool.purge();				}	\
	private:																					\
		static cCachePool m_pool;																\
	public:

#define implementCachePool(className, maxPoolNum)												\
	cCachePool className::m_pool(maxPoolNum);

	/*
		@date 2011.07.11
		@auth prodongi
		@desc 메모리 캐쉬를 저장해 놓는 풀
		@todo 
	*/
	class cCachePool
	{
	public:
		cCachePool();
		cCachePool(int maxPoolNum);
		~cCachePool();

		void* alloc(size_t size);
		void dealloc(void* data);
		void purge();

	private:
		/// @brief m_list에서 size를 가진 캐쉬를 뽑아낸다(뽑아내면서 m_list에서 삭제)
		void* pop(size_t size);
		void push(void* data, size_t size);
		bool isFull() const;
		void freeFirst();
		size_t calcMallocSize(void* data);
		void* allocation(size_t size);
		void deallocation(void* data);

	public:
		enum	{	MAX_POOL_NUM = 20	};
	private:
		enum	{	KEEP_SIZE = 4 };

	private:
		struct sCache
		{
			size_t m_size;
			void* m_data;
		};
		typedef std::vector<sCache> v_cache;
		typedef v_cache::iterator it_cache;

		v_cache m_list;
		int m_maxPoolNum;
		bool m_enable;
	};
}



#endif
