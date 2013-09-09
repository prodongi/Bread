
#ifndef _BreadInline_h_
#define _BreadInline_h_

#include <assert.h>

namespace Bread
{
	/*
	*/
	template <class T>
	inline void safeDelete(T& t)
	{
		if (t)
		{
			delete t;
			t = 0;
		}
	}
	/*
	*/
	template <class T>
	inline void safeDelete(T& t, bool callFinal)
	{
		if (t)
		{
			if (callFinal) t->finalize();
			delete t;
			t = 0;
		}
	}
	/*
	*/
	template <class T>
	inline void safeDeleteArray(T& t)
	{
		if (t)
		{
			delete[] t;
			t = 0;
		}
	}
	/*
	*/
	template <class T>
	inline void safeRelease(T& t)
	{
		if (t)
		{
			t->Release();
			t = 0;
		}
	}
	/*
	*	
	*/
	template <class Iter, class T>
	inline void safeClearMapPtr_R(T& _t)
	{
		Iter _i = _t.begin();
		for (; _i != _t.end(); ++_i)
		{
			if (_i->second)
				delete _i->second;
		}
		_t.clear();
	}
	/*
	*	
	*/
	template <class Iter, class T>
	inline void safeClearMapPtr(T& _t)
	{
		if (_t)
		{
			Iter _i = _t->begin();
			for (; _i != _t->end(); ++_i)
			{
				if (_i->second)
					delete _i->second;
			}
			_t->clear();
		}
	}
	/*
	*	
	*/
	template <class Iter, class T>
	inline void safeDeleteMapPtr(T& _t)
	{
		if (_t)
		{
			safeClearMapPtr<Iter>(_t);	
			delete _t;
			_t = NULL;
		}
	}
	/*
	*	
	*/
	template <class Iter, class T>
	inline void safeClearVectorPtr_R(T& _t)
	{
		Iter _i = _t.begin();
		for (; _i < _t.end(); ++_i)
		{
			delete (*_i);
		}
		_t.clear();
	}
	/*
	*	
	*/
	template <class Iter, class T>
	inline void safeClearVectorPtr(T& _t)
	{
		if (_t)
		{
			Iter _i = _t->begin();
			for (; _i < _t->end(); ++_i)
			{
				delete (*_i);
			}
			_t->clear();
		}
	}
	/*
	*	
	*/
	template <class Iter, class T>
	inline void safeDeleteVectorPtr(T& _t)
	{
		if (_t)
		{
			safeClearVectorPtr<Iter>(_t);
			delete _t;
			_t = 0;
		}
	}


	/*
	*	
	*/
	template <class Iter, class T>
	inline void safeClearMap(T& _t)
	{
		Iter _i = _t.begin();
		for (; _i != _t.end(); ++_i)
		{
			if (_i->second)
				delete _i->second;
		}
		_t.clear();
	}
	/*
	*	
	*/
	template <class Iter, class T>
	inline void safeClearVector(T& _t)
	{
		Iter _i = _t.begin();
		for (; _i != _t.end(); ++_i)
		{
			delete (*_i);
		}
		_t.clear();
	}
	/*
	*	
	*/
	template< class T >
	inline bool checkIsNull(T const& _t)
	{
		if(0 == _t)
		{
			return true;
		}
		return false;
	}
	template <class T>
	inline bool invalidBound(T _min, T _max, T _v)
	{
		if (_min <= _v && _max >= _v)
			return false;
		return true;
	}
	/*
	*	
	*/
	template <class iter1, class iter2>
	void iter_swap(iter1 i1, iter2 i2)
	{
		typename iter1::value_type tmp = *i1;
		*i1 = *i2;
		i2 = tmp;
	}
	/*
	*	
	*/
	template <class T>
	void safeCloseHandle(T& t)
	{
		if (t)
		{
			CloseHandle(t);
			t = 0;
		}
	}
	/*
	*/
	template <typename T, typename BASE>
	T dynamicCast(BASE const& base)
	{
		T t = dynamic_cast<T>(base);
		assert(t && _T("dynamic cast t is NULL"));
		return t;
	}
	/*
	*/
	template <class T>
	void clamp(T& val, T _min, T _max)
	{
		if (val < _min)			val = _min;
		else if (val > _max)	val = _max;
	}
}

#endif