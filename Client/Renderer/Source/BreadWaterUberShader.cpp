
#include "BreadWaterUberShader.h"
#include "BreadTextureManager.h"
#include "BreadMaterial.h"
#include "BreadEntityModel.h"
#include "BreadCameraManager.h"
#include "BreadGUI.h"
#include "BreadCore.h"
#include "BreadEnvironment.h"
#include "BreadFog.h"

namespace Bread
{
	/*
	*/
	cUberShader* createWaterUberShader()
	{
		return new cWaterUberShader;
	}

	enum FRAGMENT_HANDLE
	{
		/// base
		H_TUBER, H_MAT_WORLD, H_MAT_WVP, H_AMBIENT,
		///
		H_TIMER, 
		/// texture
		H_DIFFUSE_TEXTURE, H_NORMAL_TEXTURE, H_ENVIRONMENT_TEXTURE, 
		/// fog
		H_FOG,
		//
		H_NUM
	};

	/*
	*/
	cWaterUberShader::cWaterUberShader() : m_time(0.0f)
	{
	}
	/*
	*/
	cWaterUberShader::~cWaterUberShader()
	{
	}
	/*
	*/
	bool cWaterUberShader::initialize()
	{
		std_string filename;

		filename = _T("waves2.dds");
		m_normalTexBuid = cTexture::makeBUID(filename);
		_getTextureMgr()->loadTexture(filename);

		filename = _T("cubemap.dds");
		m_environmentTexBuid = cTexture::makeBUID(filename);
		_getTextureMgr()->loadTexture(filename);

		defaultValue();

		return true;
	}
	/*
	*/
	void cWaterUberShader::update(float elapsedtime, cMesh* /*mesh*/, cMaterial* /*mtl*/, cEntityModel* /*entity*/)
	{
		updateTime(elapsedtime);
		updateKeyboard();
	}
	/*
	*/
	void cWaterUberShader::updateTime(float elapsedtime)
	{
		m_time += elapsedtime*0.1f;
		if (10.0f < m_time)
			m_time -= 10.0f;
	}
	/*
	*/
	void cWaterUberShader::modifyValue(float& value, float offset, float _min, float _max, TCHAR const* type)
	{
		if (_getInputSystem()->isLButtonDown())
		{
			offset *= -1.0f;
		}
		value += offset;
		value = max(_min, value);
		value = min(_max, value);
		trace(_T("%s : %f\n"), type, value);
	}
	/*
	*/
	void cWaterUberShader::updateKeyboard()
	{
		if (_getInputSystem()->isPressedKey(DIK_1))
		{
			modifyValue(m_waveFreq, 0.01f, 0.0f, 1.0f, _T("m_waveFreq"));
		}
		else if (_getInputSystem()->isPressedKey(DIK_2))
		{
			modifyValue(m_waveAmp, 0.1f, 0.0f, 10.0f, _T("m_waveAmp"));
		}
		else if (_getInputSystem()->isPressedKey(DIK_3))
		{
			modifyValue(m_bumpScale, 0.01f, 0.0f, 2.0f, _T("m_bumpScale"));
		}
		else if (_getInputSystem()->isPressedKey(DIK_4))
		{
			modifyValue(m_waterAmount, 0.01f, 0.0f, 2.0f, _T("m_waterAmount"));
		}
		else if (_getInputSystem()->isPressedKey(DIK_5))
		{
			modifyValue(m_reflectionAmount, 0.01f, 0.0f, 2.0f, _T("m_reflectionAmount"));
		}
		else if (_getInputSystem()->isPressedKey(DIK_6))
		{
			modifyValue(m_reflectionBlur, 0.01f, 0.0f, 1.0f, _T("m_reflectionBlur"));
		}
		else if (_getInputSystem()->isPressedKey(DIK_7))
		{
			modifyValue(m_fresnelPower, 0.1f, 1.0f, 10.0f, _T("m_fresnelPower"));
		}
		else if (_getInputSystem()->isPressedKey(DIK_8))
		{
			modifyValue(m_fresnelBias, 0.01f, 0.0f, 1.0f, _T("m_fresnelBias"));
		}
		else if (_getInputSystem()->isPressedKey(DIK_9))
		{
			modifyValue(m_hdrMultiplier, 1.0f, 0.0f, 100.0f, _T("m_hdrMultiplier"));
		}
		else if (_getInputSystem()->isPressedKey(DIK_0))
		{
			defaultValue();
		}
	}
	/*
	*/
	void cWaterUberShader::setMaterialParam(cMaterial const* mtl)
	{
		setTechnique(m_handle[H_TUBER]);

		if (m_handle[H_DIFFUSE_TEXTURE])
		{
			cTexture* tex = _getTextureMgr()->getTexture(mtl->m_mapBuid[cMaterial::DIFFUSE]);
			setTexture(m_handle[H_DIFFUSE_TEXTURE], tex);
		}
		if (m_handle[H_NORMAL_TEXTURE])
		{
			cTexture* tex = _getTextureMgr()->getTexture(m_normalTexBuid);
			setTexture(m_handle[H_NORMAL_TEXTURE], tex);
		}
		if (m_handle[H_ENVIRONMENT_TEXTURE])
		{
			cTexture* tex = _getTextureMgr()->getTexture(m_environmentTexBuid);
			setTexture(m_handle[H_ENVIRONMENT_TEXTURE], tex);
		}

		if (m_handle[H_TIMER])
		{
			setFloat(m_handle[H_TIMER], m_time);
		}

		setVector(_T("textureScale"),  &m_textureScale);
		setFloat(_T("waveFreq"), m_waveFreq);
		setFloat(_T("waveAmp"), m_waveAmp);
		setFloat(_T("bumpScale"), m_bumpScale);
		setVector(_T("bumpSpeed"), &m_bumpSpeed);
		setVector(_T("deepColor"), &m_deepColor);
		setVector(_T("shallowColor"), &m_shallowColor);
		setFloat(_T("waterAmount"), m_waterAmount);
		setVector(_T("reflectionColor"), &m_reflectionColor);
		setFloat(_T("reflectionAmount"), m_reflectionAmount);
		setFloat(_T("reflectionBlur"), m_reflectionBlur);
		setFloat(_T("fresnelPower"), m_fresnelPower);
		setFloat(_T("fresnelBias"), m_fresnelBias);
		setFloat(_T("hdrMultiplier"), m_hdrMultiplier);
	}
	/*
	*/
	void cWaterUberShader::setMeshParam(cMesh const* mesh, cEntityModel const* entity)
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
	}
	/*
	*/
	void cWaterUberShader::registHandle()
	{
		m_handle = new D3DXHANDLE[H_NUM];
		ZeroMemory(m_handle, sizeof (D3DXHANDLE) * H_NUM);

		/// base
		m_handle[H_TUBER] = insertTechnique(_T("TUber"));
		m_handle[H_MAT_WORLD] = insertParameter(_T("matWorld"));
		m_handle[H_MAT_WVP] = insertParameter(_T("matWVP"));
		m_handle[H_AMBIENT] = insertParameter(_T("ambient"));

		///
		m_handle[H_TIMER] = insertParameter(_T("time"));

		/// texture
		m_handle[H_DIFFUSE_TEXTURE] = insertParameter(_T("diffuseTexture"));
		m_handle[H_NORMAL_TEXTURE] = insertParameter(_T("normalTexture"));
		m_handle[H_ENVIRONMENT_TEXTURE] = insertParameter(_T("environmentTexture"));

		// fog
		if (_getEnv()->getFog()->getEnable())
			m_handle[H_FOG] = insertParameter(_T("fog"));

		insertParameter(_T("textureScale"));
		insertParameter(_T("waveFreq"));
		insertParameter(_T("waveAmp"));
		insertParameter(_T("bumpScale"));
		insertParameter(_T("bumpSpeed"));
		insertParameter(_T("deepColor"));
		insertParameter(_T("shallowColor"));
		insertParameter(_T("waterAmount"));
		insertParameter(_T("reflectionColor"));
		insertParameter(_T("reflectionAmount"));
		insertParameter(_T("reflectionBlur"));
		insertParameter(_T("fresnelPower"));
		insertParameter(_T("fresnelBias"));
		insertParameter(_T("hdrMultiplier"));
	}
	/*
	*/
	void cWaterUberShader::defaultValue()
	{
		m_textureScale = sVector2(8.0f, 4.0f);
		m_waveFreq = 0.0065f;
		m_waveAmp = 8.0f;
		m_bumpScale = 0.15f;
		m_bumpSpeed = sVector2(-0.1f, 0.0f);
		m_deepColor = sVector4(0.0f, 0.0f, 0.1f, 1.0f);
		m_shallowColor = sVector4(0.0f, 0.3f, 0.4f, 1.0f);
		m_waterAmount = 0.3f;

		m_reflectionColor = sVector4(1.0f, 1.0f, 1.0f, 1.0f);
		m_reflectionAmount = 1.0f;
		m_reflectionBlur = 0.0f;

		m_fresnelPower = 4.0f;
		m_fresnelBias = 0.2f;
		m_hdrMultiplier = 3.0f;
	}
}