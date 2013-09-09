
#include "BreadLoadingThread.h"
#include "BreadDeletingThread.h"
#include "BreadModelCacheLoadingThread.h"
#include "BreadModelCache.h"
#include "BreadSceneManager.h"
#include "BreadUUIDManager.h"
#include "BreadMsgManager.h"
#include "BreadAnimation.h"
#include "BreadAnimationActionEnum.h"
#include "BreadRenderData.h"

namespace Bread
{
	/*
	*/
	bool sModelCache::loadingCacheThread(sModelLoadInfo const& info)
	{
		assert(!m_cacheData);
		m_cacheData = new cSerializeBmd::sCacheData;

		cSerializeBmd serialize;
		if (!serialize.load(info.getFileName(), *m_cacheData))
			return false;
		m_info = info;

#ifdef _DEBUG_TRACE_
//		trace(_T("thread load model cache : %s, %s\n"), m_info.getBuid().c_str(), m_info.getFileName().c_str());
#endif
		return true;
	}
	/*
	*/
	void sModelCache::setLoadingInfo(sModelLoadInfo const& info)
	{
		m_info = info;
	}
	/*
	*/
	sModelCache* sModelCache::copyingThread()
	{
		assert(m_cacheData);
		assert(!m_data);

		sModelCache* copyData = new sModelCache;
		copyData->m_data = new cSerializeBmd::sData;

		m_cacheData->copy(copyData->m_data, m_info.m_shareType);
		copyData->m_info = m_info;

#ifdef _DEBUG_TRACE_
//		trace(_T("thread copy model : %s, %s\n"), m_info.getBuid().c_str(), m_info.getFileName().c_str());
#endif
		return copyData;
	}

	/*
	*/
	uint WINAPI _loadModelThreadProc(LPVOID p)
	{
		reinterpret_cast<cModelCacheLoadingThread<BUID, sModelCache, sModelLoadInfo, cModelCache>*>(p)->loadingThreadProc();
#ifdef _DEBUG
		// release모드에서는 도달할 수 없는 부분이라고 하면서 에러 납니다.
		return 1;
#endif
	}

	/*
	*/
	void cModelCache::createThread()
	{
		m_loadingThread = new cModelCacheLoadingThread<BUID, sModelCache, sModelLoadInfo, cModelCache>;
		m_deletingThread = new cDeletingThreadEmpty<BUID, sModelCache, sModelLoadInfo, cModelCache>;

		m_loadingThread->initialize(_loadModelThreadProc, this);
	}
	/*
	*/
	void cModelCache::mergeCopyData(sModelCache* copyData)
	{
		assert(copyData && _T("copyData is null"));

		cEntityModel* entity = _getSceneMgr()->getEntity(copyData->m_info.getBuid());
		assert(entity && _T("entity is null"));

		cSerializeBmd::sData* bmdData = copyData->m_data;
		if (bmdData->m_renderData)
		{
			bmdData->m_renderData->m_entity = entity;
		}

		/// make renderData UUID
		BUID uuidBuid;
		if (bmdData->m_renderData)
		{
			uuidBuid = bmdData->m_renderData->m_buid;
			_getUUID()->encode(uuidBuid);
			bmdData->m_renderData->m_buid = uuidBuid;
		}
		/// make aniData UUID
		if (bmdData->m_animation)
		{
			uuidBuid = bmdData->m_animation->getBuid();
			_getUUID()->encode(uuidBuid);
			bmdData->m_animation->setBuid(uuidBuid);
		}

		_getSceneMgr()->insertRenderData(bmdData->m_renderData, copyData->m_info.m_isScene);
		_getSceneMgr()->insertAnimation(bmdData->m_animation);

		BUID renderDataBuid = (bmdData->m_renderData) ? bmdData->m_renderData->getBuid() : _T("");
		BUID animationBuid = (bmdData->m_animation) ? bmdData->m_animation->getBuid() : _T("");
		entity->initialize(copyData->m_info.m_pos, 
						   copyData->m_info.m_radian, 
						   renderDataBuid,
						   animationBuid, 
						   copyData->m_info.m_isScene);

		entity->setMotion(ANI_IDLE);

		// renderData와 animation은 메니저에서 관리하기 때문에 sModelCache 파괴자에서 삭제하지 않도록 null을 넣어준다
		bmdData->m_renderData = NULL;
		bmdData->m_animation = NULL;

		/// add fadein 
		if (copyData->m_info.m_isFadeIn)
		{
			sMsgAddFadeInEntity* msg = new sMsgAddFadeInEntity;
			msg->m_buid = entity->getBuid();
			_getMsgMgr()->addBucket(msg);
		}
	}
}