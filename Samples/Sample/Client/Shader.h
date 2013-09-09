
#ifndef _Shader_h_
#define _Shader_h_

#include "BreadShader.h"

/**
 * \ingroup Sample
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
 * \date 2009-11-20
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

/*
*	
*/
class cNormalShader : public cShader
{
public:
	cNormalShader() {}
	virtual ~cNormalShader() {}
	virtual void setMaterialParam(cMaterial const* mtl);
	virtual void setMeshParam(cMesh const* mesh, cEntityModel const* entity);
	virtual void registHandle();
};
/*
*	
*/
class cReflectionShader : public cShader
{
public:
	cReflectionShader() {}
	virtual ~cReflectionShader() {}
	virtual void setMaterialParam(cMaterial const* mtl);
	virtual void setMeshParam(cMesh const* mesh, cEntityModel const* entity);
	virtual void registHandle();
};
/*
*	
*/
class cEdgeShader : public cShader
{
public:
	cEdgeShader() {}
	virtual ~cEdgeShader() {}
	virtual void setMaterialParam(cMaterial const* mtl);
	virtual void setMeshParam(cMesh const* mesh, cEntityModel const* entity);
	virtual void registHandle();
};
/*
 *	
 */
class cEdgeIdShader : public cShader
{
public:
	cEdgeIdShader() {}
	virtual ~cEdgeIdShader() {}
	virtual void setMaterialParam(cMaterial const* mtl);
	virtual void setMeshParam(cMesh const* mesh, cEntityModel const* entity);
	virtual void registHandle();
};
/*
 *	
 */
class cPmShader : public cShader
{
public:
	cPmShader() {}
	virtual ~cPmShader() {}
	virtual void setMaterialParam(cMaterial const* mtl);
	virtual void setMeshParam(cMesh const* mesh, cEntityModel const* entity);
	virtual void registHandle();
};
/*
 *	
 */
class cHDRShader : public cShader
{
public:
	cHDRShader() {}
	virtual ~cHDRShader() {}
	virtual void setMaterialParam(cMaterial const* mtl);
	virtual void setMeshParam(cMesh const* mesh, cEntityModel const* entity);
	virtual void registHandle();
};
/*
 *	
 */
class cDeferredShader : public cShader
{
public:
	cDeferredShader() {}
	virtual ~cDeferredShader() {}
	virtual void setMaterialParam(cMaterial const* mtl);
	virtual void setMeshParam(cMesh const* mesh, cEntityModel const* entity);
	virtual void registHandle();
};
/*
 *
 */
class cMakeNormalShader : public cShader
{
public:
	cMakeNormalShader() {}
	virtual ~cMakeNormalShader() {}
	virtual void setMaterialParam(cMaterial const* /*mtl*/) {}
	virtual void setMeshParam(cMesh const* /*mesh*/, cEntityModel const* /*entity*/) {}
	virtual void registHandle();
};
/*
 *
 */
class cDeferredSingleShader : public cShader
{
public:
	cDeferredSingleShader() {}
	virtual ~cDeferredSingleShader() {}
	virtual void setMaterialParam(cMaterial const* mtl);
	virtual void setMeshParam(cMesh const* mesh, cEntityModel const* entity);
	virtual void registHandle();
};
/*
 *
 */
class cNoTextureShader : public cShader
{
public:
	cNoTextureShader() {}
	~cNoTextureShader() {}
	virtual void setMaterialParam(cMaterial const* mtl);
	virtual void setMeshParam(cMesh const* mesh, cEntityModel const* entity);
	virtual void registHandle();
};
/*
 *	
 */
class cVolumetricLightShader : public cShader
{
public:
	cVolumetricLightShader() {}
	~cVolumetricLightShader() {}
	virtual void setMaterialParam(cMaterial const* /*mtl*/) {}
	virtual void setMeshParam(cMesh const* /*mesh*/, cEntityModel const* /*entity*/) {}
	virtual void registHandle();
};
/*
 * DirectX Sample ÂüÁ¶
 */
class cShadowVolumeShader : public cShader
{
public:
	cShadowVolumeShader() {}
	~cShadowVolumeShader() {}
	virtual void setMaterialParam(cMaterial const* mtl);
	virtual void setMeshParam(cMesh const* mesh, cEntityModel const* entity);
	virtual void registHandle();

public:
	D3DXHANDLE m_renderSceneAmbient;
	D3DXHANDLE m_showShadowVolume;			
	D3DXHANDLE m_showShadowVolume2Sided;	
	D3DXHANDLE m_renderShadowVolume;		
	D3DXHANDLE m_renderShadowVolume2Sided;	
	D3DXHANDLE m_renderShadowVolumeComplexity;
	D3DXHANDLE m_renderScene;
	D3DXHANDLE m_renderDirtyStencil;		
	D3DXHANDLE m_renderComplexity;			
	D3DXHANDLE m_ambient;					
	D3DXHANDLE m_lightView;					
	D3DXHANDLE m_lightColor;				
	D3DXHANDLE m_shadowColor;				
	D3DXHANDLE m_matColor;					
	D3DXHANDLE m_worldView;					
	D3DXHANDLE m_proj;						
	D3DXHANDLE m_worldViewProjection;		
	D3DXHANDLE m_txScene;					
	D3DXHANDLE m_farClip;
	D3DXHANDLE m_matIW;
	D3DXHANDLE m_matWV;
	D3DXHANDLE m_matProj;
	D3DXHANDLE m_lightPos;
};
/*
 *	
 */
class cGammaCorrectionShader : public cShader
{
public:
	cGammaCorrectionShader() {}
	~cGammaCorrectionShader() {}
	virtual void registHandle();

public:
	D3DXHANDLE m_tluminance;
	D3DXHANDLE m_tcorrection;
	D3DXHANDLE m_baseTex;
	D3DXHANDLE m_gammaRampTex;
};
/*
 *	
 */
class cSkinShader : public cShader
{
public:
	cSkinShader() {}
	~cSkinShader() {}
	virtual void registHandle();
};


#endif
















