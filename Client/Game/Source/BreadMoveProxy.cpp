
/*
	BreadMoveProxy.cpp
	Copyright (c) prodongi. All rights resserved.
*/

#include "BreadMoveProxy.h"
#include "BreadD3DDeclaration.h"
#include "BreadEntityManager.h"
#include "BreadBatchRenderManager.h"
#include "BreadAnimationActionEnum.h"

namespace Bread
{
	float cMoveProxy::sAutoSyncDir::V = 10.0f;

	/*
	*/
	cMoveProxy::cMoveProxy() : m_isEnd(false)
	{
	}
	/*
	*/
	cMoveProxy::~cMoveProxy()
	{
	}
	/*/
	*/
	void cMoveProxy::update(float elapsedtime)
	{
		if (m_isEnd)
			return ;
		cEntity* entity = _getEntityMgr()->get(m_buid);
		if (!entity)
		{
			m_isEnd = true;
			return ;
		}

		updateAutoSyncDir(elapsedtime, entity);
		updatePosition(elapsedtime, entity);
	}
	/*
	*/
	void cMoveProxy::updateAutoSyncDir(float elapsedtime, cEntity* entity)
	{
		if (!m_autoSyncDir.m_apply)
			return ;

		float s = cMoveProxy::sAutoSyncDir::V * elapsedtime;
		float entityRadian = entity->getRadian();

		if (m_autoSyncDir.m_ccw)
		{
			entityRadian += s;
		}
		else
		{
			entityRadian -= s;
		}

		m_autoSyncDir.m_offsetRadian -= s;
		if (m_autoSyncDir.m_offsetRadian <= 0.0f)
		{
			m_autoSyncDir.m_apply = false;
			entityRadian = m_targetRadian;
		}

		entity->setRotation(entityRadian);
	}
	/*
	*/
	void cMoveProxy::updatePosition(float elapsedtime, cEntity* entity)
	{
		float s = entity->getSpeed() * elapsedtime;

		bool moveToEnd = false;
		if (m_moveLen <= s)
		{
			moveToEnd = true;
			s = m_moveLen;
		}
		else
		{
			m_moveLen -= s;
		}
				
		sVector3 offset;
		mulVec3(m_targetDir, s, offset);
		entity->addPosition(offset);
		
		if (moveToEnd)
		{
			++m_curPath;
			if (isValidCurPath())
			{
				m_autoSyncDir.m_apply = true;
				setData(entity);				
			}
			else
			{
				entity->setMotion(ANI_IDLE);
				m_isEnd = true;
			}
		}
	}
	/*
	*/
	void cMoveProxy::renderPath()
	{
		cEntity* entity = _getEntityMgr()->get(m_buid);
		if (!entity)
			return ;

		_getBatchRenderMgr()->push(&m_beginPos, &m_path[0], COLOR_RED);
		_getBatchRenderMgr()->push(m_path, COLOR_RED);
	}
	/*
	*/
	bool cMoveProxy::set(sMoveProxyInfo const& info)
	{
		cEntity* entity = _getEntityMgr()->get(info.m_buid);
		if (!entity)
		{
			m_isEnd = true;
			return false;
		}

		m_curPath = 0;
		m_isEnd = false;
		m_buid = info.m_buid;
		m_path = info.m_path;
		m_autoSyncDir.m_apply = true;
		m_beginPos = entity->getPosition();

		setData(entity);
		setEntityData(entity, info);

		return true;
	}
	/*
	*/
	void cMoveProxy::setData(cEntity const* entity)
	{
		float entityRadian = entity->getRadian();

		// entity�� ���� ����, entity�� m_matRot�� ������ ���̱� ������ ���� �ȵȴ�.
		sMatrix4 matEntity;
		matEntity.rotationY(entityRadian);
		sVector3 vEntity;
		mulMat4(matEntity, sVector3::X_BASIS, vEntity);

		// target pos�� ���� ����
		subVec3(getCurPath(), entity->getPosition(), m_targetDir);
		m_targetDir.normalize();
		
		// ȸ�� ����
		int det = sVector3::determinShortAngle(sVector3::Y_BASIS, vEntity, m_targetDir);
		m_autoSyncDir.m_ccw = (det == 0) ? true : false;

		// Ÿ�� ����
		m_targetRadian = m_targetDir.angle(sVector3::X_BASIS);
		cMath::wrapRadian(m_targetRadian);

		// ��ȭ ����, ���� ������ ���� ���Ѵ�
		m_autoSyncDir.m_offsetRadian = cMath::fabsf(entityRadian - m_targetRadian);
		if (m_autoSyncDir.m_offsetRadian > cMath::PI)
			m_autoSyncDir.m_offsetRadian = cMath::TWOPI - m_autoSyncDir.m_offsetRadian;

		/// �̵� �Ÿ�
		m_moveLen = entity->getPosition().length(getCurPath());		
	}
	/*
	*/
	void cMoveProxy::setEntityData(cEntity* entity, sMoveProxyInfo const& info)
	{
		uint motion;
		if (ENTITY_MOVE_WALK == info.m_moveType)
			motion = ANI_WALK;
		else
			motion = ANI_RUN;
		entity->setMotion(motion);
		entity->setSpeed(info.m_speed);
		entity->setMoveType(info.m_moveType);
	}
	/*
	*/
	sVector3 const& cMoveProxy::getCurPath() const
	{
		return m_path[m_curPath];
	}
	/*
	*/
	bool cMoveProxy::isValidCurPath() const
	{
		if (m_curPath < (int)m_path.size())
			return true;
		return false;
	}
}