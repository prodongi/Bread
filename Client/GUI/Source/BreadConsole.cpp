
#include "BreadConsole.h"
#include "BreadBackground.h"
#include "BreadEditBox.h"
#include "BreadStaticText.h"

namespace Bread
{
	/*
	*/
	cConsole::cConsole()
	{
	}
	/*
	*/
	cConsole::~cConsole()
	{
	}
	/*
	*/
	void cConsole::initialize(float x, float y, float w, float h)
	{
		setPosRect(cRect2(x, y, w, h));
		
		//cBackground* back = new cBackground;
		//addChild(back);

		cStaticText* staticText = new cStaticText;
		staticText->setUiName(_T("ConsoleText"));
		addChild(staticText);

		cEditBox* editBox = new cEditBox;
		editBox->setUiName(_T("ConsoleEdit"));
		editBox->setPosRect(cRect2(x, y, w, 18));
		editBox->setColor(0xffff0000);
		addChild(editBox);
	}
}