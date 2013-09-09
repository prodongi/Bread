
#include "BreadStaticText.h"


namespace Bread
{
	/*
	*
	*/
	cStaticText::cStaticText()
	{
	}
	/*
	*
	*/
	cStaticText::~cStaticText()
	{
	}
	/*
	*/
	void cStaticText::setText(sFreeTypeFontDesc const& desc, std_string const& str)
	{
		m_ftText.setText(desc, str);
	}
	/*
	*/
	void cStaticText::setTextPos(float x, float y)
	{
		m_ftText.setPrimitive(x, y);
	}
	/*
	*/
	void cStaticText::render(LPDIRECT3DDEVICE9 device)
	{
		cWindow::render(device);
		m_ftText.render(device);
	}
}