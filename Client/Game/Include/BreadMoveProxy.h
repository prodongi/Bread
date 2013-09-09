
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
		/// @brief 타겟 방향
		float m_targetRadian;
		sVector3 m_targetDir;
		// @brief 맨 처음 시작 위치(디버그 정보에 쓰임)
		sVector3 m_beginPos;
		/// @brief 이동 거리
		float m_moveLen;
		// @brief m_path에서 현재의 path
		int m_curPath;

		// @brief entity 이동에 따른 방향 자동 동기화
		struct sAutoSyncDir
		{
			bool m_apply;
			// @brief entity 회전 방향
			bool m_ccw;
			// @brief 변경되야 되는 값
			float m_offsetRadian;
			// @brief 회전 속도
			static float V;
		};
		sAutoSyncDir m_autoSyncDir;
	};
}



#endif