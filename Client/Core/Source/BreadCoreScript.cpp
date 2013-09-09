
#include "BreadCoreScript.h"
#include "BreadDeclaration.h"
#include "BreadCore.h"
#include "BreadLuaManager.h"

namespace Bread
{
	void glueDoFile(char const* filename)
	{
		_getLuaMgr()->doFile(filename);
	}
}