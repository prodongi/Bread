
#ifndef _BreadDefinition_h_
#define _BreadDefinition_h_

#include "BreadRTTI.h"

namespace Bread
{
	/*
	*	
	*/
	#ifdef UNICODE
		typedef std::wstring std_string;
	#else
		typedef std::string std_string;
	#endif
	typedef unsigned int uint;
	typedef std_string	 BUID;
	typedef unsigned int string_id;
	typedef __int64		 i64;
	#define pure = 0
	/*
	*	
	*/
	#define NAME_LEN		64
	#define FILENAME_LEN	64
	/*
	*	
	*/
	#define propertyFunc(rVal, func, member)				\
		rVal get##func()			{	return member;	}	\
		void set##func(rVal val)	{	member = val;	}
	/*
	*/
	#define propertyConstFunc(rVal, func, member)					\
		rVal const& get##func() const		{	return member;	}	\
		void set##func(rVal const& val)		{	member = val;	}
	/*
	*/
	#define unreferencedParameter(p)	(p)
	/*
	*/
	#define arrayCount(x)	(sizeof (x)/sizeof (x[0]))
	/*	
		_c_ 선언을 cClassCreater에서만 가능하도록 선언해주는 정의문
	*/
	#define declareClassCreater(_c_)	\
		protected:						\
			_c_();						\
			friend class cClassCreater;	\
		public:							\
			virtual ~_c_();
	/*
	*/
	#define declareInstance(_c_)							\
		public:												\
			static _c_* getInstance()						\
			{												\
				assert(m_instance && "m_instance is NULL");	\
				return m_instance;							\
			}												\
		private:											\
			static _c_* m_instance;
	/*
	*/
	#define implementInstance(_c_)		\
		_c_* _c_::m_instance = NULL;
	/*
	*/
	#define assignInstance(_this_)									\
		assert(m_instance != _this_ && "m_instance isn't NULL");	\
		m_instance = _this_;
	/*
	*/
	#define declareRTTI()														\
		public:																		\
			static cRTTI const m_rtti;											\
			virtual cRTTI const& getRTTI() const		{	return m_rtti;	}	\
			virtual std::string const& getClassName() const	{	return m_rtti.getClassName();	}
	/*
	*/
	#define implementBaseRTTI(className)								\
		cRTTI const className::m_rtti(#className);
	/*
	*/
	#define implementRTTI(className, parent)							\
		cRTTI const className::m_rtti(#className, &parent::m_rtti);
	/*
	*/
#ifdef _PROFILE_
	#define PROFILE(name)			cProfile profile(name)
	#define PROFILE_RENDER(x, y)	cProfileManager::render(x, y);
	#define PROFILE_RESET()			cProfileManager::reset();
#else
	#define PROFILE(name)
	#define PROFILE_RENDER(x, y)	{	x = x;	y = y;	}
	#define PROFILE_RESET()
#endif
}

#endif






