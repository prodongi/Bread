
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
		/// @brief collect�ϱ� ���� clear()�� �� ȣ������� �ȴ�.
		virtual void collect(cRenderData const* renderData);

	protected:
		typedef std::vector<BUID>	vec_col;
		typedef vec_col::iterator	iter_col;

		vec_col m_list;
	};
}













#endif






