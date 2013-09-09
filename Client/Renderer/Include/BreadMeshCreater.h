
#ifndef _BreadMeshCreater_h_
#define _BreadMeshCreater_h_

#include "BreadClientDefinition.h"
#include "BreadD3DHeader.h"
#include <assert.h>

#define _getMeshCreater()		Bread::cMeshCreater::getInstance()

namespace Bread
{
	struct sVBInfo;
	struct sIBInfo;

	/*
		@date 2011.11.24
		@auth prodongi
		@desc 
		@todo 
		*/
	class cMeshCreater
	{
		declareInstance(cMeshCreater)
		declareClassCreater(cMeshCreater)

	public:
		/// @brief plane mesh 积己
		bool createPlane(BUID const& buid, int width, int height, int segmentX, int segmentZ, bool uvWrap);
		/// @brief weater俊 静捞绰 rain/snow particles 积己
		bool createWeatherParticle(BUID const& buid, int particleNum, float boxSize);
		/// @brief skybox mesh 积己
		bool createSkyBox(BUID const& buid);
		bool createVolumeCloud(BUID const& buid, float width, float height, int sliceNum);

	protected:
		sVBInfo* createVBInfo(BUID const& buid, bool dynamic, int vertexNum, D3DPRIMITIVETYPE primitiveType, uint stride, uint vbSize, DWORD fvf);
		sIBInfo* createIBInfo(BUID const& buid, uint ibSize, uint faceNum);

		int getIbSize(int faceNum, D3DPRIMITIVETYPE primitiveType) const;
	};
}



#endif
