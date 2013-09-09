
#include "stdafx.h"
#include "EntityMover.h"
#include "BreadSceneManager.h"
#include "BreadGui.h"


/*
 *	
 */
cEntityMoverTest::cEntityMoverTest() : m_moving(false), m_moveV(10.0f), m_moveLen(0.0f)
{
}
/*
 *	
 */
cEntityMoverTest::~cEntityMoverTest()
{
}
/*
 *	
 */
void cEntityMoverTest::update(float elapsedtime)
{
	cEntityModel::update(elapsedtime);
	updateMove(elapsedtime);
	updateHeight();
}
/*
 *	
 */
void cEntityMoverTest::updateMove(float elapsedtime)
{
	if (!getMoving())
		return ;

	float s = m_moveV * elapsedtime;
	m_moveLen -= s;
	if (m_moveLen <= 0.0f)
	{
		m_position = m_destPos;
		setMoving(false);
	}
	else
	{
		sVector3 moveOffset;
		mulVec3(m_moveDir, s, moveOffset);
		moveOffset.y = 0.0f;
		m_position += moveOffset;
	}
}
/*
 *	
 */
void cEntityMoverTest::updateHeight()
{
	sVector3 pos(m_position);
	if (_getSceneMgr()->getTerrainHeight(pos))
		setPosition(pos);
}
/*
 *	
 */
void cEntityMoverTest::setDestPos(sVector3 const& pos)
{
	m_destPos = pos;
	if (m_position.equal(pos))
		return ;
	
	// 이동 방향 계산
	subVec3(pos, m_position, m_moveDir);
	m_moveDir.y = 0.0f;
	// 이동할 거리
	m_moveLen = m_moveDir.length();
	m_moveDir.normalize();

	// 모델의 각도를 구한다
	sVector3 v1(sVector3::X_BASIS);
	setRotation(v1.getRadian(m_moveDir));
}