
#include "BreadLoadingThread.h"
#include "BreadDeletingThread.h"
#include "BreadTextureCache.h"
#include "BreadTextureCacheLoadingThread.h"
#include "BreadD3DSystemClassify.h"

namespace Bread
{
	implementInstance(cTextureCache)

	/*
	*/
	uint WINAPI _loadTextureThreadProc(LPVOID p)
	{
		reinterpret_cast<cTextureCacheLoadingThread<BUID, cTexture, sTextureLoadInfo, cTextureCache>*>(p)->loadingThreadProc();
#ifdef _DEBUG
		// release모드에서는 도달할 수 없는 부분이라고 하면서 에러 납니다.
		return 1;
#endif
	}

	/*
	*/
	cTextureCache::cTextureCache()
	{
		assignInstance(this)
	}
	/*
	*/
	cTextureCache::~cTextureCache()
	{
		assignInstance(NULL)
	}
	/*
	*/
	void cTextureCache::createThread()
	{
		m_loadingThread = new cTextureCacheLoadingThread<BUID, cTexture, sTextureLoadInfo, cTextureCache>;
		m_deletingThread = new cDeletingThreadEmpty<BUID, cTexture, sTextureLoadInfo, cTextureCache>;

		m_loadingThread->initialize(_loadTextureThreadProc, this);
	}
}