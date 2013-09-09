
#include "BreadSkyBox.h"
#include "BreadEntityMesh.h"
#include "BreadMeshCreater.h"
#include "BreadShaderList.h"
#include "BreadTextureManager.h"
#include "BreadSceneManager.h"

namespace Bread
{
	/*
	*
	*/
	cSkyBox::cSkyBox() : m_entity(NULL), m_shader(NULL), m_isRender(true)
	{
	}
	/*
	*/
	cSkyBox::~cSkyBox()
	{
	}
	/*
	*/
	bool cSkyBox::initialize(int /*type*/)
	{
		if (!createEntity())
			return false;
		if (!createShader())
			return false;
		loadTexture();

		return true;
	}
	/*
	*/
	void cSkyBox::finalize()
	{
		safeDelete(m_entity);
		safeDelete(m_shader);
	}
	/*
	*/
	void cSkyBox::loadTexture()
	{
		std_string filename(_T("skybox.dds"));
		_getTextureMgr()->loadTexture(filename, TEXTURE_TYPE_CUBE);
		m_texBuid = cTexture::makeBUID(filename);
	}
	/*
	*/
	bool cSkyBox::createEntity()
	{
		BUID buid = _T("skyBoxEntity");
		m_entity = new cEntityMesh;
		cEntityModel* model = dynamicCast<cEntityModel*>(m_entity);
		model->initialize(buid);

		sCullFigure cullFigure;
		cullFigure.m_box.set(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
		cullFigure.m_sphere.m_radius = 2.0f;
		cullFigure.copyToBackup();
		m_entity->createRenderData(&cullFigure, _T(""), _T(""), false, false);

		if (!_getMeshCreater()->createSkyBox(buid))
			return false;

		return true;
	}
	/*
	*/
	bool cSkyBox::createShader()
	{
		m_shader = new cSkyBoxShader;
		if (!m_shader->createEffect(_T("skybox.fx"), cD3DSystem::getD3DDevice(), cShader::makeFlags()))
			return false;
				
		return true;
	}
	/*
	*/
	void cSkyBox::render()
	{
		if (!m_isRender)
			return ;

		_getRenderPropertyMgr()->begin();
		_getRenderPropertyMgr()->setProperty(sRenderProperty::CULLMODE, D3DCULL_NONE);

		m_shader->setTexture(m_shader->m_tex, _getTextureMgr()->get(m_texBuid));
		_getSceneMgr()->renderEntitySingleShader(m_shader, _T("T"), m_entity);

		_getRenderPropertyMgr()->end();
	}
}