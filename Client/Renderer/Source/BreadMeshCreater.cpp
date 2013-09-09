
#include "BreadMeshCreater.h"
#include "BreadVBManager.h"
#include "BreadIBManager.h"
#include "BreadRenderPrimitive.h"
#include "BreadD3DDeclaration.h"

namespace Bread
{
	implementInstance(cMeshCreater)

	/*
	*/
	cMeshCreater::cMeshCreater()
	{
		assignInstance(this)
	}
	/*
	*/
	cMeshCreater::~cMeshCreater()
	{
		assignInstance(NULL)
	}
	/*
	*/
	sVBInfo* cMeshCreater::createVBInfo(BUID const& buid, bool dynamic, int vertexNum, D3DPRIMITIVETYPE primitiveType, 
												  uint stride, uint vbSize, DWORD fvf)
	{
		sVBInfo* vbInfo = new sVBInfo;
		vbInfo->m_buid = buid;
		vbInfo->setFlag(dynamic);
		vbInfo->m_vertexNum = vertexNum;
		vbInfo->m_primitiveType = primitiveType;
		vbInfo->m_stride = stride;
		vbInfo->m_vbSize = vbSize;
		vbInfo->setD3DFVFToElement(fvf);

		return vbInfo;
	}
	/*
	*/
	sIBInfo* cMeshCreater::createIBInfo(BUID const& buid, uint ibSize, uint faceNum)
	{
		sIBInfo* ibInfo = new sIBInfo;
		ibInfo->m_buid = buid;
		ibInfo->m_ibSize = ibSize;
		ibInfo->m_faceNum = faceNum;

		return ibInfo;
	}
	/*
	*/
	int cMeshCreater::getIbSize(int faceNum, D3DPRIMITIVETYPE primitiveType) const
	{
		int ibSize;
		if (D3DPT_TRIANGLESTRIP == primitiveType)	ibSize = (faceNum + 2) * (int)sizeof (WORD);
		else										ibSize = faceNum * (int)sizeof (WORD) * 3;

		return ibSize;
	}
	/*
	*/
	bool cMeshCreater::createPlane(BUID const& buid, int width, int height, int segmentX, int segmentZ, bool uvWrap)
	{
		/// 가로/세로 타일 개수
		int tileWidth = (width - 1)/segmentX + 1;
		int tileHeight = (height - 1)/segmentZ + 1;

		/// 가로/세로 버텍스 개수
		int vertexWidth = tileWidth + 1;
		int vertexHeight = tileHeight + 1;

		/// 버텍스 개수
		uint vertexNum = max(vertexWidth * vertexHeight, 0);
		
		float uvSegmentX, uvSegmentZ;
		if (uvWrap)
		{
			uvSegmentX = 1.0f;
			uvSegmentZ = 1.0f;
		}
		else
		{
			uvSegmentX = 1.0f/(float)tileWidth;
			uvSegmentZ = 1.0f/(float)tileHeight;
		}

		D3DPRIMITIVETYPE primitiveType(D3DPT_TRIANGLELIST);

		/// 
		void* data;
		int faceNum = tileWidth * tileHeight * 2;
		int vbSize = sFaceVertex::SIZE * vertexNum;
		int ibSize = getIbSize(faceNum, primitiveType);
				
		/// create vertex buffer info
		sVBInfo* vbInfo = createVBInfo(buid, false, vertexNum, primitiveType, sFaceVertex::SIZE, vbSize, sFaceVertex::D3DFVF);
		if (!_getVBMgr()->create(vbInfo))
		{
			safeDelete(vbInfo);
			return false;
		}
		
		if (vbInfo->lock(&data))
		{
			/// vertex buffer 생성
			int sx = -(width >> 1);
			int sz = (height >> 1);
			sFaceVertex* vb_buffer = (sFaceVertex*)data;
			for (int z = 0; z < vertexHeight; ++z)
			{
				int offsetZ = z * segmentZ;
				float uvOffsetZ = (float)z * uvSegmentZ;
				for (int x = 0; x < vertexWidth; ++x)
				{
					int offsetX = x * segmentX;
					float uvOffsetX = (float)x * uvSegmentX;
					vb_buffer[z * vertexWidth + x].set((float)(sx+offsetX), 0.0f, (float)(sz-offsetZ), 0.0f, 1.0f, 0.0f, uvOffsetX, uvOffsetZ, COLOR_WHITE);
				}
			}
			vbInfo->unlock();
		}
		else
		{
			return false;
		}

		/// create index buffer info
		sIBInfo* ibInfo = createIBInfo(buid, ibSize, faceNum);
		if (!_getIBMgr()->create(ibInfo))
		{
			safeDelete(ibInfo);
			return false;
		}

		/// index buffer 생성
		if (ibInfo->lock(&data))
		{
			int tile_index, buffer_index, temp1, temp2;
			WORD* ib_buffer = (WORD*)data;
			for (int z = 0; z < tileHeight; ++z)
			{
				for (int x = 0; x < tileWidth; ++x)
				{
					tile_index = (z * tileWidth + x) * 2;
					temp1 = z * vertexWidth + x;
					temp2 = (z + 1) * vertexWidth + x;

					buffer_index = tile_index * 3;
					ib_buffer[buffer_index + 0] = (WORD)temp2;
					ib_buffer[buffer_index + 1] = (WORD)temp1 + 1;
					ib_buffer[buffer_index + 2] = (WORD)temp1;

					buffer_index += 3;
					ib_buffer[buffer_index + 0] = (WORD)temp2;
					ib_buffer[buffer_index + 1] = (WORD)temp2 + 1;
					ib_buffer[buffer_index + 2] = (WORD)temp1 + 1;
				}
			}

			ibInfo->unlock();
		}
		else
		{
			return false;
		}

		return true;
	}
	/*
	*/
	bool cMeshCreater::createWeatherParticle(BUID const& buid, int particleNum, float boxSize)
	{
		int vertexNum = particleNum * 4;
		int faceNum = particleNum * 2;

		D3DPRIMITIVETYPE primitiveType(D3DPT_TRIANGLELIST);

		/// 
		void* data;
		int vbSize = sUvVertex::SIZE * vertexNum;
		int ibSize = getIbSize(faceNum, primitiveType);

		/// create vertex buffer info
		sVBInfo* vbInfo = createVBInfo(buid, false, vertexNum, primitiveType, sUvVertex::SIZE, vbSize, sUvVertex::D3DFVF);
		if (!_getVBMgr()->create(vbInfo))
		{
			safeDelete(vbInfo);
			return false;
		}

		if (vbInfo->lock(&data))
		{
			sVector3 defaultOffset(boxSize, boxSize, boxSize);
			sVector3 pos;
			sUvVertex* vb_buffer = (sUvVertex*)data;
			for (int i = 0; i < particleNum; ++i)
			{
				pos.x = cMath::rand(boxSize, (float)RAND_MAX);
				pos.y = cMath::rand(boxSize, (float)RAND_MAX);
				pos.z = cMath::rand(boxSize, (float)RAND_MAX);
				pos += defaultOffset;

				vb_buffer->set(pos.x, pos.y, pos.z, 0.0f, 1.0f);	++vb_buffer;
				vb_buffer->set(pos.x, pos.y, pos.z, 0.0f, 0.0f);	++vb_buffer;
				vb_buffer->set(pos.x, pos.y, pos.z, 1.0f, 0.0f);	++vb_buffer;
				vb_buffer->set(pos.x, pos.y, pos.z, 1.0f, 1.0f);	++vb_buffer;
			}

			vbInfo->unlock();
		}
		else
		{
			return false;
		}


		/// create index buffer info
		sIBInfo* ibInfo = createIBInfo(buid, ibSize, faceNum);
		if (!_getIBMgr()->create(ibInfo))
		{
			safeDelete(ibInfo);
			return false;
		}

		if (ibInfo->lock(&data))
		{
			WORD* ib_buffer = (WORD*)data;
			for (int i = 0; i < particleNum; ++i)
			{
				int index = i * 6;
				WORD v_index = (WORD)i * 4;
				ib_buffer[index + 0] = v_index + 0;
				ib_buffer[index + 1] = v_index + 1;
				ib_buffer[index + 2] = v_index + 2;

				ib_buffer[index + 3] = v_index + 2;
				ib_buffer[index + 4] = v_index + 3;
				ib_buffer[index + 5] = v_index + 0;
			}

			ibInfo->unlock();
		}
		else
		{
			return false;
		}

		return true;
	}
	/*
	*/
	bool cMeshCreater::createSkyBox(BUID const& buid)
	{
		int vertexNum = 8;
		int faceNum = 12;

		D3DPRIMITIVETYPE primitiveType(D3DPT_TRIANGLELIST);

		/// 
		void* data;
		int vbSize = sPointVertex::SIZE * vertexNum;
		int ibSize = getIbSize(faceNum, primitiveType);

		/// create vertex buffer info
		sVBInfo* vbInfo = createVBInfo(buid, false, vertexNum, primitiveType, sPointVertex::SIZE, vbSize, sPointVertex::D3DFVF);
		if (!_getVBMgr()->create(vbInfo))
		{
			safeDelete(vbInfo);
			return false;
		}

		if (vbInfo->lock(&data))
		{
			sPointVertex* vb_buff = (sPointVertex*)data;

			vb_buff[0].set(1.0f, 1.0f, -1.0f);
			vb_buff[1].set(-1.0f, 1.0f, -1.0f);
			vb_buff[2].set(-1.0f, -1.0f, -1.0f);
			vb_buff[3].set(1.0f, -1.0f, -1.0f);

			vb_buff[4].set(1.0f, 1.0f, 1.0f);
			vb_buff[5].set(-1.0f, 1.0f, 1.0f);
			vb_buff[6].set(-1.0f, -1.0f, 1.0f);
			vb_buff[7].set(1.0f, -1.0f, 1.0f);

			vbInfo->unlock();
		}
		else
		{
			return false;
		}


		/// create index buffer info
		sIBInfo* ibInfo = createIBInfo(buid, ibSize, faceNum);
		if (!_getIBMgr()->create(ibInfo))
		{
			safeDelete(ibInfo);
			return false;
		}

		if (ibInfo->lock(&data))
		{
			struct sIndex
			{
				void set(WORD* ib_buffer, WORD i1, WORD i2, WORD i3)
				{
					ib_buffer[0] = i1;	ib_buffer[1] = i2;	ib_buffer[2] = i3;
				}
			};
			sIndex func;
			WORD* ib_buffer = (WORD*)data;

			func.set(ib_buffer+0, 0, 1, 3);
			func.set(ib_buffer+3, 1, 2, 3);

			func.set(ib_buffer+6, 0, 4, 1);
			func.set(ib_buffer+9, 1, 4, 5);

			func.set(ib_buffer+12, 5, 4, 7);
			func.set(ib_buffer+15, 5, 7, 6);

			func.set(ib_buffer+18, 1, 6, 5);
			func.set(ib_buffer+21, 1, 2, 6);

			func.set(ib_buffer+24, 0, 3, 7);
			func.set(ib_buffer+27, 0, 7, 4);

			func.set(ib_buffer+30, 2, 6, 7);
			func.set(ib_buffer+33, 2, 7, 3);

			ibInfo->unlock();
		}
		else
		{
			return false;
		}

		return true;
	}
	/*
	*/
	bool cMeshCreater::createVolumeCloud(BUID const& buid, float width, float height, int sliceNum)
	{
		int vertexNum = sliceNum * 4;
		int faceNum = sliceNum * 2;

		D3DPRIMITIVETYPE primitiveType(D3DPT_TRIANGLELIST);

		/// 
		void* data;
		int vbSize = sVolumeCloudVertex::SIZE * vertexNum;
		int ibSize = getIbSize(faceNum, primitiveType);

		/// create vertex buffer info
		sVBInfo* vbInfo = createVBInfo(buid, false, vertexNum, primitiveType, sVolumeCloudVertex::SIZE, vbSize, sVolumeCloudVertex::D3DFVF);
		if (!_getVBMgr()->create(vbInfo))
		{
			safeDelete(vbInfo);
			return false;
		}

		if (vbInfo->lock(&data))
		{
			sVolumeCloudVertex* vb_buff = (sVolumeCloudVertex*)data;

			DWORD color = 0xffffffff;
			float depth = 1.0f;
			float dinc = 1.0f/(float)sliceNum;
			for (int i = 0; i < sliceNum; ++i)
			{
				int _i = i * 4;
				vb_buff[_i+0].set(-width, -height, depth, color, 0.0f, 1.0f, depth);
				vb_buff[_i+1].set(-width,  height, depth, color, 0.0f, 0.0f, depth);
				vb_buff[_i+2].set( width,  height, depth, color, 1.0f, 0.0f, depth);
				vb_buff[_i+3].set( width, -height, depth, color, 1.0f, 1.0f, depth);

				depth -= dinc;
			}
			

			vbInfo->unlock();
		}
		else
		{
			return false;
		}


		/// create index buffer info
		sIBInfo* ibInfo = createIBInfo(buid, ibSize, faceNum);
		if (!_getIBMgr()->create(ibInfo))
		{
			safeDelete(ibInfo);
			return false;
		}

		if (ibInfo->lock(&data))
		{
			WORD* ib_buffer = (WORD*)data;

			for (int i = 0; i < sliceNum; ++i)
			{
				int index = i * 6;
				WORD v_index = (WORD)i * 4;
				ib_buffer[index + 0] = v_index + 0;
				ib_buffer[index + 1] = v_index + 1;
				ib_buffer[index + 2] = v_index + 2;

				ib_buffer[index + 3] = v_index + 2;
				ib_buffer[index + 4] = v_index + 3;
				ib_buffer[index + 5] = v_index + 0;
			}

			ibInfo->unlock();
		}
		else
		{
			return false;
		}

		return true;
	}
}