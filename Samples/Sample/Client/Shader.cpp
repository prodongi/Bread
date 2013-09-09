
#include "stdafx.h"
#include "Shader.h"
#include "BreadMaterial.h"
#include "BreadCameraManager.h"
#include "BreadTextureManager.h"
#include "BreadEntityModel.h"
#include "SampleApp.h"
#include "BreadMatrix3.h"
#include "BreadLightManager.h"
#include "BreadShadowMap.h"
#include "BreadShadowVolume.h"
#include "BreadProjectedShadow.h"
#include "BreadThreadBase.h"

/*
*	
*/
void cNormalShader::setMaterialParam(Bread::cMaterial const* mtl)
{
	setTechnique(_T("T0"));

	setFloat(_T("p"), 2);

	cTexture* tex = _getTextureMgr()->get(mtl->m_mapBuid[cMaterial::DIFFUSE]);
	setFloat(_T("mapW"), (float)tex->getWidth());
	setFloat(_T("mapH"), (float)tex->getHeight());

	setTexture(_T("baseTexture"), tex);
}
/*
*	
*/
void cNormalShader::setMeshParam(Bread::cMesh const* mesh, Bread::cEntityModel const* entity)
{
	unreferencedParameter(mesh);

	Bread::sVector3 const& pos = entity->getPosition();
	D3DXMATRIXA16 matWorld	;
	D3DXMatrixTranslation(&matWorld, pos.x, pos.y, pos.z);
	setMatrix(_T("matWorld"), &matWorld);

	D3DXMATRIXA16 matWVP;
	cCamera* cam = _getCameraMgr()->getMain();
	matWVP = matWorld * cam->getView() * cam->getProj();
	setMatrix(_T("matWVP"), &matWVP);
	setMatrix(_T("matView"), &cam->getView());

	//m_effect->CommitChanges();
}
/*
 *	
 */
void cNormalShader::registHandle()
{
	insertTechnique(_T("T0"));
	insertParameter(_T("matWorld"));
	insertParameter(_T("matView"));
	insertParameter(_T("matWVP"));
	insertParameter(_T("p"));
	insertParameter(_T("mapW"));
	insertParameter(_T("mapH"));
	insertParameter(_T("baseTexture"));
}






/*
*	
*/
void cReflectionShader::setMaterialParam(Bread::cMaterial const* mtl)
{
	setTechnique(_T("T0"));

	cCamera* cam = _getCameraMgr()->getMain();
	D3DXVECTOR4 eyePos = D3DXVECTOR4(cam->getEye().x, cam->getEye().y, cam->getEye().z, 1.0f);
	setVector(_T("eyePos"), &eyePos);

	cTexture* tex;
	tex = _getTextureMgr()->getTexture(mtl->m_mapBuid[Bread::cMaterial::DIFFUSE]);
	setTexture(_T("baseTexture"), tex);

	tex = _getTextureMgr()->getTexture(_T("LobbyCube.dds"));
	if (tex)
		setTexture(_T("envTexture"), tex);
	else
		_getTextureMgr()->loadTexture(_T("data\\LobbyCube.dds"), TEXTURE_TYPE_2D, LOADING_DIRECT); 
}
/*
*	
*/
void cReflectionShader::setMeshParam(Bread::cMesh const* mesh, Bread::cEntityModel const* entity)
{
	unreferencedParameter(mesh);

	Bread::sVector3 const& pos = entity->getPosition();
	D3DXMATRIXA16 matWorld	;
	D3DXMatrixTranslation(&matWorld, pos.x, pos.y, pos.z);

	D3DXMATRIXA16 matWVP;
	cCamera* cam = _getCameraMgr()->getMain();
	matWVP = matWorld * cam->getView() * cam->getProj();
	setMatrix(_T("matWVP"), &matWVP);

	setMatrix(_T("matWorld"), &matWorld);
	setMatrix(_T("matView"), &cam->getView());
	setMatrix(_T("matProj"), &cam->getProj());

	//m_effect->CommitChanges();
}
/*
 *	
 */
void cReflectionShader::registHandle()
{
	insertTechnique(_T("T0"));
	insertParameter(_T("baseTexture"));
	insertParameter(_T("envTexture"));
	insertParameter(_T("matWorld"));
	insertParameter(_T("matView"));
	insertParameter(_T("matProj"));
	insertParameter(_T("matWVP"));
	insertParameter(_T("eyePos"));
}












/*
 *	
 */
void cEdgeShader::setMaterialParam(Bread::cMaterial const* /*mtl*/)
{
}
/*
 *	
 */
void cEdgeShader::setMeshParam(Bread::cMesh const* /*mesh*/, Bread::cEntityModel const* /*entity*/)
{
}
/*
 *	
 */
void cEdgeShader::registHandle()
{
	insertTechnique(_T("T0"));
	insertParameter(_T("baseTexture"));
}








/*
 *	
 */
void cEdgeIdShader::setMaterialParam(Bread::cMaterial const* /*mtl*/)
{
}
/*
 *	
 */
void cEdgeIdShader::setMeshParam(Bread::cMesh const* /*mesh*/, Bread::cEntityModel const* entity)
{
	Bread::sVector3 const& pos = entity->getPosition();
	D3DXMATRIXA16 matWorld	;
	D3DXMatrixTranslation(&matWorld, pos.x, pos.y, pos.z);

	D3DXMATRIXA16 matWVP;
	cCamera* cam = _getCameraMgr()->getMain();
	matWVP = matWorld * cam->getView() * cam->getProj();
	setMatrix(_T("matWVP"), &matWVP);
	setFloat(_T("edgeId"), 1.0f);
}
/*
 *	
 */
void cEdgeIdShader::registHandle()
{
	insertTechnique(_T("T0"));
	insertParameter(_T("matWVP"));
	insertParameter(_T("edgeId"));
}









/*
*	
*/
void cPmShader::setMaterialParam(Bread::cMaterial const* mtl)
{
	setTechnique(_T("T0"));

	cCamera* cam = _getCameraMgr()->getMain();
	D3DXVECTOR4 eyePos(cam->getEye().x, cam->getEye().y, cam->getEye().z, 0.0f);
	D3DXVECTOR3 lightDir(-1.0f, 0.0f, -1.0f);
	setValue(_T("g_vEye"), (void*)&eyePos, sizeof (D3DXVECTOR4));
	setValue(_T("g_LightDir"), (void*)&lightDir, sizeof (D3DXVECTOR3));


	cTexture* tex;
	tex = _getTextureMgr()->get(mtl->m_mapBuid[cMaterial::DIFFUSE]);
	setTexture(_T("baseTexture"), tex);

	tex = _getTextureMgr()->get(mtl->m_mapBuid[cMaterial::BUMP]);
	setTexture(_T("normalTexture"), tex);

	setFloat(_T("g_fHeightMapScale"), 0.0f);
}
/*
*	
*/
void cPmShader::setMeshParam(Bread::cMesh const* mesh, Bread::cEntityModel const* entity)
{
	unreferencedParameter(mesh);

	Bread::sVector3 const& pos = entity->getPosition();
	D3DXMATRIXA16 matWorld	;
	D3DXMatrixTranslation(&matWorld, pos.x, pos.y, pos.z);
	setMatrix(_T("g_mWorld"), &matWorld);

	D3DXMATRIXA16 matWVP;
	cCamera* cam = _getCameraMgr()->getMain();
	matWVP = matWorld * cam->getView() * cam->getProj();
	setMatrix(_T("g_mWorldViewProjection"), &matWVP);

	setMatrix(_T("g_mView"), &(cam->getView()));
}
/*
 *	
 */
void cPmShader::registHandle()
{
	insertTechnique(_T("T0"));
	insertParameter(_T("g_LightDir"));
	insertParameter(_T("g_vEye"));
	insertParameter(_T("baseTexture"));
	insertParameter(_T("normalTexture"));
	insertParameter(_T("g_mWorld"));
	insertParameter(_T("g_mWorldViewProjection"));
	insertParameter(_T("g_mView"));
	insertParameter(_T("g_fHeightMapScale"));
}





/*
 *	
 */
void cHDRShader::setMaterialParam(cMaterial const* mtl)
{
	unreferencedParameter(mtl);
}
/*
 *	
 */
void cHDRShader::setMeshParam(cMesh const* mesh, cEntityModel const* entity)
{	
	unreferencedParameter(mesh);
	unreferencedParameter(entity);
}
/*
 *	
 */
void cHDRShader::registHandle()
{
	insertTechnique(_T("TLuminance1"));
	insertTechnique(_T("TLuminance2"));
	insertTechnique(_T("TBrightPass"));
	insertTechnique(_T("TDownSample"));
	insertTechnique(_T("TBlurHorizontal"));
	insertTechnique(_T("TBlurVertical"));
	insertTechnique(_T("THDRFinal"));
	insertParameter(_T("tcLumOffsets"));
	insertParameter(_T("tcDSOffsets"));
	insertParameter(_T("fBrightPassThreshold"));
	insertParameter(_T("tcDownSampleOffsets"));
	insertParameter(_T("HBloomWeights"));
	insertParameter(_T("HBloomOffsets"));
	insertParameter(_T("VBloomWeights"));
	insertParameter(_T("VBloomOffsets"));
	insertParameter(_T("baseTexture"));
	insertParameter(_T("originalTexture"));
	insertParameter(_T("lumTexture"));
	insertParameter(_T("bloomTexture"));
	insertParameter(_T("fExposure"));
	insertParameter(_T("fGaussianScalar"));
	insertParameter(_T("g_rcp_bloom_tex_w"));
	insertParameter(_T("g_rcp_bloom_tex_h"));
}



/*
 *	
 */
void cDeferredShader::setMaterialParam(Bread::cMaterial const* mtl)
{
	setTexture(_T("c_tDiffuseMap"), _getTextureMgr()->getTexture(mtl->m_filename[cMaterial::DIFFUSE]));

	D3DXVECTOR4 v;
	v.w = 1.0f;
//	mtl->m_ambient.toD3D(v);
//	setVector(_T("c_vMaterialAmbient"), &v);
	mtl->m_diffuse.to3(v);
	setVector(_T("c_vMaterialDiffuse"), &v);
	float power = 0.5f;
	setFloat(_T("c_fSpecularPower"), power);
}
/*
 *	
 */
void cDeferredShader::setMeshParam(Bread::cMesh const* mesh, Bread::cEntityModel const* entity)
{
	unreferencedParameter(mesh);

	Bread::sVector3 const& pos = entity->getPosition();
	D3DXMATRIXA16 matWorld	;
	D3DXMatrixTranslation(&matWorld, pos.x, pos.y, pos.z);
	setMatrix(_T("c_mWorld"), &matWorld);
}
/*
 *	
 */
void cDeferredShader::registHandle()
{
	insertTechnique(_T("TBuild"));
	insertTechnique(_T("TDirectionalLight"));
	insertTechnique(_T("TPointLight"));
	insertParameter(_T("c_mWorld"));
	insertParameter(_T("c_mViewInverse"));
	insertParameter(_T("c_mViewProjection"));
	insertParameter(_T("c_vLightDir"));
	insertParameter(_T("c_vLightPos"));
	insertParameter(_T("c_vLightDiffuse"));
	insertParameter(_T("c_vLightSpecular"));
	insertParameter(_T("c_vMaterialDiffuse"));
	insertParameter(_T("c_vMaterialSpecular"));
	insertParameter(_T("c_fSpecularPower"));
	insertParameter(_T("c_tDiffuseMap"));
	insertParameter(_T("c_tSceneMaterialMap"));
	insertParameter(_T("c_tSceneNormalMap"));
	insertParameter(_T("c_tScenePositionXYMap"));
	insertParameter(_T("c_tScenePositionZMap"));
	insertParameter(_T("range"));
}





/*
 *	
 */
void cMakeNormalShader::registHandle()
{
	insertTechnique(_T("T0"));
}







/*
 *
 */
void cDeferredSingleShader::setMaterialParam(cMaterial const* mtl)
{
	unreferencedParameter(mtl);
}
/*
 *
 */
void cDeferredSingleShader::setMeshParam(cMesh const* mesh, cEntityModel const* entity)
{
	unreferencedParameter(mesh);

	cCamera* cam = _getCameraMgr()->getMain();

	sMatrix4 matWorld, matWV;
	entity->calcuMatWorld(matWorld);
	mulMat4(matWorld, cam->getView(), matWV);
	setMatrix(_T("matWorldView"), &matWV);

	sMatrix4 matWVP;
	mulMat4(matWorld, cam->getVP(), matWVP);
	setMatrix(_T("matViewProjection"), &matWVP);
}
/*
 *
 */
void cDeferredSingleShader::registHandle()
{
	insertTechnique(_T("TGeom"));
	insertTechnique(_T("TDirectionLight"));
	insertTechnique(_T("TPointLight"));
	insertTechnique(_T("TFinal"));

	insertParameter(_T("matViewProjection"));
	insertParameter(_T("matWorldView"));
	insertParameter(_T("viewAspect"));
	insertParameter(_T("tanFOV"));
	insertParameter(_T("lightPos"));
	insertParameter(_T("geomTexture"));
	insertParameter(_T("normalTexture"));
	insertParameter(_T("sceneTexture"));
	insertParameter(_T("lightTexture"));
	insertParameter(_T("invSqrLightRange"));
	insertParameter(_T("lightColor"));
	insertParameter(_T("lightRange"));
}








/*
 *
 */
void cNoTextureShader::setMaterialParam(cMaterial const* /*mtl*/)
{
}
/*
 *
 */
void cNoTextureShader::setMeshParam(cMesh const* mesh, cEntityModel const* entity)
{
	unreferencedParameter(mesh);

	sMatrix4 matWorld, matWVP;
	entity->calcuMatWorld(matWorld);
	setMatrix(_T("matWorld"), &matWorld);
	
	cCamera const* mainCam = _getCameraMgr()->getMain();
	mulMat4(matWorld, mainCam->getVP(), matWVP);
	setMatrix(_T("matWVP"), &matWVP);
}
/*
 *
 */
void cNoTextureShader::registHandle()
{
	insertTechnique(_T("T0"));
	insertParameter(_T("matWorld"));
	insertParameter(_T("matWVP"));
	insertParameter(_T("color"));
}







/*
 *
 */
void cVolumetricLightShader::registHandle()
{
	insertTechnique(_T("TSampling"));
	insertTechnique(_T("TFinal"));
	insertParameter(_T("screenLightPos"));
	insertParameter(_T("density"));
	insertParameter(_T("weight"));
	insertParameter(_T("exposure"));
	insertParameter(_T("decay"));
	insertParameter(_T("baseTexture"));
	insertParameter(_T("samplingTexture"));
}















/*
 *	
 */
void cShadowVolumeShader::setMaterialParam(cMaterial const* mtl)
{
	D3DXVECTOR4 diffuse;
	mtl->m_diffuse.to3(diffuse);
	setVector(m_matColor, &diffuse);

	cTexture* tex = _getTextureMgr()->getTexture(mtl->m_mapBuid[Bread::cMaterial::DIFFUSE]);
	setTexture(m_txScene, tex);
}
/*
 *	
 */
void cShadowVolumeShader::setMeshParam(cMesh const* /*mesh*/, cEntityModel const* entity)
{
	unreferencedParameter(entity);

	cCamera* cam = _getCameraMgr()->getMain();
	D3DXVECTOR3 pos;
	entity->getPosition().to3(pos);
	D3DXMATRIXA16 matWorld, matIW, matWV, matWVP;
	D3DXMatrixTranslation(&matWorld, pos.x, pos.y, pos.z);
	D3DXMatrixInverse(&matIW, NULL, &matWorld);
	matWV = matWorld * cam->getView();
	matWVP = matWV * cam->getProj();

	setMatrix(m_matIW, &matIW);
	setMatrix(m_matWV, &matWV);
	setMatrix(m_matProj, &cam->getProj());
	setMatrix(m_worldViewProjection, &matWVP);
	setMatrix(m_worldView, &matWV);

	cLight* light = _getLightMgr()->get(0);
	D3DXVECTOR4 lightPos;
	light->getPosition().to3(lightPos);
	D3DXVec4Transform(&lightPos, &lightPos, &matWV);
	setVector(m_lightView, &lightPos);
}
/*
 *	
 */
void cShadowVolumeShader::registHandle()
{
	m_renderSceneAmbient		= insertTechnique(_T("RenderSceneAmbient"));
	m_showShadowVolume			= insertTechnique(_T("ShowShadowVolume"));
	m_showShadowVolume2Sided	= insertTechnique(_T("ShowShadowVolume2Sided"));
	m_renderShadowVolume		= insertTechnique(_T("RenderShadowVolume"));
	m_renderShadowVolume2Sided	= insertTechnique(_T("RenderShadowVolume2Sided"));
	m_renderShadowVolumeComplexity = insertTechnique(_T("RenderShadowVolumeComplexity"));
	m_renderScene				= insertTechnique(_T("RenderScene"));
	m_renderDirtyStencil		= insertTechnique(_T("RenderDirtyStencil"));
	m_renderComplexity			= insertTechnique(_T("RenderComplexity"));
	m_ambient					= insertParameter(_T("g_vAmbient"));
	m_lightView					= insertParameter(_T("g_vLightView"));
	m_lightColor				= insertParameter(_T("g_vLightColor"));
	m_shadowColor				= insertParameter(_T("g_vShadowColor"));
	m_matColor					= insertParameter(_T("g_vMatColor"));
	m_worldView					= insertParameter(_T("g_mWorldView"));
	m_proj						= insertParameter(_T("g_mProj"));
	m_worldViewProjection		= insertParameter(_T("g_mWorldViewProjection"));
	m_txScene					= insertParameter(_T("g_txScene"));
	m_farClip					= insertParameter(_T("g_fFarClip"));
	m_matIW						= insertParameter(_T("matIW"));
	m_matWV						= insertParameter(_T("matWV"));
	m_matProj					= insertParameter(_T("matProj"));
	m_lightPos					= insertParameter(_T("vLightPos"));
}







/*
 *	
 */
void cGammaCorrectionShader::registHandle()
{
	m_tluminance = insertTechnique(_T("TLuminance"));
	m_tcorrection = insertTechnique(_T("TCorrection"));
	m_baseTex = insertParameter(_T("baseTexture"));
	m_gammaRampTex = insertParameter(_T("gammaRampTexture"));
}




/*
 *	
 */
void cSkinShader::registHandle()
{
	insertTechnique(_T("TBeckmann"));
	insertTechnique(_T("TLightMap"));
	insertParameter(_T("lightDir"));
	insertParameter(_T("normalTexture"));
}