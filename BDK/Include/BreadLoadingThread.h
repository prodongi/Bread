
#ifndef _BreadLoadingThread_h_
#define _BreadLoadingThread_h_

#include <WinSock2.h>
#pragma comment (lib, "Ws2_32.lib")


#include <process.h> // _beginthread, _endthread
#include "BreadCriticalSection.h"
#include "BreadThreadBase.h"
#include "BreadBucketHash.h"

namespace Bread
{
	/*
		@date 2010.00.00
		@auth prodongi
		@desc 
		@todo 
	*/
	/// @param bucketSize �ε��� ��Ŷ�� ũ��
	template <class K, class V, class I, class D, size_t bucketSize = 100>
	class cLoadingThread : public cThreadBase<K, V, I, D>
	{
	private:
		enum LOADING_STATUS	
		{
			LS_LOADING,		/// @brief �ε� ���
			LS_LOADED		/// @brief �ε� �Ϸ�
		};
	public:
		cLoadingThread() : m_loadingEvent(0), m_loadingThread(0), m_dataFactory(NULL), 
						   m_loadingBucketMap(NULL), m_isLoading(false), m_isFinalize(false) {}
		virtual ~cLoadingThread() {}

		virtual void initialize(fcLoadingThreadProc _loadingThreadProc, DATA_FACTORY* dataFactory)
		{
			uint threadId;
			m_loadingEvent	= CreateEvent(NULL, FALSE, FALSE, NULL);
			m_loadingThread	= (HANDLE)_beginthreadex(NULL, 0, _loadingThreadProc, this, 0, &threadId);
			SetThreadPriority(m_loadingThread, THREAD_PRIORITY_LOWEST);

			m_cs.initialize();
			m_dataFactory = dataFactory;
			m_loadingBucketMap = new cBucketHash<BUID, int, bucketSize>;
		}
		virtual void finalize()
		{
			lock();
			m_isFinalize = true;
			unlock();

			/// ������ ���� ���
			while (m_isLoading) {}

			if (m_loadingThread)
			{
				DWORD exitcode;
				GetExitCodeThread(m_loadingThread, &exitcode);
				TerminateThread(m_loadingThread, exitcode);
				safeCloseHandle(m_loadingThread);
			}
			if (m_loadingEvent)				
				safeCloseHandle(m_loadingEvent);

			safeClearVectorPtr_R<iter_loaded>(m_loadedCacheList);
			safeClearVectorPtr_R<iter_loaded>(m_loadedCopyList);
			m_loadingList.clear();
			m_cs.finalize();
			safeDelete(m_loadingBucketMap);
#ifdef _DEBUG_TRACE_
			trace(_T("finalize loading thread\n"));
#endif
		}
		void loadingThreadProc()
		{
			int roof = 1;
			while (roof)
			{
				DWORD ret = WSAWaitForMultipleEvents(1, &m_loadingEvent, FALSE, WSA_INFINITE, FALSE);
				if (WSA_WAIT_EVENT_0 == ret)
				{
					m_isLoading = true;
					loadingThread();
					m_isLoading = false;
				}
				else if (WAIT_FAILED == ret)
					traceLastError();
			}
		}
		virtual void addLoadingList(LOADING_INFO const& info)
		{
			lock();
			/// @brief �ε� ��Ͽ� �ִ��� üũ
			if (m_loadingBucketMap->is(info.getCacheBuid()))
			{
				unlock();
				return ;
			}
			m_loadingBucketMap->insert(info.getCacheBuid(), LS_LOADING);
#ifdef _DEBUG_TRACE_
			trace(_T("add loading list : %s, %d\n"), info.getBuid().c_str(), m_loadingBucketMap->getCount());
#endif
			m_loadingList.push_back(info);
 			size_t loadCount = m_loadingList.size();
			unlock();

			if (1 == loadCount)
				setEvent();
		}
		// ���� ���μ����� ��� ������Ʈ�� ���� �Ŀ� ȣ��Ǿ�� �Ѵ�
		virtual void merge()
		{
			lock();
			bool ret;
			iter_loaded it;
			
			/// cache list
			it = m_loadedCacheList.begin();
			for (; it != m_loadedCacheList.end(); ++it)
			{
				ret = m_dataFactory->insert((*it)->getCacheBuid(), (*it));
				m_loadingBucketMap->remove((*it)->getCacheBuid());
				if (!ret)
				{
					safeDelete((*it));
				}
			}
			m_loadedCacheList.clear();

			/// copy list
			it = m_loadedCopyList.begin();
			for (; it != m_loadedCopyList.end(); ++it)
			{
				m_dataFactory->mergeCopyData(*it);
				m_loadingBucketMap->remove((*it)->getCacheBuid());
				safeDelete((*it));
			}
			m_loadedCopyList.clear();
			unlock();
		}
		/*
		*/
		virtual void directLoading(LOADING_INFO const& info)
		{
			lock();

			/// @brief �ε� ��Ͽ� �ִ��� üũ
			if (m_loadingBucketMap->is(info.getCacheBuid()))
			{
				unlock();
				return ;
			}

			VALUE* cache = m_dataFactory->get(info.getCacheBuid());

			if (!cache)
			{
				cache = m_dataFactory->create();
				if (!cache->loadingCacheThread(info))
				{
					safeDelete(cache);
					unlock();
					return ;
				}

				m_dataFactory->insert(info.getCacheBuid(), cache);
			}

			VALUE* copyData = cache->copyingThread();
			m_dataFactory->mergeCopyData(copyData);

			unlock();
		}

	private:
		void lock()		{	m_cs.lock();		}
		void unlock()		{	m_cs.unlock();	}
		void setEvent()
		{
			if (FALSE == SetEvent(m_loadingEvent))
				traceLastError();
		}
	protected:
		virtual void loadingThread()
		{
			lock();
			if (m_isFinalize)
			{
				unlock();
				return ;
			}
			unlock();

			lock();
			if (m_loadingList.empty())
			{
				unlock();
				return ;
			}
			iter_loading it = m_loadingList.begin();
			LOADING_INFO info = *it;
			m_loadingList.pop_front();

			size_t loadCount = m_loadingList.size();

			VALUE* cache = m_dataFactory->get(info.getCacheBuid());
			unlock();

			if (!cache)
			{
				cache = m_dataFactory->create();
				/// @brief ���⿡ lock()�� �� �ϸ� app������ ��,������ �ε����̸� �޸� ���� ����(� �������� �� �𸣰���)
				if (!cache || !cache->loadingCacheThread(info))
				{
					safeDelete(cache);

					lock();
					if (!m_isFinalize && 0 < loadCount)
						setEvent();
					unlock();

					return ;
				}
				lock();
				m_loadedCacheList.push_back(cache);
				unlock();
			}
			else
			{
				cache->setLoadingInfo(info);
			}

			VALUE* copyData = NULL;
			if (cache)
				copyData = cache->copyingThread();

			if (copyData)
			{
				lock();
				m_loadedCopyList.push_back(copyData);
				unlock();
			}

			lock();
			if (!m_isFinalize && 0 < loadCount)
				setEvent();
			unlock();
		}

	private:
		HANDLE m_loadingEvent;
		HANDLE m_loadingThread;
		cCriticalSection m_cs;

	protected:
		DATA_FACTORY* m_dataFactory;

		/// �ε��� ����Ʈ
		typedef std::list<LOADING_INFO>			list_loading;
		typedef typename list_loading::iterator	iter_loading;
		list_loading m_loadingList;

		/// �ε��� ����Ʈ �ؽ�
		cBucketHash<std_string, int, bucketSize>* m_loadingBucketMap;

		// �ε��� ����Ʈ
		typedef std::vector<VALUE*>				list_loaded;
		typedef typename list_loaded::iterator	iter_loaded;
		list_loaded m_loadedCacheList;
		list_loaded m_loadedCopyList;
		bool m_isLoading;
		bool m_isFinalize;
	};
}



#endif