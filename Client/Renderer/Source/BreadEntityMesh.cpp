
#include "BreadEntityMesh.h"
#include "BreadUUIDManager.h"
#include "BreadSceneManager.h"
#include "BreadTextureManager.h"
#include "BreadRenderData.h"

namespace Bread
{
	implementRTTI(cEntityMesh, cEntityModel)

	/*
	*/
	cEntityMesh::cEntityMesh()
	{
	}
	/*
	*/
	cEntityMesh::~cEntityMesh()
	{
	}
	/*
	*/
	void cEntityMesh::initialize(sVector3 const& /*pos*/, float /*radian*/, BUID const& /*renderDataBuid*/, BUID const& /*animationBuid*/, bool /*isSceneObject*/, bool /*isSyncHeight*/)
	{
		assert(0 && _T("please, call anothers initialize function"));
	}
	/*
	*/
	void cEntityMesh::initialize(BUID const& /*buid*/, sVector3 const& /*pos*/, float /*radian*/, BUID const& /*renderDataBuid*/, BUID const& /*animationBuid*/, bool /*isSceneObject*/, bool /*isSyncHeight*/)
	{
		assert(0 && _T("please, call anothers initialize function"));
	}
	/*
	*/
	void cEntityMesh::createRenderData(sCullFigure const* cullFigure, std_string const& texFileName, BUID const& shaderBuid, bool isSceneObject, bool addOctree)
	{
		cRenderData* rd = new cRenderData;
		m_renderDataBuid = getBuid();
		_getUUID()->encode(m_renderDataBuid);
		rd->m_buid = m_renderDataBuid;
		rd->m_cullFigure = *cullFigure;
		rd->m_entity = this;
		_getSceneMgr()->insertRenderData(rd, isSceneObject, addOctree);

		/// 
		cRenderData::bucket_mesh* meshList = rd->getMeshList();
		cMesh* mesh = meshList->create();
		mesh->m_buid = getBuid();
		mesh->m_vbBuid = getBuid();
		mesh->m_ibBuid = getBuid();
		meshList->insert(mesh->m_buid, &mesh);

		/// 
		cRenderData::bucket_mtl* mtlList = rd->getMtlList();
		cMaterial* mtl = mtlList->create();
		mtl->m_buid = getBuid();
		mtl->m_diffuse = sVector3(1.0f, 1.0f, 1.0f);
		mtl->m_specular = sVector3(1.0f, 1.0f, 1.0f);
		mtl->m_ambient = sVector3(1.0f, 1.0f, 1.0f);
		mtl->m_opacity = 1.0f;
		mtl->m_glossiness = 1.0f;
		mtl->m_specularLevel = 1.0f;
		int mapIndex = cMaterial::DIFFUSE;
		_tcsncpy(mtl->m_filename[mapIndex], texFileName.c_str(), NAME_LEN);
		mtl->m_mapBuid[mapIndex] = cTexture::makeBUID(mtl->m_filename[mapIndex]);
		_tcsncpy(mtl->m_filename[cMaterial::OPACITY], texFileName.c_str(), NAME_LEN);
		_getTextureMgr()->loadTexture(mtl->m_filename[mapIndex]);

		mtl->m_meshList.push_back(getBuid());
		mtl->m_shaderBuid = shaderBuid;
		mtlList->insert(mtl->m_buid, &mtl);
	}
}