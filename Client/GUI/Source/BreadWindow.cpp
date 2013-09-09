
#include "BreadWindow.h"
#include "BreadUtils.h"
#include "BreadCore.h"
#include "BreadGui.h"
#include "BreadDeclaration.h"
#include "BreadStringHelper.h"

namespace Bread
{
	/*
	*/
	cWindow::cWindow() : m_texWidth(1.0f), m_texHeight(1.0f), m_root(false),
						 m_parent(NULL), m_show(true), m_enable(true), m_focus(false)
	{
		setWinType(cWindow::UI_WINDOW);
	}
	/*
	 *	
	 */
	cWindow::cWindow(cWindow* parent) : m_texWidth(1.0f), m_texHeight(1.0f), m_root(false),
											  m_parent(parent)
	{
		setWinType(cWindow::UI_WINDOW);
	}
	/*
	 *	
	 */
	cWindow::cWindow(bool root, cWindow* parent) : m_root(root), m_parent(parent)
	{
	}
	/*
	 *	
	 */
	cWindow::~cWindow()
	{
	}
	/*
	 *	
	 */
	void cWindow::render(LPDIRECT3DDEVICE9 device)
	{
		if (!getShow()) return ;
		cWindowRenderData::render(device);
		renderChildList(device);
	}
	/*
	 *	
	 */
	void cWindow::update(float elapsedtime, int x, int y)
	{
		if (!getShow()) return ;
		if (isIn(x, y))	_getGui()->setOverWindow(this);

		updateChildList(elapsedtime, x, y);
	}
	/*
	 *	
	 */
	void cWindow::setXmlAttribute(cWindowXmlSerialize<cWindow>::sXmlAttribute const& attri)
	{
		m_posRect = cRect2(attri.m_x, attri.m_y, attri.m_w, attri.m_h);
		_getStringHelper()->m2w(attri.m_name.c_str(), m_uiName, WINDOW_NAME_LEN);
		m_tooltipId = attri.m_tooltipId;
	}
	/*
	 *	
	 */
	void cWindow::setXmlTextureAttribute(cWindowXmlSerialize<cWindow>::sXmlAttribute const& attri)
	{
		m_texRect = cRect2(attri.m_tx, attri.m_ty, attri.m_tw, attri.m_th);
		_getStringHelper()->m2w(attri.m_file.c_str(), m_texName, WINDOW_NAME_LEN);
	}
	/*
	 *	
	 */
	HRESULT cWindow::createVB()
	{
		HRESULT hr;
		if (!m_root)
		{
			if (FAILED(hr = cWindowRenderData::createVB(m_posRect, m_texRect, m_texWidth, m_texHeight)))
				return hr;
		}

		unsigned int childNum = m_childList.getNum();
		for (unsigned int i = 0; i < childNum; ++i)
		{
			cWindow* child = m_childList.get(i);
			if (FAILED(hr = child->createVB()))
				return hr;
		}

		return S_OK;
	}
}















