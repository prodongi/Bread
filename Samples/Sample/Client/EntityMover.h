
#ifndef _EntityMoverTest_h_
#define _EntityMoverTest_h_

#include "BreadEntityModel.h"


class cEntityMoverTest : public cEntityModel
{
public:
	cEntityMoverTest();
	virtual ~cEntityMoverTest();
	virtual void update(float elapsedtime);
	
	void setDestPos(sVector3 const& pos);

	propertyConstFunc(float, MoveLen, m_moveLen)
	propertyConstFunc(float, MoveV, m_moveV)

protected:
	void updateMove(float elapsedtime);
	void updateHeight();

	propertyFunc(bool, Moving, m_moving)

protected:
	bool m_moving;
	/// @brief 이동할 거리
	float m_moveLen;
	/// @brief 이동 속도
	float m_moveV;
	/// @brief 이동 방향
	sVector3 m_moveDir;
	sVector3 m_destPos;

};



















#endif