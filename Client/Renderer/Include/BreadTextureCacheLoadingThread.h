
#ifndef _BreadModelCacheLoadingThread_h_
#define _BreadModelCacheLoadingThread_h_

#include "BreadLoadingThread.h"

namespace Bread
{
	/*
		@date 2011.04.21
		@auth prodongi
		@desc ������ ����� ������ ������ ���ؼ� ����� ����
		@todo 
	*/
	template <class K, class V, class I, class D>
	class cTextureCacheLoadingThread : public cLoadingThread<K, V, I, D>
	{
	public:
		cTextureCacheLoadingThread() {}
		virtual ~cTextureCacheLoadingThread() {}

	protected:
		virtual void merge()
		{
			cLoadingThread::merge();
		}
		virtual void loadingThread()
		{
			cLoadingThread::loadingThread();
		}
	};
}



#endif