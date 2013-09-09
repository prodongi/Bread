
#include "BreadVolumeCloud.h"
#include "BreadEntityMesh.h"
#include "BreadMeshCreater.h"
#include "BreadCloudUberShader.h"
#include "BreadSceneManager.h"
#include "BreadUberShaderManager.h"
#include "BreadTextureManager.h"
#include "BreadMaterial.h"
#include "BreadVolumeTexture.h"
#include "BreadRenderData.h"
#include "BreadUberShaderFragment.h"

namespace Bread
{
	/*
	*/
	cVolumeCloud::cVolumeCloud() : m_entity(NULL), 
								   m_shader(NULL),
								   m_isRender(true)
	{
	}
	/*
	*/
	cVolumeCloud::~cVolumeCloud()
	{
	}
	/*
	*/
	bool cVolumeCloud::initialize()
	{
		if (!createTexture())
			return false;
		if (!createEntity())
			return false;

		return true;
	}
	/*
	*/
	void cVolumeCloud::finalize()
	{
		safeDelete(m_entity);
		safeDelete(m_shader);
	}
	/*
	*/
	bool cVolumeCloud::createEntity()
	{
		BUID buid = _T("volumeCloudEntity");

		cEntityMesh* entity = dynamicCast<cEntityMesh*>(_getSceneMgr()->createEntity<cEntityMesh>(buid));
		if (!entity)
		{
			assert(0 && _T("failed create volume cloud entity"));
			return false;
		}

		float w = 100.0f;
		float h = 100.0f;
		float d = 100.0f;
		sCullFigure cullFigure;
		cullFigure.m_box.set(-w, w, -h, h, -d, d);
		cullFigure.m_sphere.m_radius = w * 2.0f;
		cullFigure.copyToBackup();


		cUberShader* shader = _getUberShaderMgr()->createShader(_T("cloudUberShader"), FRAGMENT_BASE);
		if (!shader)
		{
			assert(0 && _T("failed create volume cloud shader"));
			return false;
		}
		entity->createRenderData(&cullFigure, _T(""), shader->getBuid(), false);
		if (!_getMeshCreater()->createVolumeCloud(buid, w, h, 50))
			return false;

		entity->setPosition(sVector3(0.0f, 150.0f, 0.0f));
		entity->getRenderDataBuid();


		cRenderData* rd = _getSceneMgr()->getRenderData(entity->getRenderDataBuid());
		if (!rd)
			return false;
		cMaterial* mtl = rd->getMtl(buid);
		if (!mtl)
			return false;
		mtl->m_mapBuid[cMaterial::DIFFUSE] = m_texBuid;

		return true;
	}
	/*
	*/
	bool cVolumeCloud::createTexture()
	{
		m_texBuid = _T("cloudTexture");

		if (!_getTextureMgr()->createTexture(m_texBuid, TEXTURE_TYPE_VOLUME))
			return false;

		cTexture* tex = _getTextureMgr()->getTexture(m_texBuid);
		if (!tex)
			return false;
		sVolumeTexture* volumeTex = dynamicCast<sVolumeTexture*>(tex->getData());
		if (!volumeTex)
			return false;

		volumeTex->create(64, 64, 64, D3DFMT_A16B16G16R16F);
		volumeTex->fillNoise();

		return true;
	}
	/*
	*/
	void cVolumeCloud::render()
	{
		if (!m_isRender)
			return ;
	}
}