
#ifndef _BreadModelCacheLoadingThread_h_
#define _BreadModelCacheLoadingThread_h_

#include "BreadLoadingThread.h"

namespace Bread
{
	/*
		@date 2010.00.00
		@auth prodongi
		@desc ������ ����� ������ ������ ���ؼ� ����� ����
		@todo 
	*/
	template <class K, class V, class I, class D>
	class cModelCacheLoadingThread : public cLoadingThread<K, V, I, D>
	{
	public:
		cModelCacheLoadingThread() {}
		virtual ~cModelCacheLoadingThread() {}

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