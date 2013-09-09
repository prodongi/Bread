
#ifndef _BreadDataFactory_h_
#define _BreadDataFactory_h_

#include "BreadStdHeader.h"
#include "BreadInline.h"

namespace Bread
{
	/*
		@date 2011.01.21
		@auth prodongi
		@desc atlmap
		@todo 데이타들을 관리
	*/
	template <class K, class V, class B, class S, class T>
	class cDataFactory
	{
	protected:
		typedef K KEY;
		typedef V VALUE;
		typedef B BUCKET;
		typedef S SCHEDULER;
		typedef T THREAD;
		typedef typename T::LOADING_INFO LOADING_INFO;

	public:
		cDataFactory()
		{
			m_bucket			= NULL;
			m_scheduler			= NULL;
			m_loadingThread		= NULL;
			m_deletingThread	= NULL;
		}
		virtual void initialize()
		{
			m_bucket			= new BUCKET;
			m_scheduler			= new SCHEDULER;

			createThread();
		}
		virtual void finalize()
		{
			safeDelete(m_loadingThread, true);
			safeDelete(m_bucket);
			safeDelete(m_scheduler);
			safeDelete(m_deletingThread);
		}
		virtual void update(float /*elapsedtime*/) {}
		bool is(KEY const& key) const
		{
			return m_bucket->is(key);
		}
		V* get(KEY const& key) const
		{
			return m_bucket->get(key);
		}
		bool insert(KEY const& key, V* value)
		{
			return m_bucket->insert(key, &value);
		}
		bool remove(KEY const& key)
		{
			return m_bucket->remove(key);
		}
		V* create(KEY const& key)
		{
			return m_bucket->create(key);
		}
		V* create()
		{
			return m_bucket->create();
		}
		template <class Fn> void forEach(Fn func)
		{
			m_bucket->forEach(func);
		}
		size_t getBucketSize() const	
		{	
			return m_bucket->getBucketSize();
		}
		size_t getCount() const			
		{	
			return m_bucket->getCount();
		}
		void addLoadingList(LOADING_INFO const& info)
		{
			m_loadingThread->addLoadingList(info);
		}
		void directLoading(LOADING_INFO const& info)
		{
			m_loadingThread->directLoading(info);
		}
		void mergeThread()
		{
			m_loadingThread->merge();
			m_deletingThread->merge();
		}
		/// @brief m_loadingThread에서 copy한 데이타를 merge하기 위해 호출 함
		virtual void mergeCopyData(VALUE* /*copyData*/) {}

	protected:
		virtual ~cDataFactory() {}
		virtual void createThread() = 0;

	protected:
		BUCKET* m_bucket;
		SCHEDULER* m_scheduler;
		THREAD* m_loadingThread;
		THREAD* m_deletingThread;
	};
}



#endif