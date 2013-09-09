
#include "BreadEntity.h"
#include "BreadBatchRenderManager.h"
#include "BreadSceneManager.h"

namespace Bread
{
	implementBaseRTTI(cEntity)

	/*
	 *	
	 */
	cEntity::cEntity() : m_isSceneObject(false), 
						 m_renderMask(0), 
						 m_revisionRadian(0.0f),
						 m_cellIndex((uint)-1)
	{
		m_fadeAlpha = 1.0f;
		m_matRot.identity();
	}
	/*
	*/
	void cEntity::initialize(BUID const& buid)
	{
		m_property.m_buid = buid;
	}
	/*
	 *	
	 */
	void cEntity::initialize(sVector3 const& pos, float radian, bool isSceneObject, bool isSyncHeight)
	{
		m_isSceneObject = isSceneObject;
		setTranslation(pos, radian, isSyncHeight);
	}
	/*
	 *	
	 */
	void cEntity::initialize(BUID const& buid, sVector3 const& pos, float radian, bool isSceneObject, bool isSyncHeight)
	{
		m_property.m_buid = buid;
		cEntity::initialize(pos, radian, isSceneObject, isSyncHeight);
	}
	/*
	 *	
	 */
	void cEntity::setRotation(float radian)
	{
		m_property.m_radian = radian;
		m_matRot.rotationY(m_property.m_radian + getRevisionRadian());
	}
	/*
	*/
	void cEntity::setTranslation(sVector3 const& pos, float radian, bool isSyncHeight)
	{
		cEntity::setPosition(pos, isSyncHeight);
		cEntity::setRotation(radian);
	}
	/*
	*/
	void cEntity::calcuMatWorld(sMatrix4& matWorld) const
	{
		static sMatrix4 matPos(true);

		matPos.translation(getPosition());
		mulMat4(m_matRot, matPos, matWorld);
	}
	/*
	*/
	bool cEntity::isValidRenderMask(uint renderMask) const
	{
		if (cBatchRenderManager::MASK_ALL == renderMask)
			return true;

		if (getRenderMask() == renderMask)
			return true;
		return false;
	}
	/*
	*/
	void cEntity::setPosition(sVector3 const& pos, bool isSyncHeight)
	{
		m_property.m_position = pos;
		if (isSyncHeight)
			_getSceneMgr()->getTerrainHeight(m_property.m_position, m_cellIndex, 10.0f, true);
	}
	/*
	*/
	void cEntity::addPosition(sVector3 const& offset, bool isSyncHeight)
	{
		m_property.m_position += offset;
		if (isSyncHeight)
			_getSceneMgr()->getTerrainHeight(m_property.m_position, m_cellIndex);
	}
	/*
	*/
	void cEntity::syncHeight()
	{
		_getSceneMgr()->getTerrainHeight(m_property.m_position, m_cellIndex);
	}
}