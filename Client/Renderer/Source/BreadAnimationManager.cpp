
#include "BreadAnimationManager.h"
#include "BreadEntityModel.h"
#include "BreadD3DSystemClassify.h"
#include "BreadRenderData.h"
#include "BreadCore.h"
#include "BreadRenderPropertyManager.h"
#include "BreadRenderPrimitive.h"
#include "BreadAnimationActionManager.h"
#include "BreadAnimationEventManager.h"
#include "BreadAnimationBlendResult.h"
#include "BreadBatchRenderManager.h"

namespace Bread
{
	implementInstance(cAnimationManager)

	/*
	*/
	struct sUpdateAnim
	{
		sUpdateAnim(float elapsedtime) : m_elapsedtime(elapsedtime) {}
		void operator()(cAnimation* animation)
		{
			if (animation)
				animation->update(m_elapsedtime);
		}
		float m_elapsedtime;
	};
	/*
	*/
	struct sUpdateSkinning
	{
		sUpdateSkinning(cAnimationManager* mgr) : m_mgr(mgr) {}
		void operator ()(cBatchRenderManager::sEntityList& rl)
		{
			if (rl.m_rd)
			{
				cAnimation* ad = m_mgr->get(rl.m_rd->m_entity->getAnimationBuid());
				if (ad)
					ad->skinning(rl.m_rd);
			}
		}

		cAnimationManager* m_mgr;
	};
	/*
	*/
	struct sRenderBoneAnim
	{
		sRenderBoneAnim(cAnimationManager* mgr) : m_mgr(mgr) {}
		void operator ()(cBatchRenderManager::sEntityList const& rl)
		{
			if (rl.m_rd)
			{
				cAnimation* ad = m_mgr->get(rl.m_rd->m_entity->getAnimationBuid());
				if (ad)
					ad->renderBone(rl.m_rd->m_entity->getPosition());
			}
		}
		cAnimationManager* m_mgr;
	};

	/*
	 *	
	 */
	cAnimationManager::cAnimationManager() : m_actionMgr(NULL),
											 m_eventMgr(NULL)
	{
		assignInstance(this)
	}
	/*
	 *	
	 */
	cAnimationManager::~cAnimationManager()
	{
		assignInstance(NULL)

		safeDelete(m_actionMgr, true);
		safeDelete(m_eventMgr);
		sJointInterResult::purgePool();
		sJointLastResult::purgePool();
	}
	/*
	*/
	void cAnimationManager::initialize()
	{
		m_actionMgr = _getClassCreater()->newAnimationActionMgr();
		m_actionMgr->initialize();

		m_eventMgr = _getClassCreater()->newAnimationEventMgr();
	}
	/*
	 *	
	 */
	void cAnimationManager::update(float elapsedtime)
	{
		forEach(sUpdateAnim(elapsedtime));
		_getBatchRenderMgr()->forEachEntityList(sUpdateSkinning(this));
	}
	/*
	 *	
	 */
	void cAnimationManager::renderBone()
	{
		_getRenderPropertyMgr()->begin();
		_getRenderPropertyMgr()->setProperty(sRenderProperty::ZENABLE, D3DZB_FALSE);

		LPDIRECT3DDEVICE9 device = cD3DSystem::getD3DDevice();
		device->SetTexture(0, NULL);
		device->SetFVF(sLineVertex::D3DFVF);

		_getBatchRenderMgr()->forEachEntityList(sRenderBoneAnim(this));

		_getRenderPropertyMgr()->end();
	}
	/*
	*/
	void cAnimationManager::setMotion(uint id, cEntityModel* entity)
	{
		if (!entity)
		{
			return ;
		}

		cAnimation* ani = get(entity->getAnimationBuid());
		if (!ani)
		{
			return ;
		}

		BUID motionBuid = m_actionMgr->getBuid(id);
		if (motionBuid.empty())
		{
			assert(0 && _T("motionBuid is empty"));
			return ;
		}

		///
		ani->setMotion(motionBuid);
		///
		cAnimationAction* newAction = m_actionMgr->get(motionBuid);
		if (newAction)
		{
			cAnimationAction* curAction = entity->getAnimationAction();
			if (curAction)
				curAction->terminate(entity);
			newAction->initialize(entity);
			entity->setAnimationAction(newAction);
		}
	}
}















