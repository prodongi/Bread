
#ifndef _BreadClientSystemInfo_h_
#define _BreadClientSystemInfo_h_

#include "BreadSystemInfo.h"

namespace Bread
{
	class cClientSystemInfo : public cSystemInfo
	{
	public:
		cClientSystemInfo() {}
		virtual ~cClientSystemInfo() {}

	protected:
		virtual HRESULT getDXVersion(DWORD* pdwDirectXVersion, TCHAR* strDirectXVersion, int cchDirectXVersion);
	};
}

#endif