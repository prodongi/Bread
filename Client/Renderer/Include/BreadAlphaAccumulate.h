
#ifndef _BreadAlphaAccumulate_h_
#define _BreadAlphaAccumulate_h_

#include "BreadStdHeader.h"
#include "BreadClientDefinition.h"

namespace Bread
{
	class cRenderData;
	class cMaterial;
	class cMesh;
	class cShader;

	/*
	 *	
	 */
	struct sAlphaAccumulateData
	{
		sAlphaAccumulateData(float len, cMaterial const* mtl, cMesh* mesh, cRenderData* rd) : m_len(len),
																m_mtl(mtl), m_mesh(mesh), m_rd(rd)
		{}

		float m_len;
		cMaterial const* m_mtl;
		cMesh* m_mesh;
		cRenderData* m_rd;
	};
	/*
	 *	
	 */
	struct sAlphaAccumulateOperator
	{
		bool operator () (sAlphaAccumulateData const& data1, sAlphaAccumulateData const& data2)
		{
			return data1.m_len > data2.m_len;
		}
	};
	/*
	 *	
	 */
	struct sAlphaAccumulateRender
	{
		inline void operator () (sAlphaAccumulateData const& data);
	};
	/*
	 *	
	 */
	struct sAlphaAccumulateRenderCullFigure
	{
		inline void operator () (sAlphaAccumulateData const& data);
	};

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
	 * \date 2010-01-06
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
	class cAlphaAccumulate
	{
	public:
		cAlphaAccumulate();
		~cAlphaAccumulate();
		void clear()	{	m_list.clear();	}
		void sort();
		void render();
		void renderCullFigure();
		void collect(cRenderData* renderData, cMaterial const* mtl);
		void collectMesh(cRenderData* renderData, cMaterial const* mtl, BUID const& meshBuid);
		void update(float elapsedtime);
		void renderSingleShader(cShader* shader, std_string const& technique, uint renderMask);

	private:
		enum {	CONTAINER_SIZE = 1000	};
		std::vector<sAlphaAccumulateData>	m_list;
	};
}















#endif















