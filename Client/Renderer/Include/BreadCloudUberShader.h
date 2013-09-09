
#ifndef _BreadCloudUberShader_h_
#define _BreadCloudUberShader_h_

#include "BreadUberShader.h"
#include "BreadVector2.h"
#include "BreadVector4.h"

namespace Bread
{
	/*
		@date 2012
		@auth prodongi
		@desc 
		@todo 
	*/
	class cCloudUberShader : public cUberShader
	{
	public:
		cCloudUberShader();
		virtual ~cCloudUberShader();
		virtual void update(float elapsedtime, cMesh* /*mesh*/, cMaterial* /*mtl*/, cEntityModel* /*entity*/);
		virtual void setMaterialParam(cMaterial const* mtl);
		virtual void setMeshParam(cMesh const* mesh, cEntityModel const* entity);
		virtual void beginRenderProperty();
		virtual void endRenderProperty();
		
	protected:
		virtual bool initialize();
		virtual void registHandle();
		void setDefaultValue();

	protected:
		/*
		sVector3 m_lightPos;
		float m_blend;
		float m_noiseScale1;
		float m_noiseScale2;
		float m_implictBlend;
		*/
	};

	/*
	*/
	cUberShader* createCloudUberShader();
}



#endif