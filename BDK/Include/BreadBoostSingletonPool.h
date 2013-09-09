
#ifndef _BreadBoostSingletonPool_h_
#define _BreadBoostSingletonPool_h_

#include <boost/pool/singleton_pool.hpp>

/// @brief overhead�� ���̱� ���� thread non safty�� ������
#define singletonPool(className)											\
	public:																	\
		void* operator new (size_t /*size*/)								\
		{																	\
			void* p = boost::singleton_pool<className, sizeof (className),	\
			boost::default_user_allocator_new_delete,						\
			boost::details::pool::null_mutex>::malloc();					\
			assert(p && "Failed Pool");										\
			return p;														\
		}																	\
		void operator delete(void* p)										\
		{																	\
			boost::singleton_pool<className, sizeof (className),			\
			boost::default_user_allocator_new_delete,						\
			boost::details::pool::null_mutex>::free(p);						\
		}																	\
		static void pergeMemory()											\
		{																	\
			boost::singleton_pool<className, sizeof (className),			\
			boost::default_user_allocator_new_delete,						\
			boost::details::pool::null_mutex>::purge_memory();				\
		}










#endif