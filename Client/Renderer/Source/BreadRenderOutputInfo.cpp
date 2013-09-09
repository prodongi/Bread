
#include "BreadD3DHeader.h"
#include "BreadRenderOutputInfo.h"


namespace Bread
{
	unsigned int cRenderOutputInfo::m_vertexNumber = 0;
	unsigned int cRenderOutputInfo::m_faceNumber = 0;
	unsigned int cRenderOutputInfo::m_dpNumber = 0;
	bool cRenderOutputInfo::m_enable = true;

	/*
	 *	
	 */
	void cRenderOutputInfo::clear()
	{
		m_vertexNumber = 0;
		m_faceNumber = 0;
		m_dpNumber = 0;
	}
	/*
	 *	
	 */
	void cRenderOutputInfo::add(unsigned int primitiveType, unsigned int vertexNum, unsigned int faceNum)
	{
		addVertexNumber(vertexNum);
		if (primitiveType != D3DPT_POINTLIST && primitiveType != D3DPT_LINELIST && primitiveType != D3DPT_LINESTRIP)
			addFaceNumber(faceNum);
		++m_dpNumber;
	}
}



















