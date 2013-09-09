
#ifndef _BreadSerialize3D_h_
#define _BreadSerialize3D_h_

#include "BreadSerializeBase.h"
#include "BreadSerializeBfx.h"
#include "BreadVector3.h"
#include "BreadQuaternion.h"

namespace Bread
{
	struct sJointRef;
	struct sFVFInfo;
	struct sVBInfo;
	struct sVBCacheInfo;
	struct sCullFigure;
	struct sLocalJointIndex;
	class cAnimation;
	class cTerrain;
	class cRenderData;
	class cRenderCacheData;
	class cMaterial;
	class cMesh;
	class cCacheMesh;
	class cOctree;

	/**
	 * \ingroup Renderer
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
	 * \date 2009-09-27
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
	class cSerialize3D : public cSerializeBase
	{
	public:
		cSerialize3D();
		cSerialize3D(const cSerialize3D& /*serialize*/) {}
		virtual ~cSerialize3D();
		static void clearBuffer();
		static TCHAR const* makeBUID(std_string const& filename);

	protected:
		void readVersion();
		void readCullFigure(sCullFigure* cullFigure);
		void readMaterialList(cRenderData* rd);
		void readMaterialList(cRenderCacheData* rd);
		bool readMeshList(cRenderData* rd);
		bool readMeshList(cRenderCacheData* rd);
		void readMaterial(cMaterial* mtl);
		bool readMesh(cMesh* mesh);
		bool readMesh(cCacheMesh* mesh);
		void readOctree(cOctree* octree);
		void readRenderData(cRenderData* rd);
		void readRenderData(cRenderCacheData* rd);
		void readAnimation(cAnimation** aniData);
		void readJointRef(sJointRef* jointRef, int jointNum);
		void readLocalJointIndex(sLocalJointIndex* localJointIndex);
		bool readFVFInfo(sVBInfo* vbInfo);
		bool readFVFInfo(sVBCacheInfo* vbInfo);
		void readVector3(sVector3& value);
		void readQuaternion(sQuaternion& value);
		void readTerrain(cTerrain* terrain);


		/// @brief newBufferSize가 m_bufferSize보다 크면 버퍼를 재할당해준다
		static void checkReallocBuffer(int newBufferSize);

	private:
		void loadBfx();

	protected:
		bool m_isAni;
		static char* m_buffer;
		static int m_bufferSize;

		cSerializeBfx	m_serializeBfx;
	};
}










#endif