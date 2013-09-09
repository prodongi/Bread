
#include "BreadSerialize3D.h"
#include "BreadMaterial.h"
#include "BreadMesh.h"
#include "BreadCacheMesh.h"
#include "BreadVBManager.h"
#include "BreadIBManager.h"
#include "BreadUtils.h"
#include "BreadTextureManager.h"
#include "BreadAnimationManager.h"
#include "BreadAnimationMotionSet.h"
#include "BreadAnimationController.h"
#include "BreadD3DSystemClassify.h"
#include "BreadTerrain.h"
#include "BreadTerrainCell.h"
#include "BreadRenderCacheData.h"
#include "BreadRenderData.h"
#include "BreadOctree.h"
#include "BreadFileHelper.h"
#include "BreadInternString.h"

namespace Bread
{
	char* cSerialize3D::m_buffer = NULL;
	int cSerialize3D::m_bufferSize = 0;
	/*
	 *	
	 */
	void cSerialize3D::clearBuffer()
	{
		safeDeleteArray(m_buffer);
	}
	/*
	*	
	*/
	void cSerialize3D::checkReallocBuffer(int newBufferSize)
	{
		if (!m_buffer)
		{
			m_bufferSize = newBufferSize;
			m_buffer = (char*)malloc(m_bufferSize);
		}
		else if (m_bufferSize < newBufferSize)
		{
			m_bufferSize = newBufferSize;
			void* mem = realloc(m_buffer, m_bufferSize);
			if (!mem)
			{
				assert(!mem && "realloc is null");
				return ;
			}
			m_buffer = (char*)mem;
		}
	}
	TCHAR const* cSerialize3D::makeBUID(std_string const& filename)
	{
		return cFileHelper::getFileNameInFullPassT(filename.c_str());
	}

	/*
	 *	
	 */
	cSerialize3D::cSerialize3D() : m_isAni(false)
	{
	}
	/*
	 *	
	 */
	cSerialize3D::~cSerialize3D()
	{
	}
	/*
	 *	
	 */
	void cSerialize3D::readVersion()
	{
		uint version;
		readValue(version);	
	}
	/*
	 *	
	 */
	void cSerialize3D::readCullFigure(sCullFigure* cullFigure)
	{
		readValue(cullFigure->m_sphere);
		readValue(cullFigure->m_box);
		cullFigure->copyToBackup();
	}
	/*
	 *	
	 */
	void cSerialize3D::readMaterialList(cRenderData* rd)
	{
		loadBfx();

		cRenderData::bucket_mtl* mtlList = rd->getMtlList();
		uint mtlNum;
		readValue(mtlNum);
		for (uint i = 0; i < mtlNum; ++i)
		{
			cMaterial* mtl = mtlList->create();
			readMaterial(mtl);
			mtlList->insert(mtl->m_buid, &mtl);
		}
	}
	/*
	 *	
	 */
	void cSerialize3D::readMaterialList(cRenderCacheData* rd)
	{
		loadBfx();

		cRenderData::bucket_mtl* mtlList = rd->getMtlList();
		uint mtlNum;
		readValue(mtlNum);
		for (uint i = 0; i < mtlNum; ++i)
		{
			cMaterial* mtl = mtlList->create();
			readMaterial(mtl);
			mtlList->insert(mtl->m_buid, &mtl);
		}
	}
	/*
	 *	
	 */
	void cSerialize3D::readMaterial(cMaterial* mtl)
	{
		readBUIDW(mtl->m_buid);
		readVector3(mtl->m_diffuse);
		readVector3(mtl->m_specular);
		readVector3(mtl->m_ambient);
		readValue(mtl->m_opacity);
		readValue(mtl->m_glossiness);
		readValue(mtl->m_specularLevel);

		std_string dir;
		_getInternString()->getStr(getDir(), &dir);
		for (uint i = 0; i < cMaterial::MAPNUM; ++i)
		{
			readNameW(mtl->m_filename[i]);
			mtl->m_mapBuid[i] = cTexture::makeBUID(mtl->m_filename[i]);

			// load texture
			if (_tcslen(mtl->m_filename[i]) >= 1)
			{
				static TCHAR pass[FILENAME_MAX];
				_stprintf(pass, _T("%s%s"), dir.c_str(), mtl->m_filename[i]);
				_getTextureMgr()->loadTexture(pass);
			}
		}

		TCHAR meshName[NAME_LEN];
		uint meshNum;
		readValue(meshNum);
		for (uint mn = 0; mn < meshNum; ++mn)
		{
			readNameW(meshName);
			mtl->m_meshList.push_back(meshName);
		}

		//
		mtl->m_shaderBuid = m_serializeBfx.getUberShader(mtl->m_buid);
	}
	/*
	 *	
	 */
	bool cSerialize3D::readMeshList(cRenderData* rd)
	{
		cRenderData::bucket_mesh* meshList = rd->getMeshList();

		uint meshNum;
		readValue(meshNum);
		for (uint i = 0; i < meshNum; ++i)
		{
			cMesh* mesh = new cMesh;
			if (!readMesh(mesh))
				return false;
			meshList->insert(mesh->m_buid, &mesh);
		}
		return true;
	}
	/*
	 *	
	 */
	bool cSerialize3D::readMeshList(cRenderCacheData* rd)
	{
		cRenderCacheData::bucket_mesh* meshList = rd->getMeshList();

		uint meshNum;
		readValue(meshNum);
		for (uint i = 0; i < meshNum; ++i)
		{
			cCacheMesh* mesh = new cCacheMesh;
			if (!readMesh(mesh))
				return false;
			meshList->insert(mesh->m_buid, &mesh);
		}
		return true;
	}
	/*
	 *	
	 */
	bool cSerialize3D::readMesh(cMesh* mesh)
	{
		uint vertexNum, faceNum;
		uint primitiveType;

		readBUIDW(mesh->m_buid);
		readBUIDW(mesh->m_mtlBuid);

		readCullFigure(&mesh->m_cullFigure);

		readValue(vertexNum);
		readValue(faceNum);
		readValue(primitiveType);

		BUID buid;
		std_string fname;
		_getInternString()->getStr(getFileName(), &fname);
		buid = cVBManager::makeBUID(fname.c_str(), mesh->m_buid.c_str(), _getVBMgr()->getCount());
		if (_getVBMgr()->is(buid))
		{
			// 중복되는 데이타를 fseek로 건너 뛰어야 된다
			return true;
		}

		/*
		 *	vb info
		 */
		mesh->m_vbBuid = buid;
		sVBInfo* vbInfo = new sVBInfo;
		vbInfo->m_buid = buid;
		// animation이 있을때는 dynamic으로 설정해준다
		vbInfo->setFlag(m_isAni);
		vbInfo->m_vertexNum = vertexNum;
		vbInfo->m_primitiveType = (D3DPRIMITIVETYPE)primitiveType;
		if (!readFVFInfo(vbInfo))
			return false;

		readValue<uint>(vbInfo->m_stride);

		uint readIndexSize, readVertexSize;
		if (D3DPT_TRIANGLESTRIP == primitiveType)
			readIndexSize = (faceNum + 2) * sizeof (WORD);
		else
			readIndexSize = faceNum * sizeof (WORD) * 3;

		readVertexSize = vbInfo->m_stride * vertexNum;
		cSerialize3D::checkReallocBuffer(max(readVertexSize, readIndexSize));

		if (m_isAni)
		{
			readLocalJointIndex(&vbInfo->m_localJointIndex);
		}

		vbInfo->m_vbSize = readVertexSize;
		readBuffer(m_buffer, readVertexSize);
		if (!_getVBMgr()->create(vbInfo, m_buffer))
			return false;

		/*
		 *	ib info
		 */
		buid = cIBManager::makeBUID(fname.c_str(), mesh->m_buid.c_str(), _getIBMgr()->getCount());
		mesh->m_ibBuid = buid;
		sIBInfo* ibInfo = new sIBInfo;
		ibInfo->m_buid = buid;
		ibInfo->m_ibSize = readIndexSize;
		ibInfo->m_faceNum = faceNum;

		readBuffer(m_buffer, readIndexSize);
		if (!_getIBMgr()->create(ibInfo, m_buffer))
			return false;

		return true;
	}
	/*
	 *	
	 */
	bool cSerialize3D::readMesh(cCacheMesh* mesh)
	{
		uint vertexNum, faceNum;
		uint primitiveType;

		readBUIDW(mesh->m_buid);
		readBUIDW(mesh->m_mtlBuid);

		readCullFigure(&mesh->m_cullFigure);

		readValue(vertexNum);
		readValue(faceNum);
		readValue(primitiveType);

		/*
		 *	vb info
		 */
		mesh->m_vbInfo = new sVBCacheInfo;
		sVBCacheInfo* vbInfo = mesh->m_vbInfo;
		vbInfo->m_buid = mesh->m_buid;
		// animation이 있을때는 dynamic으로 설정해준다
		vbInfo->setFlag(m_isAni);
		vbInfo->m_vertexNum = vertexNum;
		vbInfo->m_primitiveType = (D3DPRIMITIVETYPE)primitiveType;
		if (!readFVFInfo(vbInfo))
			return false;
		readValue<uint>(vbInfo->m_stride);

		if (m_isAni)
		{
			readLocalJointIndex(&vbInfo->m_localJointIndex);
		}

		vbInfo->m_bufferSize = vbInfo->m_stride * vertexNum;
		vbInfo->m_buffer = new char[vbInfo->m_bufferSize];
		readBuffer(vbInfo->m_buffer, vbInfo->m_bufferSize);

		/*
		 *	ib info
		 */
		mesh->m_ibInfo = new sIBCacheInfo;
		sIBCacheInfo* ibInfo = mesh->m_ibInfo;
		ibInfo->m_buid = mesh->m_buid;
		ibInfo->m_faceNum = faceNum;
		if (D3DPT_TRIANGLESTRIP == primitiveType)	ibInfo->m_bufferSize = (faceNum + 2) * sizeof (WORD);
		else										ibInfo->m_bufferSize = faceNum * sizeof (WORD) * 3;
		ibInfo->m_buffer = new char[ibInfo->m_bufferSize];
		readBuffer(ibInfo->m_buffer, ibInfo->m_bufferSize);

		return true;
	}
	/*
	 *	
	 */
	void cSerialize3D::readOctree(cOctree* octree)
	{
		octree->read(this);
	}
	/*
	 *	
	 */
	void cSerialize3D::readRenderData(cRenderData* rd)
	{
		readBUIDW(rd->m_buid);
		readOctree(&rd->m_octree);
		readCullFigure(&rd->m_cullFigure);
		readMaterialList(rd);
		readMeshList(rd);
	}
	/*
	 *	
	 */
	void cSerialize3D::readRenderData(cRenderCacheData* rd)
	{
		readBUIDW(rd->m_buid);
		readOctree(&rd->m_octree);
		readCullFigure(&rd->m_cullFigure);
		readMaterialList(rd);
		readMeshList(rd);
	}
	/*
	 *	
	 */
	void cSerialize3D::readAnimation(cAnimation** aniData)
	{
		readValue(m_isAni);
		if (!m_isAni)
			return ;

		bool useGpu;
		int jointNum, motionNum;

		readValue(useGpu);
		readValue(jointNum);
		readValue(motionNum);

		std_string fname;
		_getInternString()->getStr(getFileName(), &fname);
		*aniData = new cAnimation;
		(*aniData)->initialize(fname.c_str(), jointNum, useGpu);

		// read joint reference
		readJointRef((*aniData)->m_controller->m_jointRef, jointNum);

		// motion
		for (int m = 0; m < motionNum; ++m)
		{
			cAnimationMotion* motion = new cAnimationMotion;

			// read frame
			readBUIDW(motion->m_buid);
			readValue(motion->m_frameNum);
			motion->m_frame = new sAnimationFrame[motion->m_frameNum];

			for (int f = 0; f < motion->m_frameNum; ++f)
			{
				sAnimationFrame* frame = motion->m_frame + f;
				frame->m_joint = new sJoint[jointNum];

				readValueList(frame->m_joint, jointNum);
			}

			(*aniData)->m_motionSet->insert(motion->m_buid, &motion);
		}

		std_string fullName;
		_getInternString()->getStr(getFullName(), &fullName);
		(*aniData)->loadMotionFile(fullName.c_str());
	}
	/*
	 *	
	 */
	void cSerialize3D::readJointRef(sJointRef* jointRef, int jointNum)
	{
		readValueList(jointRef, jointNum);
	}
	/*
	*/
	void cSerialize3D::readLocalJointIndex(sLocalJointIndex* localJointIndex)
	{
		readValue(localJointIndex->m_num);
		if (0 < localJointIndex->m_num)
		{
			localJointIndex->m_index = new WORD[localJointIndex->m_num];
			readValueList(localJointIndex->m_index, localJointIndex->m_num);
		}
	}
	/*
	 *	
	 */
	bool cSerialize3D::readFVFInfo(sVBInfo* vbInfo)
	{
		int count;
		readValue<int>(count);
		readValueList<sFVFInfo>(vbInfo->m_fvfInfo, sFVFInfo::FVF_NUM);
		return vbInfo->setFVFInfoToElement(count);
	}
	/*
	 *	
	 */
	bool cSerialize3D::readFVFInfo(sVBCacheInfo* vbInfo)
	{
		int count;
		readValue<int>(count);
		readValueList<sFVFInfo>(vbInfo->m_fvfInfo, sFVFInfo::FVF_NUM);
		return vbInfo->setFVFInfoToElement(count);
	}
	/*
	 *	
	 */
	void cSerialize3D::readVector3(sVector3& value)
	{
		readValue(value.x);
		readValue(value.y);
		readValue(value.z);
	}
	/*
	*/
	void cSerialize3D::readQuaternion(sQuaternion& value)
	{
		readValue(value.x);
		readValue(value.y);
		readValue(value.z);
		readValue(value.w);
	}
	/*
	 *	
	 */
	void cSerialize3D::readTerrain(cTerrain* terrain)
	{
		bool is;
		readValue(is);
		if (!is)
			return ;

		// cell list
		int cellType;
		readValue(cellType);
		int cellNum;
		readValue(cellNum);
		terrain->createCell(cellNum, cellType);
		terrain->m_cell->readCellData(this);

		// vertex list
		int vertNum;
		readValue(vertNum);
		terrain->createVertex(vertNum);
		readValueList(terrain->m_vertexList, vertNum);

		readOctree(&terrain->m_octree);
	}
	/*
	 *	
	 */
	void cSerialize3D::loadBfx()
	{
		TCHAR filename[MAX_PATH];
		std_string dir;
		_getInternString()->getStr(getDir(), &dir);
		std_string fname;
		_getInternString()->getStr(getFileName(), &fname);
		_stprintf(filename, _T("%s\\%s.bfx"), dir.c_str(), fname.c_str());
		m_serializeBfx.load(filename);
	}
}






















