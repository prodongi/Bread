
#include "BreadUberShader.h"
#include "BreadUberShaderFragment.h"
#include "BreadCore.h"
#include "BreadTextureManager.h"
#include "BreadCameraManager.h"
#include "BreadShadowManager.h"
#include "BreadLightManager.h"
#include "BreadMaterial.h"
#include "BreadVector3.h"
#include "BreadMatrix4.h"
#include "BreadEntityModel.h"
#include "BreadShadowMap.h"
#include "BreadProjectedShadow.h"
#include "BreadShadowCSM.h"
#include "BreadEnvironment.h"
#include "BreadFog.h"
#include "BreadAnimationManager.h"
#include "BreadTimer.h"
#include "BreadShaderEffect.h"

namespace Bread
{
	/*
	*/
	cUberShader* createUberShader()
	{
		return new cUberShader;
	}

	enum FRAGMENT_HANDLE
	{
		// base
		H_TUBER, H_DIFFUSE_TEXTURE, H_MAT_WORLD, H_MAT_WVP, H_AMBIENT, H_FADE_ALPHA,
		H_MAT_WLP, H_MAT_WLPT, H_DEPTH_BIAS,
		// shadow map
		H_MAT_WLPB, H_SHADOWMAPDPETH_TEXTURE, 
		// projected shadow
		H_MAT_WVPT, H_PROJECTEDSHADOW_TEXTURE,
		// csm
		H_CSM_TEXTURE0, H_CSM_TEXTURE1, H_CSM_TEXTURE2, H_CSM_TEXTURE3, H_CSM_ZNEAR, 
		// bump, pom
		H_NORMAL_TEXTURE,
		// gpu ani
		H_MAT_SKIN, H_SKIN_LINK_NUM,
		// vegetation ani
		H_VEGETATION_TIME, H_VEGETATION_Y,
		// fog
		H_FOG,
		//
		H_NUM
	};

	/*
	 *	
	 */
	TCHAR const* cUberShader::makeBUID(std_string const& type, uint fragments)
	{
		static TCHAR buid[MAX_PATH];
		_stprintf(buid, _T("%s_%d"), type.c_str(), fragments);
		return buid;
	}
	/*
	*/
	TCHAR const* cUberShader::makeBUID(std_string const& type, std_string const& fragments)
	{
		uint f = _getUberShaderFragment()->stringToFragment(fragments.c_str());
		return cUberShader::makeBUID(type, f);
	}

	
	/*
	*/
	cUberShader::cUberShader() : m_fragments(0), m_handle(NULL)
	{
	}
	/*
	*/
	cUberShader::~cUberShader()
	{
		safeDeleteArray(m_handle);
	}
	/*
	 *	
	 */
	bool cUberShader::createEffect(std_string const& type, std::string const& srcEffectData, LPDIRECT3DDEVICE9 device, DWORD flags, uint fragments, LPD3DXEFFECTPOOL pool)
	{
		m_fragments = fragments;
		m_buid = cUberShader::makeBUID(type, fragments);

		std::string effectData;
		_getUberShaderFragment()->addDefinitionFragments(srcEffectData, fragments, effectData);

		if (!m_effect->create(device, effectData.c_str(), effectData.length(), flags, pool))
			return false;

		if (!initialize())
			return false;
		registHandle();
		shareRegistHandle();

		return true;
	}
	/*
	 *	
	 */
	void cUberShader::setMaterialParam(cMaterial const* mtl)
	{
		setTechnique(m_handle[H_TUBER]);

		if (m_handle[H_DIFFUSE_TEXTURE])
		{
			cTexture* tex = _getTextureMgr()->getTexture(mtl->m_mapBuid[cMaterial::DIFFUSE]);
			setTexture(m_handle[H_DIFFUSE_TEXTURE], tex);
		}
		
		if (m_handle[H_AMBIENT])
		{
			setVector(m_handle[H_AMBIENT], &mtl->m_ambient);
		}

		if (m_handle[H_NORMAL_TEXTURE])
		{
			cTexture* tex = _getTextureMgr()->getTexture(mtl->m_mapBuid[cMaterial::BUMP]);
			setTexture(m_handle[H_NORMAL_TEXTURE], tex);
		}

		// shadow
		if (_getShadowMgr()->isType(_T("shadowmap")))
		{
			cShadowMap* shadow = dynamicCast<cShadowMap*>(_getShadowMgr()->getShadow());
			setTexture(m_handle[H_SHADOWMAPDPETH_TEXTURE], shadow->getDepthTexture());
		}
		else if (_getShadowMgr()->isType(_T("projected")))
		{
			cProjectedShadow* shadow = dynamicCast<cProjectedShadow*>(_getShadowMgr()->getShadow());
			setTexture(m_handle[H_PROJECTEDSHADOW_TEXTURE], shadow->getShadowTexture());
		}
		else if (_getShadowMgr()->isType(_T("csm")))
		{
			cShadowCSM* shadow = dynamicCast<cShadowCSM*>(_getShadowMgr()->getShadow());
			setTexture(m_handle[H_CSM_TEXTURE0], shadow->getDepthTexture(0));
			setTexture(m_handle[H_CSM_TEXTURE1], shadow->getDepthTexture(1));
			setTexture(m_handle[H_CSM_TEXTURE2], shadow->getDepthTexture(2));
			setTexture(m_handle[H_CSM_TEXTURE3], shadow->getDepthTexture(3));
		}
	}
	/*
	 *	
	 */
	void cUberShader::setMeshParam(cMesh const* mesh, cEntityModel const* entity)
	{
		unreferencedParameter(mesh);

		sMatrix4 matWorld;
		entity->calcuMatWorld(matWorld);
		setMatrix(m_handle[H_MAT_WORLD], &matWorld);

		sMatrix4 matWVP;
		//cCamera const* mainCam = _getCameraMgr()->getMain();
		cCamera* mainCam = _getCameraMgr()->getMain();
		mulMat4(matWorld, mainCam->getVP(), matWVP);
		setMatrix(m_handle[H_MAT_WVP], &matWVP);

		setFloat(m_handle[H_FADE_ALPHA], entity->getFadeAlpha());

		// gpu ani
		if (_getUberShaderFragment()->is(_GPU_ANI_, m_fragments))
		{
			cAnimation* animation = _getAnimationMgr()->get(entity->getAnimationBuid());
			if (animation && animation->getUseGpu())
			{
				setInt(m_handle[H_SKIN_LINK_NUM], cAnimation::MAX_SKIN_LINK_NUM);

				sVBInfo* vbInfo = _getVBMgr()->get(mesh->m_vbBuid);
				if (vbInfo)
				{
					setMatrixArray(m_handle[H_MAT_SKIN], vbInfo->m_localJointResult, vbInfo->getLocalJointNum());
				}
			}
		}

		// vegetation ani
		if (_getUberShaderFragment()->is(_VEGETATION_ANI_, m_fragments))
		{
			setFloat(m_handle[H_VEGETATION_TIME], _getTimer()->getElapsedTotalTime());
			setFloat(m_handle[H_VEGETATION_Y], entity->getPosition().y);
		}

		// shadow
		if (_getShadowMgr()->isType(_T("shadowmap")))
		{
			cShadowMap* shadow = dynamicCast<cShadowMap*>(_getShadowMgr()->getShadow());
			sMatrix4 matWLP, matWLPB;

			mulMat4(matWorld, shadow->getLightVP(), matWLP);
			setMatrix(m_handle[H_MAT_WLP], &matWLP);

			mulMat4(matWorld, shadow->getScaleBias(), matWLPB);
			setMatrix(m_handle[H_MAT_WLPB], &matWLPB);
		}
		else if (_getShadowMgr()->isType(_T("projected")))
		{
			cProjectedShadow* shadow = dynamicCast<cProjectedShadow*>(_getShadowMgr()->getShadow());
			sMatrix4 matWVPT, matVPT;
			mulMat4(shadow->getMatShadowVP(), shadow->getMatShadowTex(), matVPT);
			mulMat4(matWorld, matVPT, matWVPT);
			setMatrix(m_handle[H_MAT_WVPT], &matWVPT);
		}
		else if (_getShadowMgr()->isType(_T("csm")))
		{
			cShadowCSM* shadow = dynamicCast<cShadowCSM*>(_getShadowMgr()->getShadow());
			sMatrix4 matLP, matT, matWLPT[cShadowCSM::CASCADED_NUM];
			sMatrix4 matLPT;

			for (uint cascadedIndex = 0; cascadedIndex < cShadowCSM::CASCADED_NUM; ++cascadedIndex)
			{
				mulMat4(shadow->getVP(cascadedIndex), shadow->getMatTex(), matLPT);
				mulMat4(matWorld, matLPT, matWLPT[cascadedIndex]);
			}
			setMatrixArray(m_handle[H_MAT_WLPT], matWLPT, cShadowCSM::CASCADED_NUM);
			setFloat(m_handle[H_DEPTH_BIAS], shadow->getDepthBias());

			sVector4 cascadedZNear;
			shadow->getCascadedZNear(cascadedZNear);
			setVector(m_handle[H_CSM_ZNEAR], &cascadedZNear); 
		}
	}
	/*
	 *	
	 */
	void cUberShader::registHandle()
	{
		m_handle = new D3DXHANDLE[H_NUM];
		ZeroMemory(m_handle, sizeof (D3DXHANDLE) * H_NUM);
		
		/// base
		m_handle[H_TUBER] = insertTechnique(_T("TUber"));
		m_handle[H_DIFFUSE_TEXTURE] = insertParameter(_T("diffuseTexture"));
		m_handle[H_MAT_WORLD] = insertParameter(_T("matWorld"));
		m_handle[H_MAT_WVP] = insertParameter(_T("matWVP"));
		m_handle[H_AMBIENT] = insertParameter(_T("ambient"));
		m_handle[H_FADE_ALPHA] = insertParameter(_T("fadeAlpha"));

		//
		if (_getUberShaderFragment()->is(_BUMP_, m_fragments) || _getUberShaderFragment()->is(_POM_, m_fragments))
		{
			m_handle[H_NORMAL_TEXTURE] = insertParameter(_T("normalTexture"));
		}

		//
		if (_getUberShaderFragment()->is(_GPU_ANI_, m_fragments))
		{
			m_handle[H_MAT_SKIN] = insertParameter(_T("matSkin"));
			m_handle[H_SKIN_LINK_NUM] = insertParameter(_T("skinLinkNum"));
		}

		//
		if (_getUberShaderFragment()->is(_VEGETATION_ANI_, m_fragments))
		{
			m_handle[H_VEGETATION_TIME] = insertParameter(_T("vegetationTime"));
			m_handle[H_VEGETATION_Y] = insertParameter(_T("vegetationY"));
		}

		// shadow
		if (_getShadowMgr()->isType(_T("shadowmap")))
		{
			m_handle[H_MAT_WLPB] = insertParameter(_T("matWLPB"));
			m_handle[H_MAT_WLP] = insertParameter(_T("matWLP"));
			m_handle[H_SHADOWMAPDPETH_TEXTURE] = insertParameter(_T("shadowMapDepthTexture"));
		}
		else if (_getShadowMgr()->isType(_T("projected")))
		{
			m_handle[H_MAT_WVPT] = insertParameter(_T("matWVPT"));
			m_handle[H_PROJECTEDSHADOW_TEXTURE] = insertParameter(_T("projectedShadowTexture"));
		}
		else if (_getShadowMgr()->isType(_T("csm")))
		{
			m_handle[H_MAT_WLPT] = insertParameter(_T("matWLPT"));
			m_handle[H_DEPTH_BIAS] = insertParameter(_T("depthBias"));
			m_handle[H_CSM_TEXTURE0] = insertParameter(_T("shadowCsmTexture0"));
			m_handle[H_CSM_TEXTURE1] = insertParameter(_T("shadowCsmTexture1"));
			m_handle[H_CSM_TEXTURE2] = insertParameter(_T("shadowCsmTexture2"));
			m_handle[H_CSM_TEXTURE3] = insertParameter(_T("shadowCsmTexture3"));
			m_handle[H_CSM_ZNEAR] = insertParameter(_T("cascadedZNear"));
		}

		// fog
		if (_getEnv()->getFog()->getEnable())
			m_handle[H_FOG] = insertParameter(_T("fog"));
	}
	/*
	*/
	void cUberShader::shareRegistHandle()
	{
		insertParameter(_T("eyePos"));
		insertParameter(_T("lightDir"));
		insertParameter(_T("matVP"));
	}
	/*
	*/
	void cUberShader::setDefaultTexture()
	{
		if (m_handle[H_DIFFUSE_TEXTURE])
		{
			setTexture(m_handle[H_DIFFUSE_TEXTURE], _getTextureMgr()->getDefault());
		}
	}
}