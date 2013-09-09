
#include "BreadShaderList.h"
#include "BreadMaterial.h"
#include "BreadCameraManager.h"
#include "BreadTextureManager.h"
#include "BreadEntityModel.h"
#include "BreadMatrix3.h"
#include "BreadLightManager.h"
#include "BreadShadowMap.h"
#include "BreadProjectedShadow.h"
#include "BreadShadowPSSM.h"
#include "BreadAnimationManager.h"
#include "BreadVBManager.h"

namespace Bread
{
	static sMatrix4 m;
	static sMatrix4 matWorld;
	static sMatrix4 matPos;
	static sMatrix4 matWVP;
	static sMatrix4 matWV;
	static sMatrix4 matWVPT;
	static sMatrix4 matWLP;
	
	/*
	*	
	*/
	void cProjectedShadowShader::setMaterialParam(cMaterial const* mtl)
	{
		unreferencedParameter(mtl);
	}
	/*
	*	
	*/
	void cProjectedShadowShader::setMeshParam(cMesh const* mesh, cEntityModel const* entity)
	{
		unreferencedParameter(mesh);

		cProjectedShadow* projShadow = (cProjectedShadow*)getUserData();

		// world
		entity->calcuMatWorld(matWorld);
		mulMat4(matWorld, projShadow->getMatShadowVP(), matWVPT);

		setMatrix(m_hMatWVP, &matWVPT);
	}
	/*
	*	
	*/
	void cProjectedShadowShader::registHandle()
	{
		m_hTProjShadow = insertTechnique(_T("TProjShadow"));
		m_hMatWVP = insertParameter(_T("matWVP"));
	}
	

	/*
	*	
	*/
	void cShadowMapDepthShader::setMeshParam(cMesh const* /*mesh*/, cEntityModel const* entity)
	{
		cShadowMap* shadowMap = (cShadowMap*)getUserData();
		entity->calcuMatWorld(matWorld);
		mulMat4(matWorld, shadowMap->getLightVP(), matWLP);
		setMatrix(m_hMatWLP, &matWLP);
	}
	/*
	*	
	*/
	void cShadowMapDepthShader::registHandle()
	{
		m_hTDepth = insertTechnique(_T("TDepth"));
		m_hMatWLP = insertParameter(_T("matWLP"));
		m_hDepthBias = insertParameter(_T("depthBias"));
	}

	/*
	*/
	void cShadowPSSMShader::setMeshParam(cMesh const* /*mesh*/, cEntityModel const* entity)
	{
		cShadowPSSM* shadowPSSM = (cShadowPSSM*)getUserData();
		entity->calcuMatWorld(matWorld);
		mulMat4(matWorld, shadowPSSM->getLightVP(), matWLP);
		setMatrix(m_matVP, &matWLP);
	}
	/*
	*/
	void cShadowPSSMShader::registHandle()
	{
		m_t = insertTechnique(_T("TShadowPSSM"));
		m_matVP = insertParameter(_T("matVP"));
	}

	
	/*
	*/
	void cDepthTextureShader::setMeshParam(cMesh const* /*mesh*/, cEntityModel const* entity)
	{
		cCamera* cam = _getCameraMgr()->getMain();

		// world
		entity->calcuMatWorld(matWorld);
		mulMat4(matWorld, cam->getVP(), matWVP);

		setMatrix(m_matWVP, &matWVP);
	}
	/*
	*/
	void cDepthTextureShader::registHandle()
	{
		m_t = insertTechnique(_T("T"));
		m_matWVP = insertParameter(_T("matWVP"));
		m_farClip = insertParameter(_T("farClip"));
	}

	/*
	*/
	void cSSAOShader::registHandle()
	{
		m_t = insertTechnique(_T("T"));
		m_depthTexture = insertParameter(_T("depthTexture"));
		m_rotTexture = insertParameter(_T("rotTexture"));
		m_farDistance = insertParameter(_T("farDistance"));
	}

	
	
	/*
	*/
	void cLightDepthMapShader::registHandle()
	{
		m_t = insertTechnique(_T("T"));
		m_matWLP = insertParameter(_T("matWLP"));
		m_matSkin = insertParameter(_T("matSkin"));
		m_skinLinkNum = insertParameter(_T("skinLinkNum"));
		m_isAni = insertParameter(_T("isAni"));
	}
	/*
	*/
	void cLightDepthMapShader::setMeshParam(cMesh const* mesh, cEntityModel const* entity)
	{
		entity->calcuMatWorld(matWorld);
	
		mulMat4(matWorld, m_matLP, matWLP);
		setMatrix(m_matWLP, &matWLP);

		cAnimation* animation = _getAnimationMgr()->get(entity->getAnimationBuid());
		if (animation && animation->getUseGpu())
		{
			setBool(m_isAni, TRUE);
			setInt(m_skinLinkNum, cAnimation::MAX_SKIN_LINK_NUM);

			sVBInfo* vbInfo = _getVBMgr()->get(mesh->m_vbBuid);
			if (vbInfo)
			{
				setMatrixArray(m_matSkin, vbInfo->m_localJointResult, vbInfo->getLocalJointNum());
			}
		}
		else
		{
			setBool(m_isAni, FALSE);
		}
	}

	/*
	*/
	void cNoTextureShader::registHandle()
	{
		m_t = insertTechnique(_T("T"));
		m_matWVP = insertParameter(_T("matWVP"));
		m_color = insertParameter(_T("color"));
	}
	/*
	*/
	void cNoTextureShader::setMeshParam(cMesh const* /*mesh*/, cEntityModel const* entity)
	{
		cCamera* cam = _getCameraMgr()->getMain();

		// world
		entity->calcuMatWorld(matWorld);
		mulMat4(matWorld, cam->getVP(), matWVP);
		setMatrix(m_matWVP, &matWVP);
	}

	/*
	*/
	void cWeatherShader::registHandle()
	{
		m_t = insertTechnique(_T("T"));
		m_matWVP = insertParameter(_T("matWVP"));
		m_matWVPPrev = insertParameter(_T("matWVPPrev"));
		m_positionOffset = insertParameter(_T("positionOffset"));
		m_velocity = insertParameter(_T("velocity"));
		m_alpha = insertParameter(_T("alpha"));
		m_viewPosition = insertParameter(_T("viewPosition"));
		m_sizeScale = insertParameter(_T("sizeScale"));
		m_lighting = insertParameter(_T("lighting"));
		m_forward = insertParameter(_T("forward"));
		m_diffuseTexture = insertParameter(_T("diffuseTexture"));
	}
	/*
	*/
	void cWeatherShader::setMeshParam(cMesh const* mesh, cEntityModel const* entity)
	{
		unreferencedParameter(mesh);
		unreferencedParameter(entity);
	}

	/*
	*/
	void cSkyBoxShader::registHandle()
	{
		m_t = insertTechnique(_T("T"));
		m_matWorld = insertParameter(_T("matWorld"));
		m_matVP = insertParameter(_T("matVP"));
		m_tex = insertParameter(_T("skyBox"));
	}
	/*
	*/
	void cSkyBoxShader::setMeshParam(cMesh const* /*mesh*/, cEntityModel const* /*entity*/)
	{
		cCamera* cam = _getCameraMgr()->getMain();

		setMatrix(m_matWorld, &cam->getWorld());
		setMatrix(m_matVP, &cam->getVP());
	}
}