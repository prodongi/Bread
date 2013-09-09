
#ifndef _BreadDeletingThread_h_
#define _BreadDeletingThread_h_

#ifndef _WINSOCKAPI_
#include <WinSock2.h>
#pragma comment (lib, "Ws2_32.lib")
#endif

#include <process.h> // _beginthread, _endthread
#include "BreadCriticalSection.h"
#include "BreadThreadBase.h"

namespace Bread
{
	/*
		@date 2010.00.00
		@auth prodongi
		@desc 
		@todo 
	*/
	template <class K, class V, class I, class D>
	class cDeletingThreadEmpty : public cThreadBase<K, V, I, D>
	{
	public:
		cDeletingThreadEmpty() {}
		virtual ~cDeletingThreadEmpty() {}
	};
	
	/*
		@date 2010.00.00
		@auth prodongi
		@desc 
		@todo 
	*/
	class cDeletingThread
	{
	};
}



#endif