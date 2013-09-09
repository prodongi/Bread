
#include "stdafx.h"
#include "SampleConsole.h"
#include "BreadStaticText.h"

/*
*/
void cSampleConsole::initialize(float x, float y, float w, float h)
{
	cConsole::initialize(x, y, w, h);

	cStaticText* consolText = (cStaticText*)findChild(_T("ConsoleText"));
	if (consolText)
	{
		consolText->setText(sFreeTypeFontDesc(_T("default"), 13), _T("prodongi"));
		consolText->setTextPos(100.0f,25.0f);
	}

	createVB();
}
/*
*/
void cSampleConsole::update(float elapsedtime, int x, int y)
{
	if (!getShow()) return ;
	cConsole::update(elapsedtime, x, y);


}









