
#include "stdafx.h"
#include "TreasureBag.h"
#include "TreasureApp.h"
#include "CustomClassCreater.h"

/*
 *	
 */
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	unreferencedParameter(hPrevInstance);
	unreferencedParameter(lpCmdLine);
	unreferencedParameter(nCmdShow);

	_getCore()->setClassCreater(new cCustomClassCreater);
	cTreasureApplication app;
	app.run(hInstance);

	return 0;
}




