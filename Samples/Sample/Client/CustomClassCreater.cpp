
#include "stdafx.h"
#include "CustomClassCreater.h"
#include "SampleGui.h"

cGui* cCustomClassCreater::newGui()	{	return new cSampleGui;	}