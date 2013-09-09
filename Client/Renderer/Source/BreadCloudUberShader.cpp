
#include "BreadCloudUberShader.h"
#include "BreadMatrix4.h"
#include "BreadCameraManager.h"
#include "BreadEntityModel.h"
#include "BreadTextureManager.h"
#include "BreadCore.h"
#include "BreadRenderPropertyManager.h"
#include "BreadMaterial.h"

namespace Bread
{
	/*
	*/
	cUberShader* createCloudUberShader()
	{
		return new cCloudUberShader;
	}

	enum FRAGMENT_HANDLE
	{
		/// base
		H_TUBER, H_MAT_WVP,
		//
		H_TEXTURE,
		//
		H_LIGHT_POS, H_BLEND, H_NOISESCALE1, H_NOISESCALE2, H_IMPLICT_BLEND,
		//
		H_NUM
	};

	/*
	*/
	cCloudUberShader::cCloudUberShader()
	{
	}
	/*
	*/
	cCloudUberShader::~cCloudUberShader()
	{
	}
	/*
	*/
	bool cCloudUberShader::initialize()
	{
		return true;
	}
	/*
	*/
	void cCloudUberShader::update(float /*elapsedtime*/, cMesh* /*mesh*/, cMaterial* /*mtl*/, cEntityModel* /*entity*/)
	{
	}
	/*
	*/
	void cCloudUberShader::setMaterialParam(cMaterial const* mtl)
	{
		setTechnique(m_handle[H_TUBER]);

		setTexture(m_handle[H_TEXTURE], _getTextureMgr()->getTexture(mtl->m_mapBuid[cMaterial::DIFFUSE]));
	}
	/*
	*/
	void cCloudUberShader::setMeshParam(cMesh const* mesh, cEntityModel const* entity)
	{
		unreferencedParameter(mesh);
		unreferencedParameter(entity);

		sMatrix4 matWorld;
		entity->calcuMatWorld(matWorld);

		sMatrix4 matWVP;
		//cCamera const* mainCam = _getCameraMgr()->getMain();
		cCamera* mainCam = _getCameraMgr()->getMain();
		mulMat4(matWorld, mainCam->getVP(), matWVP);
		setMatrix(m_handle[H_MAT_WVP], &matWVP);
	}
	/*
	*/
	void cCloudUberShader::registHandle()
	{
		m_handle = new D3DXHANDLE[H_NUM];
		ZeroMemory(m_handle, sizeof (D3DXHANDLE) * H_NUM);

		/// base
		m_handle[H_TUBER] = insertTechnique(_T("TUber"));
		m_handle[H_TEXTURE] = insertParameter(_T("diffuseTexture"));
		m_handle[H_MAT_WVP] = insertParameter(_T("matWVP"));
		m_handle[H_LIGHT_POS] = insertParameter(_T("lightPos"));
		m_handle[H_BLEND] = insertParameter(_T("blend"));
		m_handle[H_NOISESCALE1] = insertParameter(_T("noiseScale1"));
		m_handle[H_NOISESCALE2] = insertParameter(_T("noiseScale2"));
		m_handle[H_IMPLICT_BLEND] = insertParameter(_T("implictBlend"));

		setDefaultValue();
	}
	/*
	*/
	void cCloudUberShader::setDefaultValue()
	{
		sVector3 lightPos(1.0f, 1.0f, -1.0f);
		setVector(_T("lightPos"), &lightPos);

		float blend = 0.0f;
		setFloat(_T("blend"), blend);

		float noiseScale1 = 1.7f;
		setFloat(_T("noiseScale1"), noiseScale1);

		float noiseScale2 = 5.0f;
		setFloat(_T("noiseScale2"), noiseScale2);

		float implictBlend = 0.3f;
		setFloat(_T("implictBlend"), implictBlend);
	}
	/*
	*/
	void cCloudUberShader::beginRenderProperty()
	{
		_getRenderPropertyMgr()->begin();
		_getRenderPropertyMgr()->setProperty(sRenderProperty::COLOROP, D3DTOP_MODULATE);
		_getRenderPropertyMgr()->setProperty(sRenderProperty::COLORARG1, D3DTA_TEXTURE);
		_getRenderPropertyMgr()->setProperty(sRenderProperty::COLORARG2, D3DTA_DIFFUSE);
		_getRenderPropertyMgr()->setProperty(sRenderProperty::ALPHAOP, D3DTOP_MODULATE);
		_getRenderPropertyMgr()->setProperty(sRenderProperty::ALPHAARG1, D3DTA_TEXTURE);
		_getRenderPropertyMgr()->setProperty(sRenderProperty::ALPHAARG2, D3DTA_DIFFUSE);
		_getRenderPropertyMgr()->setProperty(sRenderProperty::MINFILTER, D3DTEXF_LINEAR);
		_getRenderPropertyMgr()->setProperty(sRenderProperty::MAGFILTER, D3DTEXF_LINEAR);
		_getRenderPropertyMgr()->setProperty(sRenderProperty::TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT3);
		_getRenderPropertyMgr()->setProperty(sRenderProperty::TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU);

		_getRenderPropertyMgr()->setProperty(sRenderProperty::DITHERENABLE, FALSE);
		_getRenderPropertyMgr()->setProperty(sRenderProperty::SPECULARENABLE, FALSE);
		_getRenderPropertyMgr()->setProperty(sRenderProperty::ZENABLE, FALSE);
		_getRenderPropertyMgr()->setProperty(sRenderProperty::CULLMODE, FALSE);
		_getRenderPropertyMgr()->setProperty(sRenderProperty::ALPHABLENDENABLE, TRUE);
		_getRenderPropertyMgr()->setProperty(sRenderProperty::SRCBLEND, D3DBLEND_SRCALPHA);
		_getRenderPropertyMgr()->setProperty(sRenderProperty::DESTBLEND, D3DBLEND_INVSRCALPHA);

	}
	/*
	*/
	void cCloudUberShader::endRenderProperty()
	{
		_getRenderPropertyMgr()->end();
	}
}