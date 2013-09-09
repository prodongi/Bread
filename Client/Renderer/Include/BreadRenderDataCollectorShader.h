
#ifndef _BreadRenderDataCollectorShader_h_
#define _BreadRenderDataCollectorShader_h_

#include "BreadRenderDataCollector.h"

namespace Bread
{
	class cAlphaAccumulate;
	class cNoAlphaShaderAccumulate;
	class cDecalAccumulate;

	/*
	*/
	class cRenderDataCollectorShader : public cRenderDataCollector
	{
	public:
		cRenderDataCollectorShader();
		virtual ~cRenderDataCollectorShader();
		virtual void render(cRenderDataManager const* rdMgr);
		virtual void update(float elapsedtime);
		virtual void renderCullFigure();
		virtual void renderSingleShader(cShader* shader, std_string const& technique, uint renderMask);

	protected:
		/// @brief collect�ϱ� ���� clear()�� �� ȣ������� �ȴ�.
		virtual void collect(cRenderData* renderData);
		virtual void alphaSort();
		virtual void clearList();

	private:
		cAlphaAccumulate* m_alphaAccum;
		cNoAlphaShaderAccumulate* m_noalphaAccum;
		cDecalAccumulate* m_decalAccum;
	};
}

#endif






