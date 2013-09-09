
#ifndef _BreadBoostObjectPool_h_
#define _BreadBoostObjectPool_h_

#include <boost/pool/object_pool.hpp>

#define declareObjectPool(className)									\
	public:																\
		void* operator new()			{	return m_pool.malloc();	}	\
		void* operator new[](size_t size) {	return m_pool.ordered_malloc(size);	}	\
		void operator delete(void* p)	{	m_pool.free(p);			}	\
		void operator delete[](void* p)	{	m_pool.ordered_free(p);	}	\
		static void purgePool()			{	m_pool.purge_memory();	}	\
	private:															\
		static boost::object_pool<className> m_pool;					\
	public:		
										
#define implementObjectPool(className, chunkNum)						\
	boost::object_pool<className> className::m_pool(chunkNum);


#endif