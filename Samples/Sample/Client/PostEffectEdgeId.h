
#ifndef _PostEffectEdgeId_h_
#define _PostEffectEdgeId_h_

#include "BreadUiPrimitive.h"
#include "BreadRenderToTexture.h"

namespace Bread
{
	cTexture;
};

class cPostEffectEdgeId
{
public:
	cPostEffectEdgeId();
	~cPostEffectEdgeId();

	bool initialize(uint width, uint height);
	void renderEdgeId();
	void renderFinal();
	cTexture* getRttTexture() const	{	return m_rtt->getTexture();	}

private:
	struct sT4Vertex
	{
		float p[4];
		float t[4][2];
	};

	cRenderToTextureBase* m_rtt;
};








#endif