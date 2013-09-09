
#ifndef _BreadEntityMesh_h_
#define _BreadEntityMesh_h_

#include "BreadEntityModel.h"

namespace Bread
{
	struct sCullFigure;

	/*
		@date 2011.07.28
		@auth prodongi
		@desc initialize()를 호출해서 m_buid를 설정한 후에 각 서브 폴더의 createMesh를 호출 해야 된다.
				 Render Data 만 필요한 경우에도 쓰임
		@todo 
	*/
	class cEntityMesh : public cEntityModel
	{
		declareRTTI()

	public:
		cEntityMesh();
		virtual ~cEntityMesh();
		virtual void initialize(sVector3 const& pos, float radian, BUID const& renderDataBuid, BUID const& animationBuid, bool isSceneObject, bool isSyncHeight = true);
		virtual void initialize(BUID const& buid, sVector3 const& pos, float radian, BUID const& renderDataBuid, BUID const& animationBuid, bool isSceneObject, bool isSyncHeight = true);
		void createRenderData(sCullFigure const* cullFigure, std_string const& texFileName, BUID const& shaderBuid, bool isSceneObject, bool addOctree = true);
	};
}



#endif