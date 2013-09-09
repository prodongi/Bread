
#ifndef _BreadRenderDataCollectorMtl_h_
#define _BreadRenderDataCollectorMtl_h_

#include "BreadRenderDataCollector.h"
#include "BreadDeclaration.h"
#include "BreadStdHeader.h"

namespace Bread
{
	/*
	*/
	class cRenderDataCollectorMtl : public cRenderDataCollector
	{
	public:
		cRenderDataCollectorMtl() {}
		virtual ~cRenderDataCollectorMtl()	{	clearList();	}
		virtual void render(cRenderDataManager const* rdMgr);
		
	protected:
		virtual void clearList()	{	m_list.clear();	}
		/// @brief collect하기 전에 clear()를 꼭 호출해줘야 된다.
		virtual void collect(cRenderData const* renderData);

	protected:
		typedef std::vector<BUID>	vec_col;
		typedef vec_col::iterator	iter_col;

		vec_col m_list;
	};
}













#endif






