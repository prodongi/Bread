
#ifndef _BreadRenderDataCollector_h_
#define _BreadRenderDataCollector_h_

#include "BreadClientDefinition.h"

namespace Bread
{
	class cRenderData;
	class cRenderDataManager;
	class cShader;

	/*
	*/
	class cRenderDataCollector
	{
	public:
		cRenderDataCollector() {}
		virtual ~cRenderDataCollector();
		virtual void render(cRenderDataManager const* /*rdMgr*/) {}
		virtual void renderCullFigure() {}
		virtual void renderSingleShader(cShader* /*shader*/, std_string const& /*technique*/, uint /*renderMask*/) {}
		virtual void update(float elapsedtime);
		
	protected:
		virtual void collect(cRenderData* /*renderData*/) {}
		void collect();
		virtual void alphaSort() {}
		virtual void clearList() {}		
	};
}

#endif






