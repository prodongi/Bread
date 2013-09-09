
#ifndef _BreadShaderList_h_
#define _BreadShaderList_h_

#include "BreadShader.h"
#include "BreadMatrix4.h"

namespace Bread
{
	/*
	*	
	*/
	class cProjectedShadowShader : public cShader
	{
	public:
		cProjectedShadowShader() {}
		virtual ~cProjectedShadowShader() {}
		virtual void setMaterialParam(cMaterial const* mtl);
		virtual void setMeshParam(cMesh const* mesh, cEntityModel const* entity);
		virtual void registHandle();

	public:
		D3DXHANDLE m_hTProjShadow;
		D3DXHANDLE m_hMatWVP;
	};
	/*
	*	
	*/
	class cShadowMapDepthShader : public cShader
	{
	public:
		cShadowMapDepthShader() {}
		virtual ~cShadowMapDepthShader() {}
		virtual void setMeshParam(cMesh const* mesh, cEntityModel const* entity);
		virtual void registHandle();

	public:
		D3DXHANDLE m_hTDepth;
		D3DXHANDLE m_hMatWLP;
		D3DXHANDLE m_hDepthBias;
	};
	/*
	*/
	class cShadowPSSMShader : public cShader
	{
	public:
		cShadowPSSMShader() {}
		virtual ~cShadowPSSMShader() {}
		virtual void setMeshParam(cMesh const* mesh, cEntityModel const* entity);
		virtual void registHandle();

	public:
		D3DXHANDLE m_t;
		D3DXHANDLE m_matVP;
	};
	/*
	*/
	class cDepthTextureShader : public cShader
	{
	public:
		cDepthTextureShader() {}
		virtual ~cDepthTextureShader() {}
		virtual void setMeshParam(cMesh const* mesh, cEntityModel const* entity);
		virtual void registHandle();

	public:
		D3DXHANDLE m_t;
		D3DXHANDLE m_matWVP;
		D3DXHANDLE m_farClip;
	};
	/*
	*/
	class cSSAOShader : public cShader
	{
	public:
		cSSAOShader() {}
		virtual ~cSSAOShader() {}
		virtual void registHandle();

	public:
		D3DXHANDLE m_t;
		D3DXHANDLE m_depthTexture;
		D3DXHANDLE m_rotTexture;
		D3DXHANDLE m_farDistance;
	};
	/*
	*/
	class cLightDepthMapShader : public cShader
	{
	public:
		cLightDepthMapShader() {}
		virtual ~cLightDepthMapShader() {}
		virtual void setMeshParam(cMesh const* mesh, cEntityModel const* entity);
		virtual void registHandle();
		void setMatLP(sMatrix4 const& mat)	{	m_matLP = mat;	}

	public:
		D3DXHANDLE m_t;
		D3DXHANDLE m_matWLP;
		D3DXHANDLE m_matSkin;
		D3DXHANDLE m_skinLinkNum;
		D3DXHANDLE m_isAni;
	protected:
		sMatrix4 m_matLP;
	};
	/*
	*/
	class cNoTextureShader : public cShader
	{
	public:
		cNoTextureShader() {}
		virtual ~cNoTextureShader() {}
		virtual void setMeshParam(cMesh const* mesh, cEntityModel const* entity);
		virtual void registHandle();

	public:
		D3DXHANDLE m_t;
		D3DXHANDLE m_matWVP;
		D3DXHANDLE m_color;
	};
	/*
	*/
	class cWeatherShader : public cShader
	{
	public:
		cWeatherShader() {}
		virtual ~cWeatherShader() {}
		virtual void setMeshParam(cMesh const* mesh, cEntityModel const* entity);
		virtual void registHandle();

	public:
		D3DXHANDLE m_t;
		D3DXHANDLE m_matWVP;
		D3DXHANDLE m_matWVPPrev;
		D3DXHANDLE m_positionOffset;
		D3DXHANDLE m_velocity;
		D3DXHANDLE m_alpha;
		D3DXHANDLE m_viewPosition;
		D3DXHANDLE m_sizeScale;
		D3DXHANDLE m_lighting;
		D3DXHANDLE m_forward;
		D3DXHANDLE m_diffuseTexture;
	};
	/*
	*/
	class cSkyBoxShader : public cShader
	{
	public:
		cSkyBoxShader() {}
		virtual ~cSkyBoxShader() {}
		virtual void setMeshParam(cMesh const* mesh, cEntityModel const* entity);
		virtual void registHandle();

	public:
		D3DXHANDLE m_t;
		D3DXHANDLE m_matWorld;
		D3DXHANDLE m_matVP;
		D3DXHANDLE m_tex;
	};
}




#endif