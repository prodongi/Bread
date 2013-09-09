
#ifndef _BreadVolumeCloud_h_
#define _BreadVolumeCloud_h_

#include "BreadClientDefinition.h"

namespace Bread
{
	class cEntityMesh;
	class cCloudUberShader;

	/*
		@date 2012.
		@auth prodongi
		@desc 
		@todo 
	*/
	class cVolumeCloud
	{
	public:
		cVolumeCloud();
		virtual ~cVolumeCloud();
		virtual bool initialize();
		virtual void finalize();
		virtual void render();

		propertyConstFunc(bool, IsRender, m_isRender)

	protected:
		bool createEntity();
		bool createTexture();

	protected:
		cEntityMesh* m_entity;
		cCloudUberShader* m_shader;
		BUID m_texBuid;
		bool m_isRender;
	};
}



#endif