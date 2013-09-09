
#ifndef _BreadEntityMesh_h_
#define _BreadEntityMesh_h_

#include "BreadEntityModel.h"

namespace Bread
{
	struct sCullFigure;

	/*
		@date 2011.07.28
		@auth prodongi
		@desc initialize()�� ȣ���ؼ� m_buid�� ������ �Ŀ� �� ���� ������ createMesh�� ȣ�� �ؾ� �ȴ�.
				 Render Data �� �ʿ��� ��쿡�� ����
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