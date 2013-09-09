
#ifndef _BreadBoostNormalPool_h_
#define _BreadBoostNormalPool_h_

#include <boost/pool/pool.hpp>

#define declareNormalPool(className)											\
	public:																		\
		void* operator new(size_t /*size*/)	{	return m_pool.malloc();	}		\
		void operator delete(void* p)		{	m_pool.free(p);			}		\
		static void purgePool()				{	m_pool.purge_memory();	}		\
	private:																	\
		static boost::pool<> m_pool;											\
	public:

#define implementNormalPool(className)											\
		boost::pool<> className::m_pool(sizeof (className));


#endif