
#include "BreadCoreScript.h"
#include "BreadDeclaration.h"
#include "BreadCore.h"
#include "BreadLuaManager.h"

namespace Bread
{
	void registerLuaFunc_CoreScript()
	{
		_getLuaMgr()->registerFunc("glueDoFile",glueDoFile);
	}
}
