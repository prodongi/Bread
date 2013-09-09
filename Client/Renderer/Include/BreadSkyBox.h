
#ifndef _BreadSkyBox_h_
#define _BreadSkyBox_h_

#include "BreadClientDefinition.h"

namespace Bread
{
	class cEntityMesh;
	class cSkyBoxShader;

	/*
	*/
	class cSkyBox
	{
	public:
		cSkyBox();
		virtual ~cSkyBox();
		virtual bool initialize(int type);
		virtual void finalize();
		virtual void render();

		propertyConstFunc(bool, IsRender, m_isRender)

	protected:
		bool createEntity();
		bool createShader();
		void loadTexture();

	public:
		enum TYPE	{	BOX, PLANE, DOME	};
		
	protected:
		cEntityMesh* m_entity;
		cSkyBoxShader* m_shader;
		BUID m_texBuid;
		bool m_isRender;
	};
}







#endif