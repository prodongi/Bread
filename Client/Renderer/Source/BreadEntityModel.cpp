
#include "BreadEntityModel.h"
#include "BreadSceneManager.h"
#include "BreadCore.h"
#include "BreadDeclaration.h"
#include "BreadShader.h"
#include "BreadMesh.h"
#include "BreadAnimationManager.h"
#include "BreadAnimationActionManager.h"
#include "BreadRenderData.h"

namespace Bread
{
	implementRTTI(cEntityModel, cEntity)

	/*
	 *	
	 */
	cEntityModel::cEntityModel() : m_action(NULL)
	{
	}
	/*
	 *	
	 */
	cEntityModel::~cEntityModel()
	{
	}
	/*
	 *	
	 */
	void cEntityModel::initialize(sVector3 const& pos, float radian, BUID const& renderDataBuid, BUID const& animationBuid, bool isSceneObject, bool isSyncHeight)
	{
		// 순서 중요
		m_renderDataBuid = renderDataBuid;
		m_animationBuid = animationBuid;
		cEntity::initialize(pos, radian, isSceneObject, isSyncHeight);
	}
	/*
	*/
	void cEntityModel::initialize(BUID const& buid)
	{
		cEntity::initialize(buid);
	}
	/*
	 *	
	 */
	void cEntityModel::initialize(BUID const& buid, sVector3 const& pos, float radian, BUID const& renderDataBuid, BUID const& animationBuid, bool isSceneObject, bool isSyncHeight)
	{
		cEntity::initialize(buid);
		cEntityModel::initialize(pos, radian, renderDataBuid, animationBuid, isSceneObject, isSyncHeight);
	}
	/*
	 *	
	 */
	void cEntityModel::setPosition(sVector3 const& pos, bool isSyncHeight)
	{
		if (getPosition().equal(pos))
			return ;

		cEntity::setPosition(pos, isSyncHeight);
		cRenderData* rd = _getSceneMgr()->getRenderData(m_renderDataBuid);
		if (rd)
			rd->setTranslation(pos, getMatRot());
	}
	/*
	*/
	void cEntityModel::addPosition(sVector3 const& offset, bool isSyncHeight)
	{
		if (offset.isZero())
			return ;

		cEntity::addPosition(offset, isSyncHeight);
		cRenderData* rd = _getSceneMgr()->getRenderData(m_renderDataBuid);
		if (rd)
			rd->setTranslation(getPosition(), getMatRot());
	}
	/*
	 *	
	 */
	void cEntityModel::setRotation(float radian)
	{
		cEntity::setRotation(radian);
		cRenderData* rd = _getSceneMgr()->getRenderData(m_renderDataBuid);
		if (rd)
			rd->setTranslation(getPosition(), getMatRot());
	}
	/*
	*/
	void cEntityModel::setTranslation(sVector3 const& pos, float radian, bool isSyncHeight)
	{
		if (!getPosition().equal(pos) || !cMath::equal(getRadian(), radian))
		{
			cEntity::setTranslation(pos, radian, isSyncHeight);
			cRenderData* rd = _getSceneMgr()->getRenderData(m_renderDataBuid);
			if (rd)
				rd->setTranslation(getPosition(), getMatRot());
		}
	}	
	/*
	*/
	void cEntityModel::setMotion(uint id)
	{
		_getAnimationMgr()->setMotion(id, this);
	}
	/*
	*/
	void cEntityModel::update(float elapsedtime)
	{
		if (m_action)
			m_action->action(elapsedtime, this);
	}
}




















