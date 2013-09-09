
#include "BreadAxis.h"
#include "BreadDeclaration.h"
#include "BreadCore.h"
#include "BreadRenderPrimitive.h"
#include "BreadBatchRenderManager.h"
#include "BreadD3DDeclaration.h"

namespace Bread
{
	bool cAxis::m_isRender = true;

	/*
	 *	
	 */
	cAxis::cAxis() : m_iv(NULL)
	{
	}
	/*
	 *	
	 */
	cAxis::~cAxis()
	{
		safeDeleteArray(m_iv);
	}
	/*
	 *	
	 */
	void cAxis::initialize()
	{
		assert(!m_iv);
		m_iv = new sLineVertex[6];

		// x, r
		m_iv[0].set(0.0f, 0.0f, 0.0f, COLOR_RED);
		m_iv[1].set(50.0f, 0.0f, 0.0f, COLOR_RED);

		// y, g
		m_iv[2].set(0.0f, 0.0f, 0.0f, COLOR_GREEN);
		m_iv[3].set(0.0f, 50.0f, 0.0f, COLOR_GREEN);

		// z, b
		m_iv[4].set(0.0f, 0.0f, 0.0f, COLOR_BLUE);
		m_iv[5].set(0.0f, 0.0f, 50.0f, COLOR_BLUE);
	}
	/*
	 *	
	 */
	void cAxis::render()
	{
		if (!cAxis::m_isRender)
			return ;

		_getBatchRenderMgr()->push(m_iv, 6);
	}
}






















