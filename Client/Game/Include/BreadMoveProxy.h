
#ifndef _BreadMoveProxy_h_
#define _BreadMoveProxy_h_

/*
	BreadMoveProxy.h
	Copyright (c) prodongi. All rights resserved.

	date : 2012-08-27
	purpose : 
*/

#include "BreadDeclaration.h"
#include "BreadVector3.h"

namespace Bread
{
	class cEntity;

	/*
	*/
	struct sMoveProxyInfo
	{
		~sMoveProxyInfo() { m_path.clear(); }
		BUID m_buid;
		std::vector<sVector3> m_path;
		uint m_moveType;
		float m_speed;
	};
	/*
	*/
	class cMoveProxy
	{
	public:
		cMoveProxy();
		~cMoveProxy();

		void update(float elapsedtime);
		bool set(sMoveProxyInfo const& info);
		bool isEnd() const	{	return m_isEnd;	}
		void renderPath();

	private:
		void updateAutoSyncDir(float elapsedtime, cEntity* entity);
		void updatePosition(float elapsedtime, cEntity* entity);
		void setData(cEntity const* entity);
		void setEntityData(cEntity* entity, sMoveProxyInfo const& info);
		sVector3 const& getCurPath() const;
		bool isValidCurPath() const;

	private:
		bool m_isEnd;
		BUID m_buid;
		std::vector<sVector3> m_path;
		/// @brief Ÿ�� ����
		float m_targetRadian;
		sVector3 m_targetDir;
		// @brief �� ó�� ���� ��ġ(����� ������ ����)
		sVector3 m_beginPos;
		/// @brief �̵� �Ÿ�
		float m_moveLen;
		// @brief m_path���� ������ path
		int m_curPath;

		// @brief entity �̵��� ���� ���� �ڵ� ����ȭ
		struct sAutoSyncDir
		{
			bool m_apply;
			// @brief entity ȸ�� ����
			bool m_ccw;
			// @brief ����Ǿ� �Ǵ� ��
			float m_offsetRadian;
			// @brief ȸ�� �ӵ�
			static float V;
		};
		sAutoSyncDir m_autoSyncDir;
	};
}



#endif