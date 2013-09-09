
#ifndef _BreadSerializeBmd_h_
#define _BreadSerializeBmd_h_

#include "BreadSerialize3D.h"
#include "BreadBounding.h"

namespace Bread
{
	class cAnimation;
	class cRenderData;
	class cRenderCacheData;

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
	class cSerializeBmd : public cSerialize3D
	{
	public:
		/*
		*/
		struct sData
		{
			sData() : m_renderData(NULL), m_animation(NULL), m_octree(NULL) {}
			~sData();
			void copy(sData* copyData, uint shareType);
			BUID const& getBuid() const	{	return m_buid;					}
			BUID m_buid;
			cRenderData* m_renderData;
			cOctree* m_octree;
			sCullFigure m_cullFigure;
			cAnimation* m_animation;
		};
		/*	thread cache를 로드 할 때 쓰임
		*/
		struct sCacheData
		{
			sCacheData() : m_renderData(NULL), m_animation(NULL), m_octree(NULL) {}
			~sCacheData();
			void copy(sData* copyData, uint shareType);
			BUID const& getBuid() const	{	return m_buid;					}
			BUID m_buid;
			cRenderCacheData* m_renderData;
			cOctree* m_octree;
			sCullFigure m_cullFigure;
			cAnimation* m_animation;
		};

		static void makeFileName(BUID const& buid, std_string& filename);

	public:
		cSerializeBmd();
		virtual ~cSerializeBmd();

		bool load(std_string const& filename, sData& data);
		bool load(std_string const& filename, sCacheData& data);
	};
}










#endif