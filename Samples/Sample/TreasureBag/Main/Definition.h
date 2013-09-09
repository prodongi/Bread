
#ifndef _Definition_h_
#define _Definition_h_

/*
	Definition.h
	Copyright (c) prodongi. All rights resserved.

	date : 2012-06-07
	purpose : 
	*/

/*	
_c_ 선언을 cClassCreater에서만 가능하도록 선언해주는 정의문
*/
#define declareCustomClassCreater(_c_)		\
	protected:								\
		_c_();								\
		friend class cCustomClassCreater;	\
	public:									\
		virtual ~_c_();

#endif