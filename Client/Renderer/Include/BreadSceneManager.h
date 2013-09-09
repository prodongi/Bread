
#ifndef _BreadSceneManager_h_
#define _BreadSceneManager_h_

#include "BreadDeclaration.h"
#include "BreadVector3.h"
#include "BreadModelCache.h"
#include "BreadEntityManager.h"
#include "BreadCore.h"

namespace Bread
{
	class cEntityModel;
	class cVBManager;
	class cIBManager;
	class cUberShaderManager;
	class cAnimationManager;
	class cTerrain;
	class cPick;
	class cShader;
	class cTextureCache;
	class cTextureManager;
	class cEntityFadeManager;
	class cRenderDataManager;
	class cRenderDataCollector;
	class cMeshCreater;
	class cOctreeWorld;
	class cOctree;
	struct sMsgInfo;
	class cMoveProxyManager;

	/**
	 * \ingroup Renderer
	 *
	 *
	 * \par requirements
	 * win98 or later\n
	 * win2k or later\n
	 * MFC\n
	 *
	 * \version 1.0
	 * first version
	 *
	 * \date 2009-09-26
	 *
	 * \author Prodongi
	 *
	 * \par license
	 * This code is absolutely free to use and modify. The code is provided "as is" with
	 * no expressed or implied warranty. The author accepts no liability if it causes
	 * any damage to your computer, causes your pet to fall ill, increases baldness
	 * or makes your car start emitting strange noises when you start it up.
	 * This code has no bugs, just undocumented features!
	 * 
	 * \todo 
	 *
	 * \bug 
	 *
	 */
	class cSceneManager
	{
		declareClassCreater(cSceneManager)

	public:
		virtual bool initialize();
		virtual void finalize();
		virtual void update(float elapsedtime);
		virtual void msgProc(sMsgInfo* msg);
		virtual void mergeThread();
		virtual int  renderDebugInfo(int y);
		
		template <typename T>	bool createEntity(std_string const& filename, BUID const& buid, sVector3 const& pos, 
												  float radian, uint loadingType = 0, uint shareType = 0, bool isFadeIn = true);
		template <typename T> cEntityModel* createEntity(BUID const& buid);
		template <typename T>	bool createAvatar(std_string const& filename, BUID const& buid, sVector3 const& pos, 
												  float radian, uint loadingType = 0, uint shareType = 0, bool isFadeIn = true);
		bool createScene(std_string const& filename, int loadingType);
		virtual void render();
		/// @brief shader한개로 모든 render data를 렌더링 할때 쓰임
		void renderSingleUberShader(BUID const& shaderBuid, std_string const& technique, uint renderMask = 0);
		void renderSingleShader(cShader* shader, std_string const& technique, uint renderMask = 0);
		void renderRenderListShader(cShader* shader, std_string const& technique, cBatchRenderManager::vecEntityList const& renderList);
		void renderEntitySingleShader(cShader* shader, std_string const& technique, cEntityModel const* entity);
		/// @param offsetY first가 false일때 pos의 높이 옵셋값
		bool getTerrainHeight(sVector3& pos, uint& cellIndex, float offsetY = 10.f, bool first = false);
		bool pickTerrain();
		void insertRenderData(cRenderData* rd, bool isScene, bool addOctree = true);
		void insertAnimation(cAnimation* ani);

		__inline cRenderData* getRenderData(BUID const& buid) const;
		cOctree* getMainOctree() const;
		cPick* getPick()									{	return m_pick;					}
		cTerrain* getTerrain()								{	return m_terrain;				}
		cEntityModel* getEntity(BUID const& buid)			{	return m_entityMgr->get(buid);	}
		cEntityFadeManager* getEntityFadeMgr()				{	return m_entityFadeMgr;			}


	protected:
		/// @brief 각 멤버들에 메모리를 할당한다, 각 멤버들의 상속 때문에 따로 가상 함수로 뺐다.
		virtual void mallocMember();
		virtual void freeMember();

		void beginPick();
		void renderRdShader(cShader* shader, uint pass, cRenderData* rd);

	protected:
		cEntityManager*			m_entityMgr;
		cEntityFadeManager*		m_entityFadeMgr;		
		cVBManager*				m_vbMgr;
		cIBManager*				m_ibMgr;
		cUberShaderManager*		m_uberShaderMgr;
		cOctreeWorld*			m_worldOctree;
		sCullFigure				m_cullFigure;
		cRenderDataManager*		m_renderDataMgr;
		cTextureManager*		m_textureMgr;
		cAnimationManager*		m_animationMgr;
		cTerrain*				m_terrain;
		cPick*					m_pick;
		cModelCache*			m_modelCache;
		cRenderDataCollector*	m_collector;
		cMeshCreater*			m_meshCreater;
		cMoveProxyManager*		m_moveProxyMgr;
	};

	/*
	*	파일을 가지고 entity를 생성할 때 쓰임
	*/
	template <typename T>
	bool cSceneManager::createEntity(std_string const& filename, BUID const& buid, sVector3 const& pos, float radian, 
									 uint loadingType, uint shareType, bool isFadeIn)
	{
		assert(m_entityMgr);
		assert(m_renderDataMgr);

		if (LOADING_ACCESS == loadingType)
		{
		}
		else
		{
			if (!m_entityMgr->createEntity<T>(buid))
				return false;

			sModelLoadInfo info(buid, filename);
			info.m_pos			= pos;
			info.m_radian		= radian;
			info.m_shareType	= shareType;
			info.m_isFadeIn		= isFadeIn;

			if (LOADING_BACKGROUND == loadingType)
			{
				m_modelCache->addLoadingList(info);
			}
			else if (LOADING_DIRECT == loadingType)
			{
				m_modelCache->directLoading(info);
			}
		}

		return true;
	}
	/*
		메쉬를 직접 생성 할 때 쓰임, 쉐이더,렌더 데이타,메쉬 생성을 직접 해줘야 된다.
	*/
	template <typename T>
	cEntityModel* cSceneManager::createEntity(BUID const& buid)
	{
		return m_entityMgr->createEntity<T>(buid);
	}
	//
	template <typename T>
	bool cSceneManager::createAvatar(std_string const& filename, BUID const& buid, sVector3 const& pos, float radian, 
									 uint loadingType, uint shareType, bool isFadeIn)
	{
		assert(m_entityMgr);
		assert(m_renderDataMgr);

		if (LOADING_ACCESS == loadingType)
		{
		}
		else
		{
			if (!m_entityMgr->createAvatar<T>(buid))
				return false;

			sModelLoadInfo info(buid, filename);
			info.m_pos			= pos;
			info.m_radian		= radian;
			info.m_shareType	= shareType;
			info.m_isFadeIn		= isFadeIn;

			if (LOADING_BACKGROUND == loadingType)
			{
				m_modelCache->addLoadingList(info);
			}
			else if (LOADING_DIRECT == loadingType)
			{
				m_modelCache->directLoading(info);
			}
		}

		return true;
	}

}












#endif