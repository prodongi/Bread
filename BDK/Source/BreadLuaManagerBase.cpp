
#include "BreadLuaManagerBase.h"

namespace Bread
{
	/*
	 *	
	 */
	cLuaManagerBase::cLuaManagerBase() : m_luaState(NULL)
	{
	}
	/*
	 *	
	 */
	cLuaManagerBase::~cLuaManagerBase()
	{
		terminate();
	}
	/*
	 *	
	 */
	void cLuaManagerBase::init()
	{
		m_luaState = lua_open();

//		luaL_openlibs(m_luaState);

		luaopen_base(m_luaState);
		luaopen_string(m_luaState);
		luaopen_math(m_luaState);
		luaopen_debug(m_luaState);
		luaopen_table(m_luaState);
	}
	/*
	 *	
	 */
	void cLuaManagerBase::terminate()
	{
		lua_close(m_luaState);
	}
	/*
	 *	
	 */
	void cLuaManagerBase::doFile(const char* filename)
	{
		lua_tinker::dofile(m_luaState, filename);	
	}
	/*
	*	
	*/
	void cLuaManagerBase::registerPredefineMember(char const* name, int value)
	{
		lua_pushnumber(m_luaState, (lua_Number)value);
		lua_setglobal(m_luaState, name);
	}
}