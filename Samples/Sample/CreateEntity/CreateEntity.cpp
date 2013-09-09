
#include "stdafx.h"
#include "CreateEntity.h"
#include "SampleApp.h"
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
	cSampleApplication app;
	app.run(hInstance);

	return 0;
   }




