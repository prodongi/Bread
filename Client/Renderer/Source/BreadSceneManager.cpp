
#include "BreadSceneManager.h"
#include "BreadVBManager.h"
#include "BreadIBManager.h"
#include "BreadUberShaderManager.h"
#include "BreadCameraManager.h"
#include "BreadAnimationManager.h"
#include "BreadTerrain.h"
#include "BreadPick.h"
#include "BreadGui.h"
#include "BreadFileHelper.h"
#include "BreadTextureManager.h"
#include "BreadEntityFadeManager.h"
#include "BreadEntityManager.h"
#include "BreadClassCreater.h"
#include "BreadSerializeBsd.h"
#include "BreadSerializeBmd.h"
#include "BreadRenderDataManager.h"
#include "BreadRenderDataCollectorShader.h"
#include "BreadMeshCreater.h"
#include "BreadOctreeWorld.h"
#include "BreadProfile.h"
#include "BreadBatchRenderManager.h"
#include "BreadMoveProxyManager.h"

namespace Bread
{
	/*
	 *	
	 */
	cSceneManager::cSceneManager() : m_worldOctree(NULL), m_terrain(NULL)
	{
		mallocMember();
	}
	/*
	 *	
	 */
	cSceneManager::~cSceneManager()
	{
		freeMember();
	}
	/*
	 *	
	 */
	bool cSceneManager::initialize()
	{
		if (!m_uberShaderMgr->initialize())
			return false;
		return true;
	}
	/*
	 *	
	 */
	void cSceneManager::mallocMember()
	{
		m_entityMgr		= new cEntityManager;
		m_vbMgr			= new cVBManager;
		m_ibMgr			= new cIBManager;
		m_uberShaderMgr = new cUberShaderManager;
		m_renderDataMgr = new cRenderDataManager;
		m_collector		= new cRenderDataCollectorShader;
		m_pick			= new cPick;
		m_worldOctree	= new cOctreeWorld;
		float s = _getEngineConfig()->getZFar();
		if (!m_worldOctree->initialize(s, s, s))	return ;
		//
		m_modelCache	= new cModelCache;
		m_modelCache->initialize();
		//
		m_textureMgr	= new cTextureManager;
		m_textureMgr->initialize();
		//
		m_animationMgr  = _getClassCreater()->newAnimationMgr();
		m_animationMgr->initialize();
		//
		m_entityFadeMgr	= _getClassCreater()->newEntityFadeMgr();
		//
		m_meshCreater = _getClassCreater()->newMeshCreater();
		//
		m_moveProxyMgr = _getClassCreater()->newMoveProxyMgr();
	}
	/*
	 *	
	 */
	void cSceneManager::freeMember()
	{
		safeDelete(m_entityMgr);
		safeDelete(m_vbMgr);
		safeDelete(m_ibMgr);
		safeDelete(m_uberShaderMgr);
		safeDelete(m_renderDataMgr);
		safeDelete(m_collector);
		safeDelete(m_animationMgr);
		safeDelete(m_terrain, true);
		safeDelete(m_worldOctree, true);
		safeDelete(m_pick);
		safeDelete(m_modelCache, true);
		safeDelete(m_textureMgr, true);
		safeDelete(m_entityFadeMgr);
		safeDelete(m_meshCreater);
		safeDelete(m_moveProxyMgr);
	}
	/*
	 *	
	 */
	void cSceneManager::finalize()
	{
		m_uberShaderMgr->finalize();
	}
	/*
	*/
	bool cSceneManager::createScene(std_string const& filename, int loadingType)
	{
		cSerializeBsd serialize;
		cSerializeBsd::sData data;

		if (!serialize.load(filename, data))
			return false;

		m_worldOctree->addScene(data.getOctreeOnce(), true);
		m_terrain = data.getTerrainOnce();
		m_terrain->initialize();
		memcpy(&m_cullFigure, &data.m_cullFigure, sizeof (sCullFigure));

		assert(m_terrain);

		BUID buid;
		std_string _filename;
		std_string bmdName;
		std_string passName = cFileHelper::getPassNameInFullPassT(filename.c_str());

		if (LOADING_BACKGROUND == loadingType)
		{
			for (UINT i = 0; i < data.getRdSize(); ++i)
			{
				buid = data.m_rdList[i].m_buid;
				cSerializeBmd::makeFileName(buid, bmdName);
				_filename = passName + bmdName;

				sModelLoadInfo info(buid, _filename);
				info.m_pos = data.m_rdList[i].m_position;
				info.m_radian = 0.0f;
				info.m_isScene = true;
				info.m_shareType = (uint)(ST_IB | ST_VB);
				m_modelCache->addLoadingList(info);

				m_entityMgr->createEntity<cEntityModel>(info.getBuid());
			}
		}
		else if (LOADING_DIRECT == loadingType)
		{
			for (UINT i = 0; i < data.getRdSize(); ++i)
			{
				buid = data.m_rdList[i].m_buid;
				cSerializeBmd::makeFileName(buid, bmdName);
				_filename = passName + bmdName;

				createEntity<cEntityModel>(_filename, buid, data.m_rdList[i].m_position, 1.0f, loadingType);
			}
		}
		else if (LOADING_ACCESS == loadingType)
		{
		}

		return true;
	}
	/*
	*/
	cRenderData* cSceneManager::getRenderData(BUID const& buid) const
	{
		return m_renderDataMgr->get(buid);
	}
	/*
	 *	
	 */
	void cSceneManager::update(float elapsedtime)
	{
		beginPick();

		{
			PROFILE(_T("entityFadeMgr"));
			m_entityFadeMgr->update(elapsedtime);
		}
		{
			PROFILE(_T("textureMgr"));
			m_textureMgr->update(elapsedtime);
		}
		{
			PROFILE(_T("modelCache"));
			m_modelCache->update(elapsedtime);
		}
		{
			PROFILE(_T("moveProxyMgr"));
			m_moveProxyMgr->update(elapsedtime);
		}
		{
			PROFILE(_T("entityMgr"));
			m_entityMgr->update(elapsedtime);
		}
		{
			PROFILE(_T("worldOctree culling"));
			if (m_worldOctree) m_worldOctree->culling(_getCameraMgr()->getMain()->getFrustum());
		}
		{
			PROFILE(_T("collector"));
			m_collector->update(elapsedtime);
		}
		{
			PROFILE(_T("animationMgr"));
			m_animationMgr->update(elapsedtime);
		}
		{
			PROFILE(_T("uberShaderMgr"));
			m_uberShaderMgr->update(elapsedtime);
		}
	}
	/*
	*/
	void cSceneManager::msgProc(sMsgInfo* /*msg*/)
	{
	}
	/*
	*/
	void cSceneManager::mergeThread()
	{
		m_modelCache->mergeThread();
		m_textureMgr->mergeThread();
	}
	/*
	 *	
	 */
	void cSceneManager::render()
	{
		if (_getIsRender(RENDER_SCENE))
		{
			m_collector->render(m_renderDataMgr);
			m_collector->renderCullFigure();
		}
		if (_getIsRender(RENDER_SCENE_OCTREE))
			cRenderer::renderOctree(m_worldOctree, _getIsRenderValue1(RENDER_SCENE_OCTREE));
		if (_getIsRender(RENDER_BONE))
			m_animationMgr->renderBone();
		if (_getIsRender(RENDER_TERRAIN))
			m_terrain->render();
		if (_getIsRender(RENDER_PICK))
			m_pick->render(m_terrain);
		if (_getIsRender(RENDER_MOVE_PROXY_PATH))
			m_moveProxyMgr->renderPath();
		if (_getIsRender(RENDER_RENDERLIST_CULLFIGURE))
			_getBatchRenderMgr()->renderEntityCullFigure();
	}
	/*
	 *	
	 */
	void cSceneManager::renderSingleUberShader(BUID const& shaderBuid, std_string const& technique, uint renderMask)
	{
		renderSingleShader(m_uberShaderMgr->get(shaderBuid.c_str()), technique, renderMask);
	}
	/*
	 *	
	 */
	void cSceneManager::renderSingleShader(cShader* shader, std_string const& technique, uint renderMask)
	{
		m_collector->renderSingleShader(shader, technique, renderMask);
	}
	/*
	*/
	void cSceneManager::renderRenderListShader(cShader* shader, std_string const& technique, cBatchRenderManager::vecEntityList const& renderList)
	{
		if (!shader)
			return ;

		uint pass;
		shader->setTechnique(technique.c_str());
		shader->begin(&pass);

		cBatchRenderManager::vecEntityList::const_iterator it = renderList.begin();
		for (; it != renderList.end(); ++it)
		{
			renderRdShader(shader, pass, it->m_rd);
		}

		shader->end();
	}
	/*
	 *	
	 */
	void cSceneManager::renderEntitySingleShader(cShader* shader, std_string const& technique, cEntityModel const* entity)
	{
		if (!shader)
			return ;
		if (!entity)
			return ;
		cRenderData* rd = getRenderData(entity->getRenderDataBuid());
		if (!rd)
			return ;

		uint pass;
		shader->setTechnique(technique.c_str());
		shader->begin(&pass);

		renderRdShader(shader, pass, rd);

		shader->end();
	}
	/*
	 *	
	 */
	void cSceneManager::renderRdShader(cShader* shader, uint pass, cRenderData* rd)
	{
		if (!rd)
			return ;

		cRenderData::bucket_mesh* meshList = rd->getMeshList();
		for (meshList->begin(); !meshList->isEnd(); meshList->next())
		{
			cMesh* mesh = meshList->getIterData();
			shader->setMeshParam(mesh, rd->m_entity);

			for (uint p = 0; p < pass; ++p)
			{
				shader->beginPass(p);

				sVBInfo* vbInfo = _getVBMgr()->get(mesh->m_vbBuid);
				sIBInfo* ibInfo = _getIBMgr()->get(mesh->m_ibBuid);

				cRenderer::renderIndexedPrimitive(vbInfo, ibInfo);

				shader->endPass();
			}
		}
	}
	/*
	 *	
	 */
	void cSceneManager::beginPick()
	{
		PROFILE(_T("beginPick"));

		cCamera* cam = _getCameraMgr()->getMain();
		m_pick->begin(cam, _getInputSystem()->getMouseX(), _getInputSystem()->getMouseY());
	}
	/*
	 *	
	 */
	bool cSceneManager::pickTerrain()
	{
		return m_pick->pickTerrainFace(m_terrain);
	}
	/*
	 *	
	 */
	bool cSceneManager::getTerrainHeight(sVector3& pos, uint& cellIndex, float offsetY, bool first)
	{
		sVector3 orig = pos;
		if (first)
			orig.y = m_worldOctree->getWorldBound()->maxy();
		else
			orig.y += offsetY;

		float height;
		if (!m_pick->pickTerrainHeight(m_terrain, orig, height, cellIndex))
			return false;

		pos.y = height;
		return true;
	}
	/*
	 *	
	 */
	int cSceneManager::renderDebugInfo(int y)
	{
		TCHAR str[32];
		_stprintf(str, _T("render num : %d"), _getBatchRenderMgr()->getEntityListSize());
		_getFontMgr()->drawD3DText(10, y+=16, str);

		y = _getUberShaderMgr()->renderDebugInfo(y);
		y = m_pick->renderDebugInfo(y);

		return y;
	}
	/*
	*/
	void cSceneManager::insertRenderData(cRenderData* rd, bool isScene, bool addOctree)
	{
		if (!rd)
		{
			assert(!rd);
			return ;
		}
		m_renderDataMgr->insert(rd->m_buid, &rd);
		if (m_worldOctree && addOctree)
			m_worldOctree->add(rd, &rd->m_cullFigure.m_box, isScene);
	}
	/*
	*/
	void cSceneManager::insertAnimation(cAnimation* ani)
	{
		if (!ani)
		{
			assert(!ani);
			return ;
		}

		m_animationMgr->insert(ani->getBuid(), &ani);
	}
	/*
	*/
	cOctree* cSceneManager::getMainOctree() const
	{
		return m_worldOctree->getMain();
	}
}















