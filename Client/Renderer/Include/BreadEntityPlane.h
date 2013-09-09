
#ifndef _BreadEntityPlane_h_
#define _BreadEntityPlane_h_

#include "BreadEntityMesh.h"

namespace Bread
{
	/*
		@date 2011.07.11
		@auth prodongi
		@desc 
		@todo 
	*/
	class cEntityPlane : public cEntityMesh
	{
		declareRTTI()

	public:
		cEntityPlane();
		virtual ~cEntityPlane();
		virtual bool createMesh(int width, int height, int  segmentX, int segmentY, bool uvWrap, 
									 std_string const& uberShaderType, std_string const& uberFragments,
									 std_string const& texFileName, bool isSceneObject, bool addOctree = true);
	};
}



#endif