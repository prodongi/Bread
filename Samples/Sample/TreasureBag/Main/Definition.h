
#ifndef _Definition_h_
#define _Definition_h_

/*
	Definition.h
	Copyright (c) prodongi. All rights resserved.

	date : 2012-06-07
	purpose : 
	*/

/*	
_c_ ������ cClassCreater������ �����ϵ��� �������ִ� ���ǹ�
*/
#define declareCustomClassCreater(_c_)		\
	protected:								\
		_c_();								\
		friend class cCustomClassCreater;	\
	public:									\
		virtual ~_c_();

#endif