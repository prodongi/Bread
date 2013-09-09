
#ifndef _BreadNoAlphaShaderAccumulate_h_
#define _BreadNoAlphaShaderAccumulate_h_

#include "BreadDeclaration.h"
#include "BreadStdHeader.h"
#include "BreadBoostSingletonPool.h"

namespace Bread
{
	class cMaterial;
	class cMesh;
	class cEntityModel;
	class cRenderData;
	class cAlphaAccumulate;
	class cShader;

	/*
		@date 2011.07.11
		@auth prodongi
		@desc 
		@todo 
	*/
	class cNoAlphaShaderAccumulate
	{
	public:
		cNoAlphaShaderAccumulate();
		~cNoAlphaShaderAccumulate();

		void collect(cRenderData* renderData, cMaterial const* mtl, cAlphaAccumulate* alphaAccum);
		void collectEmptyMtl(cRenderData* renderData, cAlphaAccumulate* alphaAccum);
		void renderCullFigure();
		void render();
		void renderSingleShader(cShader* shader, std_string const& technique, uint renderMask);
		void update(float elapsedtime);
		void updateDebugInfo();
		void clear();

	public:
		struct sMeshCollector
		{
			singletonPool(sMeshCollector)

			cEntityModel* m_entity;
			cMesh* m_mesh;
		};
		struct sMtlCollector
		{
			singletonPool(sMtlCollector)

			typedef std::vector<sMeshCollector*> v_mc;
			typedef v_mc::iterator				 iter_mc;

			enum	{	CONTAINER_SIZE = 20	};
			sMtlCollector()		{	m_meshColList.reserve(CONTAINER_SIZE);	}
			~sMtlCollector()	{	clear();	}
			void clear()		{	safeClearVectorPtr_R<iter_mc>(m_meshColList);	}
			cMaterial const* m_mtl;
			v_mc m_meshColList;
		};
		struct sShaderCollector
		{
			singletonPool(sShaderCollector)
			
			/// @brief map<material BUID, material collector>
			typedef std::map<BUID, sMtlCollector*> m_mc;
			typedef m_mc::iterator	iter_mc;

			~sShaderCollector()	{	clear();	}
			void clear() {	safeClearMapPtr_R<iter_mc>(m_mtlColList);	}
			m_mc m_mtlColList;
		};

	protected:
		typedef std::map<BUID, sShaderCollector*>	map_shadercol;
		typedef map_shadercol::iterator				iter_shadercol;
		typedef std::map<BUID, sMtlCollector*>		map_mtlcol;
		typedef map_mtlcol::iterator				iter_mtlcol;
		typedef std::vector<sMeshCollector*>		vec_meshcol;
		typedef vec_meshcol::iterator				iter_meshcol;

		/// @brief map<shader BUID, shader collector>
		map_shadercol m_list;
	};
}



#endif