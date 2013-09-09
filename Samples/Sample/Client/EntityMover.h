
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
	/// @brief �̵��� �Ÿ�
	float m_moveLen;
	/// @brief �̵� �ӵ�
	float m_moveV;
	/// @brief �̵� ����
	sVector3 m_moveDir;
	sVector3 m_destPos;

};



















#endif