
#ifndef _BreadLuaManagerBase_h_
#define _BreadLuaManagerBase_h_

#include "lua_tinker.h"

namespace Bread
{
	/**
	 * \ingroup Core
	 *
	 *
	 * \par requirements
	 * win98 or later\n
	 * win2k or later\n
	 * MFC\n
	 *
	 * \version 1.0
	 * first version
	 *
	 * \date 2009-07-31
	 *
	 * \author Prodongi
	 *
	 * \par license
	 * This code is absolutely free to use and modify. The code is provided "as is" with
	 * no expressed or implied warranty. The author accepts no liability if it causes
	 * any damage to your computer, causes your pet to fall ill, increases baldness
	 * or makes your car start emitting strange noises when you start it up.
	 * This code has no bugs, just undocumented features!
	 * 
	 * \todo 
	 *
	 * \bug 
	 *
	 */
	class cLuaManagerBase
	{
	public:
		cLuaManagerBase();
		virtual ~cLuaManagerBase();

		virtual void init();
		virtual void terminate();
		void doFile(const char* filename);
		
		/*
		 *	
		 */
		template <typename F> void registerFunc(const char* funcname, F func)
		{
			lua_tinker::def(m_luaState, funcname, func);
		}
		/*
		 *	
		 */
		template<typename T> void registerClass(const char* classname)
		{
			lua_tinker::class_add<T>(m_luaState, classname);
		}
		/*
		 *	
		 */
		template <typename T, typename F> void registerClassFunc(const char* funcname, F func)
		{
			lua_tinker::class_def<T>(m_luaState, funcname, func);
		}
		/*
		 *	
		 */
		template<typename T, typename P> void registerClassInh()
		{
			lua_tinker::class_inh<T, P>(m_luaState);
		}
		/*
		 *	
		 */
		// Tinker Class Constructor
		template<typename T, typename CONSTRUCTOR> void registerClassCon(lua_State* L, CONSTRUCTOR)
		{
			lua_tinker::class_con<T>(m_luaState, lua_tinker::constructor<CONSTRUCTOR>());
		}
		/*
		 *	
		 */
		template<typename T> void set(const char* name, T object)
		{
			lua_tinker::set(m_luaState, name, object);
		}
		/*
		 *	
		 */
		template<typename RVal> RVal callFunc(const char* name)
		{
			return lua_tinker::call<RVal>(m_luaState, name);
		}
		/*
		 *	
		 */
		template <typename RVal> RVal getTableValue(const char* tablename, const char* valuename)
		{
			lua_tinker::table t = lua_tinker::get<lua_tinker::table>(m_luaState, tablename);
			return t.get<RVal>(valuename);
		}		
		/*
		 *	
		 */
		template <typename RVal> RVal getTableValue(const char* tablename, int idx)
		{
			lua_tinker::table t = lua_tinker::get<lua_tinker::table>(m_luaState, tablename);
			return t.get<RVal>(idx);
		}
		/*
		 *	tablIdx is zero base
		 */
		template <typename RVal> RVal getTableValue(const char* tablename, int tablIdx, const char* valuename)
		{
			lua_tinker::table t = lua_tinker::get<lua_tinker::table>(m_luaState, tablename);
			lua_tinker::table t_idx = t.get<lua_tinker::table>(tablIdx+1);
			return t_idx.get<RVal>(valuename);
		}
		/*
		*/
		size_t getn(const char* tablename)
		{
			lua_tinker::table t = lua_tinker::get<lua_tinker::table>(m_luaState, tablename);
			return t.getn();
		}

	protected:
		/// 루아에 pre define된 맴버 등록
		void registerPredefineMember(char const* name, int value);

	protected:
		lua_State*	m_luaState;
	};
}



#endif